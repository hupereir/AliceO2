//
//  DigitSampler.h
//  ALICEO2
//
//  Created by Hugo Pereira Da Costa on Oct 18 2016
//
//

#ifndef __ALICEO2__MUON__Digit__
#define __ALICEO2__MUON__Digit__

namespace AliceO2
{

  namespace MUON
  {
    class Digit
    {

      public:

      // unique id
      unsigned int fId = 0;

      // index
      unsigned short fIndex = 0;

      // adc
      unsigned short fADC = 0;

    };

  }

}

#endif
