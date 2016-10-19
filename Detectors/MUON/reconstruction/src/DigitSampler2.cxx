/**
 * DigitSampler2.cxx
 *
 * @since 2016-10-18
 * @author H. Pereira Da Costa
 */

#include <thread>
#include <chrono>

#include "MUONBase/Digit.h"
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

  // load digits
  fDigitStore->Clear();
  fInputTree->GetEntry(fEvent);
  ++fEvent;

  if( fDigitStore->GetSize() < 1 ) { return true; }

  // create digit array
  using DigitList = std::vector<Digit>;
  fDigits = new DigitList;

  // register digits with positive charge
  int status(0);
  auto digits = fDigitStore->CreateIterator();
  while( auto digit = static_cast<AliMUONVDigit*>(digits->Next() ) )
  {

    // skip digit if charge is negative
    if( digit->Charge() <= 0 ) continue;

    // store
    auto localDigit = AddDigit();
    localDigit->fId = digit->GetUniqueID();
    localDigit->fIndex = 0;
    localDigit->fADC = digit->ADC();

    LOG(INFO) << "Adding " << localDigit;

  }

  // do nothing if there is no digits
  if( fDigits->empty() )
  {
    delete fDigits;
    return true;
  }

  LOG(INFO) << "Sending " << fDigits->size() << " digits for event " << fEvent;

  // create message and send
  FairMQMessagePtr msg(
    NewMessage( &fDigits->at(0), fDigits->size()*sizeof(Digit),
    [](void* /*data*/, void* object) { delete static_cast<DigitList*>(object); },
    fDigits));

  if( Send(msg, "data1") < 0 )
  {
    LOG(ERROR) << "Sending failed";
    return false;
  }

  return true;

}
//_________________________________________________________________________________________________
Digit* DigitSampler2::AddDigit( void )
{
  if( !fDigits ) return nullptr;
  fDigits->push_back( Digit() );
  return &fDigits->back();
}
