//
//  DigitSink.h
//  ALICEO2
//
//  Created by Hugo Pereira Da Costa on Oct 18 2016
//
//

#ifndef __ALICEO2__MUON__DigitSink__
#define __ALICEO2__MUON__DigitSink__

#include "MUONBase/Digit.h"

#include <FairMQDevice.h>

#include <vector>

namespace AliceO2
{

  namespace MUON
  {

    class DigitSink: public FairMQDevice
    {

      public:

      /// constructor
      DigitSink();

      /// destructor
      virtual ~DigitSink();

      protected:

      /// running
      bool HandleData(FairMQMessagePtr&, int);

      private:

      /// deserializer
      Digit::List Deserialize( void*, int ) const;


    };

  }

}


#endif
