#include "DigitSampler.h"

#include <runFairMQDevice.h>

namespace bpo = boost::program_options;

//_________________________________________________________________________________________________
void addCustomOptions(bpo::options_description& options)
{
    options.add_options()
      ("output", bpo::value<std::string>()->default_value( "tcp://*:5555" ), "Output socket address" )
      ("datafile", bpo::value<std::string>(), "Root file from which data are read")
      ("cdbpath", bpo::value<std::string>(), "CDB path")
      ("run", bpo::value<int>(), "run number")
      ;
}

//_________________________________________________________________________________________________
FairMQDevice* getDevice(const FairMQProgOptions& /*config*/)
{ return new AliceO2::MUON::DigitSampler(); }
