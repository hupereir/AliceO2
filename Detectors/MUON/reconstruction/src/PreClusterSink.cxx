/**
 * PreClusterSink.cxx
 *
 * @since 2016-10-18
 * @author H. Pereira Da Costa
 */

#include "PreClusterSink.h"

#include "MUONBase/PreCluster.h"

#include <FairMQLogger.h>
#include <string>

using namespace AliceO2::MUON;

//_________________________________________________________________________________________________
PreClusterSink::PreClusterSink()
{
  // register a handler for data arriving on "data2" channel
  OnData("data", &PreClusterSink::HandleData);
}

//_________________________________________________________________________________________________
PreClusterSink::~PreClusterSink()
{}

//_________________________________________________________________________________________________
bool PreClusterSink::HandleData(FairMQMessagePtr& msg, int /*index*/)
{

  auto clusters = Deserialize( msg->GetData(), msg->GetSize() );
  fEvent++;
  LOG(INFO) << "Recieving " << clusters.size() << " clusters for event " << fEvent;
  for( auto&& cluster:clusters )
  { LOG(INFO) << "recieved: " << cluster; }

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
