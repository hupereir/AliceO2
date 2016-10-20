#include <MUONBase/Digit.h>
#include <FairMQLogger.h>

using namespace AliceO2::MUON;

//____________________________________________________________
bool Digit::Serialize( void*& buffer, int& size ) const
{
  if( size < Size() ) return false;

  *(reinterpret_cast<uint32_t*>(buffer)) = fId;
  buffer = (reinterpret_cast<uint32_t*>(buffer)+1);

  *(reinterpret_cast<uint16_t*>(buffer)) = fIndex;
  buffer = (reinterpret_cast<uint16_t*>(buffer)+1);

  *(reinterpret_cast<uint16_t*>(buffer)) = fADC;
  buffer = (reinterpret_cast<uint16_t*>(buffer)+1);

  size -= Size();
  return true;

}

//____________________________________________________________
bool Digit::Deserialize( void*& buffer, int& size )
{

  if( size < Size() ) return false;

  fId = *( static_cast<uint32_t*>(buffer) );
  buffer = (reinterpret_cast<uint32_t*>(buffer)+1);

  fIndex = *( static_cast<uint16_t*>(buffer) );
  buffer = (reinterpret_cast<uint16_t*>(buffer)+1);

  fADC = *( static_cast<uint16_t*>(buffer) );
  buffer = (reinterpret_cast<uint16_t*>(buffer)+1);
  size -= Size();

  return true;

}
