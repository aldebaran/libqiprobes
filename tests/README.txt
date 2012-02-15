There is currently a single test: hello.

This program does nothing but trace through LTTng.

By default, it is built with WITH_PROBES=OFF, which means
the probes are not included.

This build should work on all platforms, it ensures the probes actuvation
mechanism works well.

When build with WITH_PROBES=ON, provided you are using linux and have the
LTTng tools installed, you can run the following commands to ensure the traces
are properly collected.

  qc -DWITH_PROBES=ON
  qm
  SDK_DIR=~/ar/m/build-linux64/qiprobes/sdk
  # check the tracepoints are built in the exec:
  nm ${SDK_DIR}/bin/hello |grep qi_probes_test_hello
  tests/run_hello.bash ${SDK_DIR}


You should get an output similar to this::

  [10:09:37.429964404] (+?.?????????) qi_probes_test_hello:counting: { 1 }, { counter = 0 }
  [10:09:37.429966859] (+0.000002455) qi_probes_test_hello:counting: { 1 }, { counter = 1 }
  [10:09:37.429967505] (+0.000000646) qi_probes_test_hello:counting: { 1 }, { counter = 2 }
  [10:09:37.429967791] (+0.000000286) qi_probes_test_hello:counting: { 1 }, { counter = 3 }
  ...
