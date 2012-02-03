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

