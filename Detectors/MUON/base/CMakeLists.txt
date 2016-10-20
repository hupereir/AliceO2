set(MODULE_NAME "MUONBase")

O2_SETUP(NAME ${MODULE_NAME})

set(SRCS
   src/Digit.cxx
   src/PreCluster.cxx
)

set(HEADERS
   include/MUONBase/Digit.h
   include/MUONBase/PreCluster.h
)

Set(LIBRARY_NAME ${MODULE_NAME})
set(BUCKET_NAME muon_base_bucket)

O2_GENERATE_LIBRARY()
