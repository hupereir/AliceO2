//
//  PreClusterSink.h
//  ALICEO2
//
//  Created by Hugo Pereira Da Costa on Oct 18 2016
//
//

#ifndef __ALICEO2__MUON__PreClusterSink__
#define __ALICEO2__MUON__PreClusterSink__

#include "MUONBase/PreCluster.h"

#include <FairMQDevice.h>

namespace AliceO2
{

  namespace MUON
  {

    class PreClusterSink: public FairMQDevice
    {

      public:

      /// constructor
      PreClusterSink();

      /// destructor
      virtual ~PreClusterSink();

      protected:

      /// running
      bool HandleData(FairMQMessagePtr&, int);

      private:

      /// deserializer
      PreCluster::List Deserialize( void*, int ) const;

      /// event
      int fEvent = 0;

    };

  }

}


#endif
