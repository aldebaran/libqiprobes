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
add_subdirectory("tests")

# Install CMake code where it belongs
set(_qiprobes_cmake_install_dest)
if(UNIX)
  set(_qiprobes_cmake_install_dest
    ${CMAKE_INSTALL_PREFIX}/share/cmake/
  )
else()
  # No need to try /usr/local/share/
  # on windows:
  set(_qiprobes_cmake_install_dest
    ${CMAKE_ROOT}/Modules
  )
endif()

install(
  FILES
    qiprobes/tpl.py
    qiprobes/tp_probes.in.h
    qiprobes/tp_probes.in.c
  DESTINATION
    "${_qiprobes_cmake_install_dest}/qiprobes"
)
