/**
 * DigitSampler2.cxx
 *
 * @since 2016-10-18
 * @author H. Pereira Da Costa
 */

#include <thread>
#include <chrono>

#include "DigitSampler2.h"

#include <TFile.h>
#include <TTree.h>
#include <AliMUONVDigitStore.h>

#include <FairMQLogger.h>
#include <FairMQProgOptions.h>

#include <string>

using namespace AliceO2::MUON;

//_________________________________________________________________________________________________
DigitSampler2::DigitSampler2()
{}

//_________________________________________________________________________________________________
DigitSampler2::~DigitSampler2()
{
  delete fDigitStore;
  delete fInputTree;
  delete fInputFile;
}

//_________________________________________________________________________________________________
void DigitSampler2::InitTask( void )
{

  LOG(INFO) << "Initializing";

  // filename
  auto dataFile = fConfig->GetValue<std::string>( "datafile" );
  if( dataFile.empty() )
  {
    LOG(ERROR) << "datafile is not set";
    return;
  }

  // TFile
  fInputFile = TFile::Open( dataFile.c_str(), "READ" );
  if( !fInputFile )
  {
    LOG(ERROR) << "cannot open " << dataFile;
    return;
  }

  // Tree
  fInputTree = dynamic_cast<TTree*>(fInputFile->Get("TreeD"));
  if (!fInputTree)
  {
    LOG(ERROR) << "cannot find tree named \"TreeD\" in file " << dataFile;
    return;
  }

  // store
  fDigitStore = AliMUONVDigitStore::Create(*fInputTree);
  if (!( fDigitStore && fDigitStore->Connect( *fInputTree ) ) )
  {
    LOG(ERROR) << "cannot create digit store from file " << dataFile;
    return;
  }

}

//_________________________________________________________________________________________________
bool DigitSampler2::ConditionalRun( void )
{

  std::this_thread::sleep_for(std::chrono::seconds(1));

  LOG(INFO) << "Processing event " << fEvent;

  if( fEvent >= 1 ) return true;

  // load digits
  fDigitStore->Clear();
  fInputTree->GetEntry(fEvent);
  ++fEvent;

  if( fDigitStore->GetSize() < 1 ) { return true; }

  // using serializer
  Digit::List localDigits;
  auto digits = fDigitStore->CreateIterator();
  while( auto digit = static_cast<AliMUONVDigit*>(digits->Next() ) )
  {

    // skip digit if charge is negative
    if( digit->Charge() <= 0 ) continue;

    // store
    Digit localDigit;
    localDigit.fId = digit->GetUniqueID();
    localDigit.fIndex = 0;
    localDigit.fADC = digit->ADC();

    localDigits.push_back( localDigit );

    LOG(INFO) << "Adding " << localDigit;

  }

  if( localDigits.empty() ) return true;

  LOG(INFO) << "Sending " << localDigits.size() << " digits for event " << fEvent;

  // create message and send
  void* buffer = Serialize( localDigits );
  FairMQMessagePtr msg( NewMessage(
    buffer, localDigits.size()*( sizeof( uint32_t ) + 2*sizeof( uint16_t ) ),
    [](void* data, void* /*object*/) { free( data ); } ) );


  if( Send(msg, "data1") < 0 )
  {
    LOG(ERROR) << "Sending failed";
    return false;
  }

  return true;

}

//_________________________________________________________________________________________________
void* DigitSampler2::Serialize( Digit::List digits ) const
{

  if( digits.empty() ) return nullptr;

  void* first = malloc( digits.size()*( sizeof( uint32_t ) + 2*sizeof( uint16_t ) ) );
  void* data = first;

  for( auto&& digit:digits )
  {

    *(reinterpret_cast<uint32_t*>(data)) = digit.fId;
    data = (reinterpret_cast<uint32_t*>(data)+1);

    *(reinterpret_cast<uint16_t*>(data)) = digit.fIndex;
    data = (reinterpret_cast<uint16_t*>(data)+1);

    *(reinterpret_cast<uint16_t*>(data)) = digit.fADC;
    data = (reinterpret_cast<uint16_t*>(data)+1);

  }

  return first;

}
