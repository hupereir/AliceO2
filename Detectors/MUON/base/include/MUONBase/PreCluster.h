//
//  PreCluster.h
//  ALICEO2
//
//  Created by Hugo Pereira Da Costa on Oct 18 2016
//
//

#ifndef __ALICEO2__MUON__PreCluster__
#define __ALICEO2__MUON__PreCluster__

#include <iostream>
#include <cstdint>

#include <iostream>
#include <cstdint>

namespace AliceO2
{

  namespace MUON
  {

    class Digit;

    /// digit storage
    struct PreCluster
    {

      public:

      /// number of digits
      uint16_t fNDigits = 0;

      /// pointer to first digit in array
      Digit* fDigits = nullptr;

      /// streamer
      friend std::ostream& operator << ( std::ostream& out, const PreCluster& precluster );

    };

  }

}

#endif
