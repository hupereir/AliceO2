//
//  Digit.h
//  ALICEO2
//
//  Created by Hugo Pereira Da Costa on Oct 18 2016
//
//

#ifndef __ALICEO2__MUON__Digit__
#define __ALICEO2__MUON__Digit__

#include <iostream>
#include <cstdint>
#include <vector>

namespace AliceO2
{

  namespace MUON
  {

    /// utility class to encode/decode digit unique id
    class DigitId
    {

      public:

      /// constructor
      DigitId( uint32_t id ):
        fId( id ),
        fDeId( id & 0xFFF ),
        fManuId( ( id & 0xFFF000 ) >> 12 ),
        fManuChannel( ( id & 0x3F000000 ) >> 24 ),
        fCathode( ( id & 0x40000000 ) >> 30 )
      {}

      /// constructor
      DigitId( int deId, int manuChannel, int manuId, int cathode ):
        fId( deId | ( manuId << 12 ) | ( manuChannel << 24 )  | ( cathode << 30 ) ),
        fDeId( deId ),
        fManuId( manuId ),
        fManuChannel( manuChannel ),
        fCathode( cathode )
      {}

      uint32_t fId = 0;
      int fDeId = 0;
      int fManuId = 0;
      int fManuChannel = 0;
      int fCathode = 0;

      /// streamer
      friend std::ostream& operator << ( std::ostream& out, const DigitId& digit )
      {
        out << "DE: " << digit.fDeId << " ManuId: " << digit.fManuId << " Channel: " << digit.fManuChannel << " Cathode: " << digit.fCathode;
        return out;
      }

    };

    /// digit storage
    struct Digit
    {

      public:

      /// unique id
      uint32_t fId = 0;

      /// index
      uint16_t fIndex = 0;

      /// adc
      uint16_t fADC = 0;

      using List = std::vector<Digit>;

      friend std::ostream& operator << ( std::ostream& out, const Digit& digit )
      {
        out << DigitId( digit.fId ) << " index: " << digit.fIndex << " adc: " << digit.fADC;
        return out;
      }

    };

  }

}

#endif
