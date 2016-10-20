#include "PreClusterSink.h"

#include <FairMQLogger.h>
#include <FairMQProgOptions.h>

#include <boost/program_options.hpp>

namespace bpo = boost::program_options;

//_________________________________________________________________________________________________
void addCustomOptions(bpo::options_description& options)
{
    options.add_options()
      ("source", bpo::value<std::string>()->default_value( "tcp://localhost:5555" ), "source socket address" )
      ;
}

//_________________________________________________________________________________________________
FairMQDevice* getDevice(const FairMQProgOptions& /*config*/)
{ return new AliceO2::MUON::PreClusterSink(); }

// _________________________________________________________________________________________________
int runStateMachine(FairMQDevice& device, FairMQProgOptions& config)
{

  device.SetConfig(config);
  std::string control = config.GetValue<std::string>("control");

  // create output channel
  auto source = config.GetValue<std::string>( "source" );
  LOG(INFO) << "source channel: " << source;

  FairMQChannel channel("pull", "connect", source );

  // define transport and assign channel
  device.SetTransport("zeromq");
  device.fChannels["data1"].push_back(channel);

  device.ChangeState(FairMQDevice::INIT_DEVICE);
  device.WaitForEndOfState(FairMQDevice::INIT_DEVICE);

  device.ChangeState(FairMQDevice::INIT_TASK);
  device.WaitForEndOfState(FairMQDevice::INIT_TASK);

  device.ChangeState(FairMQDevice::RUN);
  device.InteractiveStateLoop();

  return 0;

}


//_________________________________________________________________________________________________
int main(int argc, char** argv)
{
  try
  {

    boost::program_options::options_description customOptions("Custom options");
    addCustomOptions(customOptions);

    FairMQProgOptions config;
    config.AddToCmdLineOptions(customOptions);
    config.ParseAll(argc, argv);

    std::unique_ptr<FairMQDevice> device(getDevice(config));
    int result = runStateMachine(*device, config);

    if (result > 0) return 1;

  } catch (std::exception& e) {

    LOG(ERROR) << "Unhandled Exception reached the top of main: " << e.what() << ", application will now exit";
    return 1;

  }

  return 0;

}
