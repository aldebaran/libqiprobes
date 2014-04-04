cmake_minimum_required(VERSION 2.8.0)
project(qiprobesctrl)
enable_testing()
find_package(qibuild)

set(H
  qiprobes/ctrl.hpp
  )

set(C
  src/ctrl.cpp
  )

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
if(WITH_PROBES)
  add_definitions(" -DWITH_PROBES")
endif()
qi_create_lib(qiprobesctrl STATIC ${H} ${C})
qi_use_lib(qiprobesctrl QI BOOST BOOST_THREAD LTTNG_TOOLS)
qi_stage_lib(qiprobesctrl)

add_subdirectory(tests)
