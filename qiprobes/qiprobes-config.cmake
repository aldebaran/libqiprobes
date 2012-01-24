get_filename_component(_PROBES_CMAKE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

##
# qi_add_probes(tp_sensorlog.in.h
#  PROVIDER qi_sensorlog
#  INSTRUMENTED_FILES alsensorlog.cpp
# )

function(qi_add_probes tp_def)
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
                     COMMAND "python" ARGS
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

    set_source_files_properties(${_instrumented_files}
      PROPERTIES
        COMPILE_FLAGS "-DWITH_PROBES -DTRACEPOINT_DEFINE"
    )

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
