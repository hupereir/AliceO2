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

      /// size
      int Size( void ) const;

      /// serialize to buffer
      bool Serialize( void*& buffer, int& size ) const;

      /// deseriaze from buffer
      bool Deserialize( void*& buffer, int& size );

      /// shortcut for list of digits
      using List = std::vector<Digit>;

      /// streamer
      friend std::ostream& operator << ( std::ostream& out, const PreCluster& precluster );

    };

  }

}

#endif
