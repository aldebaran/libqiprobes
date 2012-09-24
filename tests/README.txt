qiprobes tests
==============

There are currently two tests: hello and subdirhello.

hello
=====

This program does nothing but trace through LTTng.

By default, it is built with WITH_PROBES=OFF, which means
the probes are not included.

This build should work on all platforms, it ensures the probes activation
mechanism works well.

When build with WITH_PROBES=ON, provided you are using linux and have the
LTTng tools installed, you can run the following commands to ensure the traces
are properly collected.::

  qc -DWITH_PROBES=ON
  qm
  # check the tracepoints are built in the exec:
  nm ${SDK_DIR}/bin/hello-shared | grep qi_probes_tests_hello
  nm ${SDK_DIR}/bin/hello-builtin | grep qi_probes_tests_hello
  # edit runtests.py to update the sdk_dir.
  python runtests.py


You should get an output similar to this::

  [21:07:55.301283867] (+?.?????????) sbarthelemy-de:hello:26708 qi_probes_tests_hello:counting: { cpu_id = 0 }, { counter = 0 }
  [21:07:55.301286607] (+0.000002740) sbarthelemy-de:hello:26708 qi_probes_tests_hello:saying: { cpu_id = 0 }, { message = 0 }
  [21:07:55.301322043] (+0.000035436) sbarthelemy-de:hello:26708 qi_probes_tests_hello:counting: { cpu_id = 0 }, { counter = 1 }
  [21:07:55.301322570] (+0.000000527) sbarthelemy-de:hello:26708 qi_probes_tests_hello:saying: { cpu_id = 0 }, { message = 1 }
  [21:07:55.301325487] (+0.000002917) sbarthelemy-de:hello:26708 qi_probes_tests_hello:counting: { cpu_id = 0 }, { counter = 2 }
  [21:07:55.301325829] (+0.000000342) sbarthelemy-de:hello:26708 qi_probes_tests_hello:saying: { cpu_id = 0 }, { message = 2 }
  ...

subdirhello
===========

This is the same program as hello, but with a different source layout, to
ensure the LTTng preprocessor magic and the qiprobes cmake sorcery are
robust to directory changes.
