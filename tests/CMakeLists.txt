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

qi_add_probes(tp_hello.in.h
    PROVIDER qi_probes_tests_hello
    INSTRUMENTED_FILES hello.cpp hello_say.cpp)
qi_create_bin("hello${_suffix}" NO_INSTALL
    SRC
    hello.cpp
    hello_say.cpp
    ${tp_hello_SOURCES})
qi_use_probes("hello${_suffix}" tp_hello)


qi_add_probes(subdir/tp_subdirhello.in.h
    PROVIDER qi_probes_tests_subdirhello
    INSTRUMENTED_FILES subdir/subdirhello.cpp subdir/subdirhello_say.cpp)
qi_create_bin("subdirhello${_suffix}" NO_INSTALL
    SRC
    subdir/subdirhello.cpp
    subdir/subdirhello_say.cpp
    ${tp_subdirhello_SOURCES})
qi_use_probes("subdirhello${_suffix}" tp_subdirhello)
