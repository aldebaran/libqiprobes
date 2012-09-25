find_package(qiprobes)

qi_add_probes(tp_example_say.in.h
    PROVIDER qi_probes_example_say
    INSTRUMENTED_FILES say.cpp)
qi_create_lib(examplesay NO_INSTALL
    SRC
    say.h
    say.cpp
    tp_example_say.h) # this header is generated
if (WITH_PROBES)
  qi_use_lib(examplesay DL)
endif()
qi_stage_lib(examplesay)

qi_add_probes(tp_example_hello.in.h
    PROVIDER qi_probes_example_hello
    INSTRUMENTED_FILES hello.cpp)

qi_create_bin(examplehello NO_INSTALL
    SRC
    hello.cpp
    tp_example_hello.h) # this header is generated
qi_use_lib(examplehello examplesay)
if (WITH_PROBES)
  qi_use_lib(examplehello DL)
endif()
