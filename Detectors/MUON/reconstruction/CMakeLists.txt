set(MODULE_NAME "MUONReconstruction")

O2_SETUP(NAME ${MODULE_NAME})

set(SRCS
   src/DigitSampler.cxx
   src/DigitSampler2.cxx
   src/DigitSink.cxx
)

Set(LIBRARY_NAME ${MODULE_NAME})
set(BUCKET_NAME muon_reconstruction_bucket)

O2_GENERATE_LIBRARY()

Set(LIBRARY_NAME ${MODULE_NAME})
set(BUCKET_NAME muon_reconstruction_bucket)

# Define application
O2_GENERATE_EXECUTABLE(
    EXE_NAME mch-digit-sampler
    SOURCES src/runDigitSampler.cxx
    MODULE_LIBRARY_NAME ${LIBRARY_NAME}
    BUCKET_NAME ${BUCKET_NAME}
)

O2_GENERATE_EXECUTABLE(
    EXE_NAME mch-digit-sampler2
    SOURCES src/runDigitSampler2.cxx
    MODULE_LIBRARY_NAME ${LIBRARY_NAME}
    BUCKET_NAME ${BUCKET_NAME}
)

O2_GENERATE_EXECUTABLE(
    EXE_NAME mch-digit-sink
    SOURCES src/runDigitSink.cxx
    MODULE_LIBRARY_NAME ${LIBRARY_NAME}
    BUCKET_NAME ${BUCKET_NAME}
)
