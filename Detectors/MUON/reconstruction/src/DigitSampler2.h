//
//  DigitSampler2.h
//  ALICEO2
//
//  Created by Hugo Pereira Da Costa on Oct 18 2016
//
//

#ifndef __ALICEO2__MUON__DigitSampler2__
#define __ALICEO2__MUON__DigitSampler2__

// from fairMQ
#include <FairMQDevice.h>

#include <unordered_map>
#include <vector>

/// forward declarations
class TTree;
class TFile;
class AliMUONVDigitStore;

namespace AliceO2
{

  namespace MUON
  {

    /// forward declaration
    class Digit;

    class DigitSampler2: public FairMQDevice
    {
      public:

      /// constructor
      DigitSampler2();

      /// destructor
      virtual ~DigitSampler2();

      protected:

      /// task initialization
      virtual void InitTask();

      /// running
      virtual bool ConditionalRun();

      private:

      /// add a digit
      Digit* AddDigit( void );

      /// input file
      TFile* fInputFile = nullptr;

      /// input file
      TTree* fInputTree = nullptr;

      /// digit store
      AliMUONVDigitStore* fDigitStore = nullptr;

      /// current event number
      int fEvent = 0;

      /// running vector of digits
      using DigitList = std::vector<Digit>;
      DigitList* fDigits = nullptr;

    };

  }

}


#endif
