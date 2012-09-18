There is currently a single test: hello.

This program does nothing but trace through LTTng.

By default, it is built with WITH_PROBES=OFF, which means
the probes are not included.

This build should work on all platforms, it ensures the probes activation
mechanism works well.

When build with WITH_PROBES=ON, provided you are using linux and have the
LTTng tools installed, you can run the following commands to ensure the traces
are properly collected.

  qc -DWITH_PROBES=ON
  qm
  SDK_DIR=~/ar/m/build-linux64/qiprobes/sdk
  # check the tracepoints are built in the exec:
  nm ${SDK_DIR}/bin/hello |grep qi_probes_tests_hello
  tests/run_hello.bash ${SDK_DIR}


You should get an output similar to this::

  [21:07:55.301283867] (+?.?????????) sbarthelemy-de:hello:26708 qi_probes_tests_hello:counting: { cpu_id = 0 }, { counter = 0 }
  [21:07:55.301286607] (+0.000002740) sbarthelemy-de:hello:26708 qi_probes_tests_hello:saying: { cpu_id = 0 }, { message = 0 }
  [21:07:55.301322043] (+0.000035436) sbarthelemy-de:hello:26708 qi_probes_tests_hello:counting: { cpu_id = 0 }, { counter = 1 }
  [21:07:55.301322570] (+0.000000527) sbarthelemy-de:hello:26708 qi_probes_tests_hello:saying: { cpu_id = 0 }, { message = 1 }
  [21:07:55.301325487] (+0.000002917) sbarthelemy-de:hello:26708 qi_probes_tests_hello:counting: { cpu_id = 0 }, { counter = 2 }
  [21:07:55.301325829] (+0.000000342) sbarthelemy-de:hello:26708 qi_probes_tests_hello:saying: { cpu_id = 0 }, { message = 2 }
  ...
