qi_stage_cmake(qiprobes-config.cmake)

# The qiprobes-config.cmake assumes the files it needs are installed
# next to it. (share/cmake/qiprobes/{qiprobe-config.cmake,tp_probes.in.c ...}
# So this function make sure the data are:
#   * generated next to the cmake file at compile time
#   * installed next to the cmake file at install time
function(qiprobes_stage_cmake_data module_name filename)
  # Depends on impementation of qi_stage_cmake ...
  configure_file(${filename}
    ${QI_SDK_DIR}/${QI_SDK_CMAKE_MODULES}/${filename}
    COPYONLY)
  install(FILES "${filename}"
    COMPONENT devel
    DESTINATION "${QI_SDK_CMAKE}/${module_name}"
  )
endfunction()

qiprobes_stage_cmake_data(qiprobes tp_probes.in.h)
qiprobes_stage_cmake_data(qiprobes probe.in.c)
qiprobes_stage_cmake_data(qiprobes probe.in.cpp)
qiprobes_stage_cmake_data(qiprobes tpl.py)
