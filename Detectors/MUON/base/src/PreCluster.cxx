#include <MUONBase/PreCluster.h>
#include <FairMQLogger.h>

namespace AliceO2
{
  namespace MUON
  {

    //___________________________________________________________________________________________
    std::ostream& operator << ( std::ostream& out, const PreCluster& preCluster )
    {
      out << "PreCluster:" << std::endl;
      for( auto&& digit: preCluster.fDigits )
      { out << "  Digit: " << digit << std::endl; }

      return out;

    }

    //____________________________________________________________
    int PreCluster::Size( void ) const
    {
      // start with the size used to write the number of digits
      int size = sizeof( uint16_t );

      // add the size needed for each digit
      for( auto&& digit:fDigits ) size += digit.Size();
      return size;
    }

    //____________________________________________________________
    bool PreCluster::Serialize( void*& buffer, int& size ) const
    {
      if( size < Size() ) return false;

      // write number of digits
      *(reinterpret_cast<uint16_t*>(buffer)) = fDigits.size();
      buffer = (reinterpret_cast<uint16_t*>(buffer)+1);
      size -= sizeof( uint16_t );

      // write digits
      for( auto&& digit:fDigits ) digit.Serialize( buffer, size );

      return true;
    }

    //____________________________________________________________
    bool PreCluster::Deserialize( void*& buffer, int& size )
    {

      if( size < sizeof( uint16_t ) ) return false;
      const uint16_t nDigits = *( reinterpret_cast<uint16_t*>(buffer) );
      buffer = (reinterpret_cast<uint16_t*>(buffer)+1);
      size -= sizeof( uint16_t );

      for( int i = 0; i < nDigits; ++i )
      {
        Digit digit;
        if( !digit.Serialize( buffer, size ) )
        {
          LOG(ERROR) << "Failed reading the expected number of digits";
          return false;
        }

        fDigits.push_back( digit );
      }

      return true;

    }

  }

}
