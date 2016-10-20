//
//  DigitSampler.h
//  ALICEO2
//
//  Created by Hugo Pereira Da Costa on Oct 18 2016
//
//

#ifndef __ALICEO2__MUON__DigitSampler__
#define __ALICEO2__MUON__DigitSampler__

#include "MUONBase/Digit.h"

// from aliroot
#include <AliMpConstants.h>
#include <AliMUONTrackerDDLDecoderEventHandler.h>
#include <AliMUONTrackerDDLDecoder.h>

// from fairMQ
#include <FairMQDevice.h>

#include <unordered_map>

/// forward declarations
class AliRawReader;

namespace AliceO2
{

  namespace MUON
  {

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

      /// serializer
      void* Serialize( Digit::List ) const;

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

      /// create raw reader
      AliRawReader* fRawReader = nullptr;

      /// current event number
      int fEvent = 0;

      /// true if event could not be properly decoded
      Bool_t fBadEvent = false;

      /// running vector of digits
      Digit::List fDigits;

      friend class AliMUONTrackerDDLDecoder<RawDecoderHandler>;

    };

  }

}


#endif
