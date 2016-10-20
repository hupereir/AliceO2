//
//  PreCluster.h
//  ALICEO2
//
//  Created by Hugo Pereira Da Costa on Oct 18 2016
//
//

#ifndef __ALICEO2__MUON__PreCluster__
#define __ALICEO2__MUON__PreCluster__

#include <MUONBase/Digit.h>

#include <iostream>
#include <cstdint>

#include <iostream>
#include <cstdint>

namespace AliceO2
{

  namespace MUON
  {

    /// digit storage
    struct PreCluster
    {

      public:

      /// digits
      Digit::List fDigits;

      /// streamer
      friend std::ostream& operator << ( std::ostream& out, const PreCluster& precluster );

    };

  }

}

#endif
