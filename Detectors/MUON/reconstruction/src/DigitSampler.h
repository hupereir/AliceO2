//
//  DigitSampler.h
//  ALICEO2
//
//  Created by Hugo Pereira Da Costa on Oct 18 2016
//
//

#ifndef __ALICEO2__MUON__DigitSampler__
#define __ALICEO2__MUON__DigitSampler__

// from aliroot
#include <AliMpConstants.h>
#include <AliMUONTrackerDDLDecoderEventHandler.h>
#include <AliMUONTrackerDDLDecoder.h>

// from fairMQ
#include <FairMQDevice.h>

#include <unordered_map>
#include <vector>

/// forward declarations
class AliRawReader;

namespace AliceO2
{

  namespace MUON
  {

    /// forward declaration
    class Digit;

    class DigitSampler: public FairMQDevice
    {
      public:

      /// constructor
      DigitSampler();

      /// destructor
      virtual ~DigitSampler();

      protected:

      /// task initialization
      virtual void InitTask();

      /// running
      virtual bool ConditionalRun();

      private:

      /// create mapping
      void CreateMapping( void );

      /// add a digit
      Digit* AddDigit( void );

      /// internal class to interface the raw data decoder
      class RawDecoderHandler : public AliMUONTrackerDDLDecoderEventHandler
      {

        public:
        void OnNewBusPatch(const AliMUONBusPatchHeaderStruct*, const void* );
        void OnData(UInt_t, bool );
        void OnError(ErrorCode, const void* );
        void SetParent( void* parent )
        { fParent = parent; }

        private:
        void* fParent;
        Int_t fDetElemId;

      };

      /// maps detector id to the plane type of cathode 0
      /** this is needed to get the right unique id for decoded digits */
      std::unordered_map<int,AliMp::PlaneType> fDEPlaneType;

      /// raw data decoder
      AliMUONTrackerDDLDecoder<RawDecoderHandler> fRawDecoder;

      /// input file name
      std::string fDatafile;

      /// create raw reader
      AliRawReader* fRawReader = nullptr;

      /// current event number
      int fEvent = 0;

      /// true if event could not be properly decoded
      Bool_t fBadEvent = false;

      /// running vector of digits
      std::vector<Digit>* fDigits = nullptr;

      friend class AliMUONTrackerDDLDecoder<RawDecoderHandler>;

    };

  }

}


#endif
