##
## CMakeLists.txt
##
## Author(s):
##  - Sebastien BARTHELEMY <sbarthelemy@aldebaran-robotics.com>
##
## Copyright (C) 2009, 2011, 2012 Aldebaran Robotics
##

find_package(qiprobes)

qi_add_probes(tp_hello.in.h
    PROVIDER qi_probes_tests_hello
    INSTRUMENTED_FILES hello.cpp hello_say.cpp)
qi_create_bin(hello NO_INSTALL
    SRC
    hello.cpp
    hello_say.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/tp_hello.h)

qi_add_probes(subdir/tp_subdirhello.in.h
    PROVIDER qi_probes_tests_subdir
    INSTRUMENTED_FILES subdir/subdir.cpp subdir_say.cpp)
qi_create_bin(subdirhello NO_INSTALL
    SRC
    subdir/subdirhello.cpp
    subdir/subdirhello_say.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/tp_subdirhello.h)
if (WITH_PROBES)
    qi_use_lib(hello DL)
    qi_use_lib(subdirhello DL)
endif ()
