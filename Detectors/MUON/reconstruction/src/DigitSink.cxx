/**
 * DigitSink.cxx
 *
 * @since 2016-10-18
 * @author H. Pereira Da Costa
 */

#include "MUONBase/Digit.h"
#include "DigitSink.h"

#include <FairMQLogger.h>
#include <string>

using namespace AliceO2::MUON;

//_________________________________________________________________________________________________
DigitSink::DigitSink()
{
  // register a handler for data arriving on "data2" channel
  OnData("data1", &DigitSink::HandleData);
}

//_________________________________________________________________________________________________
DigitSink::~DigitSink()
{}

//_________________________________________________________________________________________________
bool DigitSink::HandleData(FairMQMessagePtr& msg, int /*index*/)
{

  LOG(INFO) << "Recieved message of size " << msg->GetSize() << " number of digits: " << msg->GetSize()/sizeof( Digit );

  const Digit* digits = static_cast<const Digit*>( msg->GetData() );
  const int nDigits = msg->GetSize()/sizeof( Digit );
  for( int index = 0; index < nDigits; ++index )
  { LOG(INFO) << "recieved: " << digits[index]; }

  return true;

}
