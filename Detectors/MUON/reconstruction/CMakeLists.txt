set(MODULE_NAME "MUONReconstruction")

O2_SETUP(NAME ${MODULE_NAME})

set(SRCS
   src/DigitSampler.cxx
)

set(HEADERS
   include/MUONReconstruction/DigitSampler.h
)

Set(LIBRARY_NAME ${MODULE_NAME})
set(BUCKET_NAME muon_reconstruction_bucket)

O2_GENERATE_LIBRARY()
