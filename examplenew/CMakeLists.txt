find_package(qiprobes)
qi_create_probe(examplenewsayprobe
  tp_examplenew_say.in.h
  PROVIDER_NAME qi_probes_examplenew_say)
qi_instrument_files(examplenewsayprobe
  say.cpp)
qi_create_lib(examplenewsay NO_INSTALL
  SRC say.h say.cpp
  SUBMODULE examplenewsayprobe)
qi_stage_lib(examplenewsay)

qi_create_probe(examplenewhelloprobe
  tp_examplenew_hello.in.h
  PROVIDER_NAME qi_probes_examplenew_hello)
qi_instrument_files(examplenewhelloprobe
  hello.cpp)
qi_create_bin(examplenewhello NO_INSTALL
  SRC hello.cpp
  SUBMODULE examplenewhelloprobe)
qi_use_lib(examplenewhello examplenewsay)

