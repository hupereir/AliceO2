/**
 * DigitSink.cxx
 *
 * @since 2016-10-18
 * @author H. Pereira Da Costa
 */

#include "DigitSink.h"

#include <FairMQLogger.h>
#include <string>

using namespace AliceO2::MUON;

//_________________________________________________________________________________________________
DigitSink::DigitSink()
{
  // register a handler for data arriving on "data2" channel
  OnData("data", &DigitSink::HandleData);
}

//_________________________________________________________________________________________________
DigitSink::~DigitSink()
{}

//_________________________________________________________________________________________________
bool DigitSink::HandleData(FairMQMessagePtr& msg, int /*index*/)
{

  // use serializer
  auto digits = Deserialize( msg->GetData(), msg->GetSize() );
  fEvent++;
  LOG(INFO) << "Recieving " << digits.size() << " digits for event " << fEvent;
  // for( auto&& digit:digits )
  // { LOG(INFO) << "recieved: " << digit; }

  return true;

}

//_________________________________________________________________________________________________
Digit::List DigitSink::Deserialize( void* buffer, int size ) const
{

  Digit::List digits;

  while( true )
  {

    Digit digit;
    if( digit.Deserialize( buffer, size ) ) digits.push_back( digit );
    else break;
  }

  return digits;

}
