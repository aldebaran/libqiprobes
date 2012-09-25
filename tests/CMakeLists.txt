##
## CMakeLists.txt
##
## Author(s):
##  - Sebastien BARTHELEMY <sbarthelemy@aldebaran-robotics.com>
##
## Copyright (C) 2009, 2011, 2012 Aldebaran Robotics
##

find_package(qiprobes)

# change the target suffix according to the config to avoid collisions
if(WITH_PROBES)
  if(DEFINED QIPROBES_PROVIDER_BUILD_MODE)
    string(TOLOWER "-${QIPROBES_PROVIDER_BUILD_MODE}" _suffix)
  else()
    set(_suffix "-shared") # default
  endif()
else()
  set(_suffix "-off")
endif()

qi_create_probe(helloprobe${_suffix} tp_hello.in.h
    PROVIDER_NAME qi_probes_tests_hello
    INSTRUMENTED_FILES hello.cpp hello_say.cpp)
qi_instrument_files(helloprobe${_suffix}
    hello.cpp hello_say.cpp)
qi_create_bin("hello${_suffix}" NO_INSTALL
    SUBMODULE
    helloprobe${_suffix}
    SRC
    hello.cpp
    hello_say.cpp)


qi_create_probe(subdirhelloprobe${_suffix} subdir/tp_subdirhello.in.h
    PROVIDER_NAME qi_probes_tests_subdirhello)
qi_instrument_files(subdirhelloprobe${_suffix}
    subdir/subdirhello.cpp subdir/subdirhello_say.cpp)
qi_create_bin("subdirhello${_suffix}" NO_INSTALL
    SUBMODULE
    subdirhelloprobe${_suffix}
    SRC
    subdir/subdirhello.cpp
    subdir/subdirhello_say.cpp)
