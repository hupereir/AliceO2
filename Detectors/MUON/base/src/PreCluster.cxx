#include <MUONBase/PreCluster.h>
#include <MUONBase/Digit.h>

using namespace AliceO2::MUON;

//___________________________________________________________________________________________
std::ostream& operator << ( std::ostream& out, const PreCluster& preCluster )
{
  for( int i = 0; i < preCluster.fNDigits; ++i )
  {
    out << "PreCluster:" << std::endl;
    out << "  Digit " << i << ": " << preCluster.fDigits[i] << std::endl;
    return out;
  }

  return out;

}
