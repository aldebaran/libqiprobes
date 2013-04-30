find_package(qiprobes)
qiprobes_create_probe(examplesayprobe
  tp_example_say.in.h
  PROVIDER_NAME qi_probes_example_say)
qiprobes_instrument_files(examplesayprobe
  say.cpp)
qi_create_lib(examplesay NO_INSTALL
  SRC say.h say.cpp
  SUBMODULE examplesayprobe)
qi_stage_lib(examplesay)

qiprobes_create_probe(examplehelloprobe
  tp_example_hello.in.h
  PROVIDER_NAME qi_probes_example_hello)
qiprobes_instrument_files(examplehelloprobe
  hello.cpp)
qi_create_bin(examplehello NO_INSTALL
  SRC hello.cpp
  SUBMODULE examplehelloprobe)
qi_use_lib(examplehello examplesay)

