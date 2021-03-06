Example
=======

You have a simple ``examplehello`` application which uses ``libexamplesay``.
You build it with::

    qi_create_lib(examplesay
        SRC
        say.h
        say.cpp)
    qi_stage_lib(examplesay)
    qi_create_bin(examplehello
        SRC
        hello.cpp)
    qi_use_lib(examplehello examplesay)

You want to instrument it by adding LTTng tracepoints to both the library and
the executable. To make life easier, you will use the qiprobes helper.

You have two options: having the qiprobes dependency optional or not. In both
cases, the LTTng dependency *is* optional.

This tutorial assumes qiprobes dependency is not optional.

Tracepoints definition
----------------------

First, declare the events you need in two files named ``tp_example_say.in.h``

.. literalinclude:: /../../example/tp_example_say.in.h
   :language: c

and ``tp_example_hello.in.h``

.. literalinclude:: /../../example/tp_example_hello.in.h
   :language: c

All the events in a file should use the same "provider", which are
``qi_probes_example_say`` and ``qi_probes_example_hello`` respectively.
They act has a global namespace for your application events, and should be
unique within the whole traced system.

By convention, all Aldebaran events should start with ``qi_``. Here
it is followed by ``probes_example`` because the traced application and
library are the example from qiprobes.

The rest of the file describes the tracepoint payload, and how it is stored in
the trace. The details about the tracepoints declaration are documented in
`lttng-ust manual page
<http://lttng.org/doc/man-pages/man3/lttng-ust.3.html>`_.

Using the tracepoints
---------------------

Then, include the headers and add the tracepoints in the application files,
here ``examplesay.cpp``

.. literalinclude:: /../../example/say.cpp
   :language: c++

and ``examplehello.cpp``

.. literalinclude:: /../../example/hello.cpp
   :language: c++

Tying it all together
---------------------

Eventually, alter your CMakeLists.txt to add support for the probes:

.. literalinclude:: /../../example/CMakeLists.txt
   :language: cmake

``qiprobes_create_probe`` will:

 * create a qibuild submodule named like the probe, which should be used to
   the instrumented application/library.

 * use ``tp_example_say.in.h`` to create ``tp_example_say.h`` which is
   #included by user code (``say.cpp``) and add it to the submodule.

 * if ``WITH_PROBES`` is true:

    * add libdl to the submodule

    * use ``tp_example_say.in.h`` to create ``examplesayprobe.cpp`` and
      add it to the submodule.

    * use ``tp_example_say.in.h`` to create ``examplesayprobe.c``

    * if ``QIPROBES_PROVIDER_BUILD_MODE`` is "SHARED"

      * build ``examplesayprobe.c`` into ``examplesayprobe.so``,
        which should be LD_PRELOADED (at runtime) for the tracing to work.
        This library is linked with ``liblttng-ust.so``.

    * if ``QIPROBES_PROVIDER_BUILD_MODE`` is "BUILTIN"

      * ``examplesayprobe.c`` is added to the submodule

      * add llttng-ust and urcu as dependencies of the submodule

    * if ``QIPROBES_PROVIDER_BUILD_MODE`` is "STATIC"

      * not supported yet

``qi_instrument_files`` will:

 * check the probe was created

 * check the instumented files exist for real

 * if ``WITH_PROBES`` is true:

    * define the ``WITH_PROBES`` preprocessor macros for the instrumented
      files, so that the ``tracepoint(...)`` macros actually do something.

``qi_create_lib`` will:

 * create the lib as usual, pulling the proper source files and dependencies
   from the submodule.
