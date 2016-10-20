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

  // load digits
  fDigitStore->Clear();
  if( !fInputTree->GetEntry(fEvent) ) return false;
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

    // LOG(INFO) << "Adding " << localDigit;

  }

  if( localDigits.empty() ) return true;

  LOG(INFO) << "Sending " << localDigits.size() << " digits for event " << fEvent;

  // create message and send
  int size = 0;
  for( auto&& digit:localDigits ) { size += digit.Size(); }
  void* buffer = Serialize( localDigits );
  FairMQMessagePtr msg( NewMessage(
    buffer, size,
    [](void* data, void* /*object*/) { free( data ); } ) );


  if( Send(msg, "data1") < 0 )
  {
    LOG(ERROR) << "Sending failed";
    return false;
  }

  return true;

}

//_________________________________________________________________________________________________
void* DigitSampler2::Serialize( const Digit::List& digits ) const
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
