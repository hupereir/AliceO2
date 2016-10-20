#include <MUONBase/PreCluster.h>

using namespace AliceO2::MUON;

//___________________________________________________________________________________________
std::ostream& operator << ( std::ostream& out, const PreCluster& preCluster )
{
  out << "PreCluster:" << std::endl;
  for( auto&& digit: preCluster.fDigits )
  { out << "  Digit: " << digit << std::endl; }

  return out;

}
