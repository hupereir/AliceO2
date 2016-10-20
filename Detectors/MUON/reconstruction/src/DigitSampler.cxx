/**
 * DigitSampler.cxx
 *
 * @since 2016-10-18
 * @author H. Pereira Da Costa
 */

#include <thread>
#include <chrono>

#include "DigitSampler.h"

#include <AliCDBManager.h>
#include <AliMpDDLStore.h>
#include <AliMpDEIterator.h>
#include <AliMpSegmentation.h>
#include <AliMpVSegmentation.h>
#include <AliMUONCDB.h>
#include <AliMUONConstants.h>
#include <AliMUONVDigit.h>
#include <AliRawReader.h>

#include <FairMQLogger.h>
#include <FairMQProgOptions.h>

#include <string>

using namespace AliceO2::MUON;

//_________________________________________________________________________________________________
DigitSampler::DigitSampler()
{

  // setup parent
  fRawDecoder.GetHandler().SetParent(this);

}

//_________________________________________________________________________________________________
DigitSampler::~DigitSampler()
{ delete fRawReader; }

//_________________________________________________________________________________________________
void DigitSampler::InitTask( void )
{

  LOG(INFO) << "Initializing";

  // filename
  auto dataFile = fConfig->GetValue<std::string>( "datafile" );
  if( dataFile.empty() )
  {
    LOG(ERROR) << "datafile is not set";
    return;
  }

  // cdb path
  std::string cdbPath = fConfig->GetValue<std::string>("cdbpath");
  if( cdbPath.empty() )
  {
    LOG(ERROR) << "cdbpath is not set";
    return;
  }

  // run number
  int runNumber = fConfig->GetValue<int>("run");
  if( runNumber<= 0 )
  {
    LOG(ERROR) << "Invalid run number: " << runNumber;
    return;
  }

  // cdb manager
  AliCDBManager* cdbManager = AliCDBManager::Instance();
  if( !cdbManager )
  {
    LOG(ERROR) << "CDB manager instance does not exist";
    return;
  }

  // assign cdb path and check
  cdbManager->SetDefaultStorage(cdbPath.c_str());
  cdbManager->SetRun(runNumber);

  // Load the MUON mapping from CDB and create the internal mapping
  if( !AliMUONCDB::LoadMapping() )
  {
    LOG(ERROR) << "Failed to load muon mapping";
    return;
  }

  // create internal mapping
  CreateMapping();

  // create raw reader
  fRawReader = AliRawReader::Create( dataFile.c_str() );
  fEvent = 0;

}

//_________________________________________________________________________________________________
bool DigitSampler::ConditionalRun( void )
{

  int maxDDL = 20;
  if( !fRawReader->NextEvent() ) return true;

  ++fEvent;

  // load ddls
  int ddl = 0;
  while( ddl < maxDDL )
  {

    while( ddl < maxDDL )
    {
      fRawReader->Reset();
      fRawReader->Select("MUONTRK", ddl, ddl);
      if (fRawReader->ReadHeader()) break;

      LOG(WARN) << "Skipping DDL " << ddl+1 << " which does not seem to be there";

      ddl++;
    }

    // if last ddl was read, go to next event
    if( ddl >= maxDDL ) break;

    // read data size
    auto dataSize = fRawReader->GetDataSize(); // in bytes

    // allocate a buffer with proper size
    unsigned char* buffer = nullptr;
    try {

      buffer = new unsigned char[dataSize];

    } catch (const std::bad_alloc&) {

      LOG(ERROR) << "Could not allocate buffer space. Cannot decode DDL";
      continue;

    }

    // read
    if( !fRawReader->ReadNext( buffer, dataSize ) )
    {

      delete[] buffer;
      continue;

    }

    // create digit array
    fDigits.clear();

    // increment ddl decode buffer to fill digits
    ++ddl;
    bool decode = fRawDecoder.Decode( buffer, dataSize );
    delete[] buffer;

    // do nothing if no digits
    if( fDigits.empty() ) continue;

    // info
    LOG(INFO) << "Sending " << fDigits.size() << " digits for event " << fEvent << " and ddl " << ddl;

    // create message and send
    int size = 0;
    for( auto&& digit:fDigits)
    {
      LOG(INFO) << "Sending " << digit << " for DDL" << ddl;
      size+=digit.Size();
    }

    auto msgBuffer = Serialize( fDigits );

    FairMQMessagePtr msg( NewMessage(
      msgBuffer, size,
      [](void* data, void* /*object*/) { free( data ); } ) );

    if( Send(msg, "data1") < 0 )
    {
      LOG(ERROR) << "Sending failed";
      return false;
    }

  }

  return true;

}

//_________________________________________________________________________________________________
void DigitSampler::CreateMapping()
{

  fDEPlaneType.clear();

  // loop over chambers
  for( int iCh = 0; iCh < AliMUONConstants::NTrackingCh(); ++iCh )
  {

    // loop over detector element
    AliMpDEIterator iterator;
    for( iterator.First(iCh); !iterator.IsDone(); iterator.Next() )
    {

      // get detector element
      const int deId( iterator.CurrentDEId() );

      // get segmentation for cathode 0
      const AliMpVSegmentation* segmentation = AliMpSegmentation::Instance()->GetMpSegmentation( deId, AliMp::kCath0 );

      // store corresponding plane type
      fDEPlaneType.insert( std::make_pair(deId, segmentation->PlaneType()) );

    }

  }

}

//_________________________________________________________________________________________________
Digit* DigitSampler::AddDigit( void )
{
  fDigits.push_back( Digit() );
  return &fDigits.back();
}

//_________________________________________________________________________________________________
void* DigitSampler::Serialize( const Digit::List& digits ) const
{

  if( digits.empty() ) return nullptr;

  // compute total size
  int size = 0;
  for( auto&& digit:digits ) { size += digit.Size(); }
  void* first = malloc( size );
  void* buffer = first;

  // serialize
  for( auto&& digit:digits ) digit.Serialize( buffer, size );

  return first;

}

//_________________________________________________________________________________________________
void DigitSampler::RawDecoderHandler::OnData(UInt_t data, bool /*parityError*/)
{

  // decode the raw data word
  UShort_t manuId;
  UChar_t manuChannel;
  UShort_t adc;
  UnpackADC(data, manuId, manuChannel, adc);
  if (adc <= 0) return;

  // get plane index
  auto sampler = static_cast<DigitSampler*>(fParent);
  auto planeType = (manuId & AliMpConstants::ManuMask(AliMp::kNonBendingPlane)) ? AliMp::kNonBendingPlane : AliMp::kBendingPlane;

  // get cathode index
  auto iCath = ( planeType == sampler->fDEPlaneType[fDetElemId] ) ? 0:1;

  // get pad unique id
  auto padId = AliMUONVDigit::BuildUniqueID(fDetElemId, manuId, manuChannel, iCath );

  // add new digit
  Digit* digit = sampler->AddDigit();
  if( digit )
  {

    digit->fId = padId;
    digit->fIndex = 0;
    digit->fADC = adc;

  } else {

    sampler->fBadEvent = kTRUE;

  }

}

//_________________________________________________________________________________________________
void DigitSampler::RawDecoderHandler::OnNewBusPatch(const AliMUONBusPatchHeaderStruct* header, const void* /*data*/)
{
  // get the detection element ID containing this bus patch
  fDetElemId = AliMpDDLStore::Instance()->GetDEfromBus(header->fBusPatchId);
}

//_________________________________________________________________________________________________
void DigitSampler::RawDecoderHandler::OnError(ErrorCode /*error*/, const void* /*location*/)
{ static_cast<DigitSampler*>(fParent)->fBadEvent = kTRUE; }
