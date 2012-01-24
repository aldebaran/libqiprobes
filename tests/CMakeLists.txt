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
    INSTRUMENTED_FILES hello.cpp)
qi_create_bin(hello NO_INSTALL
    SRC hello.cpp tp_hello.h)

if (WITH_PROBES)
    qi_use_lib(hello DL)
endif ()
