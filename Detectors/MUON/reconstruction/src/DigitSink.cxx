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
  OnData("data1", &DigitSink::HandleData);
}

//_________________________________________________________________________________________________
DigitSink::~DigitSink()
{}

//_________________________________________________________________________________________________
bool DigitSink::HandleData(FairMQMessagePtr& msg, int /*index*/)
{

  LOG(INFO) << "Recieved message of size " << msg->GetSize();

  // use serializer
  auto digits = Deserialize( msg->GetData(), msg->GetSize() );
  for( auto&& digit:digits )
  { LOG(INFO) << "recieved: " << digit; }

  return true;

}

//_________________________________________________________________________________________________
Digit::List DigitSink::Deserialize( void* data, int size ) const
{

  Digit::List digits;
  while( size > 0 )
  {

    Digit digit;
    if( size >= sizeof( uint32_t ) )
    {
      digit.fId = *( static_cast<uint32_t*>(data) );
      data = (reinterpret_cast<uint32_t*>(data)+1);
      size -= sizeof( uint32_t );
    } else break;

    if( size >= sizeof( uint16_t ) )
    {
      digit.fIndex = *( static_cast<uint16_t*>(data) );
      data = (reinterpret_cast<uint16_t*>(data)+1);
      size -= sizeof( uint16_t );
    } else break;

    if( size >= sizeof( uint16_t ) )
    {
      digit.fADC = *( static_cast<uint16_t*>(data) );
      data = (reinterpret_cast<uint16_t*>(data)+1);
      size -= sizeof( uint16_t );
    } else break;

    digits.push_back( digit );

  }

  return digits;

}
