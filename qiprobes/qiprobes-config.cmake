get_filename_component(_PROBES_CMAKE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

function(append_source_file_property property value)
    foreach(_file IN LISTS ARGN)
    get_source_file_property(_current_value "${_file}" "${property}")
    if ("${_current_value}" STREQUAL "NOTFOUND")
       set(_new_value "${value}")
    else()
       set(_new_value "${_current_value} ${value}")
    endif()
    set_source_files_properties(${_file}
      PROPERTIES "${property}"
      "${_new_value}")
  endforeach()
endfunction(append_source_file_property)

##
# qi_add_probes(tp_sensorlog.in.h
#  PROVIDER qi_sensorlog
#  INSTRUMENTED_FILES alsensorlog.cpp
# )
function(qi_add_probes tp_def)
  # Find python, but avoid using python from python package
  find_program(_python_executable
    NAMES python2 python python.exe
    NO_CMAKE_FIND_ROOT_PATH)
  if (NOT _python_executable)
    qi_error("qi_add_probes needs python executable in PATH")
  endif()
  cmake_parse_arguments(ARG "" "PROVIDER" "INSTRUMENTED_FILES" ${ARGN})
  set(_provider "${ARG_PROVIDER}")
  if(NOT _provider)
    qi_error("PROVIDER argument is mandatory")
  endif()

  set(_instrumented_files ${ARG_INSTRUMENTED_FILES})

  # So that we find the header we are going to generate:
  include_directories("${CMAKE_CURRENT_BINARY_DIR}")

  set(tp_def "${CMAKE_CURRENT_SOURCE_DIR}/${tp_def}")
  if(NOT (${tp_def} MATCHES "^.*\\.in\\.h$"))
    qi_error("Cannot create probe.
    The probe definitions file ${tp_def} does not end with .in.h)
    ")
  endif()
  if(NOT EXISTS ${tp_def})
    qi_error("Cannot create probe.
    Could not find probe definitions file.
    The file should be: ${tp_def})
    ")
  endif()

  # Generate ${tp_def_base}.h
  get_filename_component(_tp_def_base ${tp_def} NAME_WE)
  set(_tp_h ${CMAKE_CURRENT_BINARY_DIR}/${_tp_def_base}.h)
  string(TOUPPER "${_tp_def_base}_H" _tp_h_reinclusion_protection)
  # call an external templating engine to include content from tp_def an
  # get proper dependency declaration.
  add_custom_command(OUTPUT "${_tp_h}"
                     COMMENT "Generating probes in ${_tp_h} ..."
                     COMMAND "${_python_executable}" ARGS
                     "${_PROBES_CMAKE_DIR}/tpl.py"
                        -d _tp_h_reinclusion_protection "${_tp_h_reinclusion_protection}"
                        -d _provider "${_provider}"
                        -d _tp_h "${_tp_h}"
                        -i _tp_def_contents "${tp_def}"
                        -o "${_tp_h}"
                        "${_PROBES_CMAKE_DIR}/tp_probes.in.h"
                     MAIN_DEPENDENCY ${tp_def})


  if(WITH_PROBES)
    if(NOT UNIX OR APPLE)
      qi_error("WITH_PROBES is only available on linux")
    endif()
    # Generate ${tp_def_base}.c
    set(_tp_c ${CMAKE_CURRENT_BINARY_DIR}/${_tp_def_base}.c)
    configure_file("${_PROBES_CMAKE_DIR}/tp_probes.in.c" "${_tp_c}")

    # set flag to enable probe in each instrumented files
    append_source_file_property(
        COMPILE_FLAGS
        "-DWITH_PROBES"
        ${_instrumented_files})


    # the tp_def file should included only once with following flags set,
    # because this inclusion will define functions, and we do not want these
    # functions to be defined twice.
    # Thus, we set the flags only for the first instrumented file
    list(GET _instrumented_files 0 _first_instrumented_file)
    append_source_file_property(
        COMPILE_FLAGS
        "-DTRACEPOINT_DEFINE -DTRACEPOINT_PROBE_DYNAMIC_LINKAGE"
        ${_first_instrumented_file})

    # create the probes lib (to be LD_PRELOAD'ed)
    set(_probes_lib ${_tp_def_base})
    qi_create_lib("${_probes_lib}"
      INTERNAL SHARED
      ${_tp_h}
      ${_tp_c}
      SUBFOLDER probes)

    # note: it is not really necessary to link with URCU,
    # LLTNG-UST only needs the urcu/compiler.h header.
    qi_use_lib("${_probes_lib}" LTTNG-UST URCU)
  endif()

endfunction()
