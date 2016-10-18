set(MODULE_NAME "MUONBase")

O2_SETUP(NAME ${MODULE_NAME})

set(SRCS
   src/Digit.cxx
)

set(HEADERS
   include/MUONBase/Digit.h
)

Set(LIBRARY_NAME ${MODULE_NAME})
set(BUCKET_NAME muon_base_bucket)

O2_GENERATE_LIBRARY()
