##
## CMakeLists.txt
##
## Author(s):
##  - Sebastien BARTHELEMY <sbarthelemy@aldebaran-robotics.com>
##
## Copyright (C) 2009, 2011, 2012 Aldebaran Robotics
##

cmake_minimum_required(VERSION 2.8.0)
project(qiprobes)

find_package(qibuild)

add_subdirectory("qiprobes")
add_subdirectory("example")
add_subdirectory("examplenew")

# tests.
#
# If WITH_PROBES==ON, build the same tests with different configs (including
# WITH_PROBES=OFF!).
# The "tests" source directory is included several times with different
# binary directories which serve as distinct namespaces.
#
# The script runtests.py allows to run all the tests and the examples
if(WITH_PROBES)
  set(QIPROBES_PROVIDER_BUILD_MODE "BUILTIN")
  add_subdirectory("tests" "tests-builtin")

  set(QIPROBES_PROVIDER_BUILD_MODE "SHARED")
  add_subdirectory("tests" "tests-shared")

  set(WITH_PROBES OFF)
endif()

add_subdirectory("tests" "tests-off")

