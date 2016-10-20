/**
 * PreClusterSink.cxx
 *
 * @since 2016-10-18
 * @author H. Pereira Da Costa
 */

#include "PreClusterSink.h"

#include "MUONBase/Digit.h"
#include "MUONBase/PreCluster.h"

#include <FairMQLogger.h>
#include <string>

using namespace AliceO2::MUON;

//_________________________________________________________________________________________________
PreClusterSink::PreClusterSink()
{
  // register a handler for data arriving on "data2" channel
  OnData("data1", &PreClusterSink::HandleData);
}

//_________________________________________________________________________________________________
PreClusterSink::~PreClusterSink()
{}

//_________________________________________________________________________________________________
bool PreClusterSink::HandleData(FairMQMessagePtr& msg, int /*index*/)
{

  LOG(INFO) << "Recieved message of size " << msg->GetSize();

  const Digit* digits = static_cast<const Digit*>( msg->GetData() );
  const int nDigits = msg->GetSize()/sizeof( Digit );
  for( int index = 0; index < nDigits; ++index )
  { LOG(INFO) << "recieved: " << digits[index]; }

  return true;

}

//_________________________________________________________________________________________________
PreCluster::List PreClusterSink::Deserialize( void* buffer, int size ) const
{

  PreCluster::List preClusters;

  while( true )
  {

    PreCluster preCluster;
    if( preCluster.Deserialize( buffer, size ) ) preClusters.push_back( preCluster );
    else break;
  }

  return preClusters;

}
