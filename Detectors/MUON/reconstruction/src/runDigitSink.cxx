#include "DigitSink.h"

#include <runFairMQDevice.h>

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
{ return new AliceO2::MUON::DigitSink(); }
