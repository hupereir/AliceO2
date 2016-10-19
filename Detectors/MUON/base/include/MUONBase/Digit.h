//
//  DigitSampler.h
//  ALICEO2
//
//  Created by Hugo Pereira Da Costa on Oct 18 2016
//
//

#ifndef __ALICEO2__MUON__Digit__
#define __ALICEO2__MUON__Digit__

#include <iostream>
#include <cstdint>

namespace AliceO2
{

  namespace MUON
  {
    class Digit
    {

      public:

      // unique id
      uint32_t fId = 0;

      // index
      uint16_t fIndex = 0;

      // adc
      uint16_t fADC = 0;

      friend std::ostream& operator << ( std::ostream& out, const Digit& digit )
      {
        out << "id: " << digit.fId << " index: " << digit.fIndex << " adc: " << digit.fADC;
        return out;
      }

    };

  }

}

#endif
