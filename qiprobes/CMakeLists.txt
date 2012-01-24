qi_stage_cmake(qiprobes-config.cmake)
# copy/paste the templates in build/sdk/cmake,
# so that qiprobes-config.cmake can find them:
configure_file(tp_probes.in.h ${QI_SDK_DIR}/cmake/tp_probes.in.h COPYONLY)
configure_file(tp_probes.in.c ${QI_SDK_DIR}/cmake/tp_probes.in.c COPYONLY)
configure_file(tpl.py ${QI_SDK_DIR}/cmake/tpl.py COPYONLY)
