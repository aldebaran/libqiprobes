qiprobes
========

Helper functions for instrumenting stuff using LTTng-UST.

Example
=======

You have a simple hello application that you build in the following way::

    qi_create_bin("hello"
        SRC
        hello.cpp
        hello_say.cpp)

You want to instrument it by adding LTTng tracepoints. To make life easier, you
will use the qiprobes helper.

You have two options: having the qiprobes dependency optional or not. In both
cases, the LTTng dependancy *is* optionnal, which is good since it only works
on linux.

This tutorial assumes you don't care keeping qiprobes optional.

First, declare the events you need in a file named ``tp_hello.in.h``.
All the events from this file should use the same "provider", here
``qi_probes_test_hello`` which acts has a global namespace for your
application events. All Aldebaran events should start with ``qi_``. Here
it is followed by ``probes_test_hello`` because the traced application is
the "hello" test from qiprobes.

Then, add the tracepoints in the application files.

Eventually, alter your CMakeLists.txt to add support for the probes::

    find_package(qiprobes)

    qi_add_probes(tp_hello.in.h
        PROVIDER qi_probes_tests_hello
        INSTRUMENTED_FILES hello.cpp hello_say.cpp)
    qi_create_bin("hello" NO_INSTALL
        SRC
        hello.cpp
        hello_say.cpp
        ${tp_hello_SOURCES})
    qi_use_probes(hello tp_hello)

``qi_add_probes`` will:

 * create ``tp_hello.h`` and ``tp_hello.c`` from ``tp_hello.in.h``
 * define some preprocessor macros for the instrumented files
 * fill up the variable ``tp_hello_SOURCES``
 * create the tp_hello target when appropriate

``qi_use_probes`` will link the application with the proper libraries, which
vary according to the configuration: libdl, liblttng-ust, liburcu,
libtp_hello.a.
