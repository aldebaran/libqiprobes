Introduction
============

About qiprobes
--------------

qiprobes is a small cmake module which streamlines the instrumentation
of C++ code with the `LTTng 2.0 <http://lttng.org>`_ tracing tool in the
qibuild framework. It generates files from templates (to save you some typing)
and adds the proper targets to the build system.

qiprobes also ensures everything will work (though without instrumentation) on
platforms which are not supported by LTTng.

About tracing and instrumentation
---------------------------------

Tracing is similar to logging: it consists in recording events that happen in a
system. However, compared to logging, it usually records much lower-level
events that occur much more frequently. Tracers must therefore be optimized to
handle a lot of data while having a small impact on the system. This is
particularly important for systems under real-time constraints, such as...
robots for instance. See http://lttng.org/project or `these slides
<http://lttng.org/files/papers/presentations/lttng20tracingforeveryone.pdf>`_
for a longer introduction.

About LTTng
-----------

LTTng is linux-only tool which brings us traces which are

- thread-safe,
- activable at runtime, on a one-by-one basis, or using loglevels,
- very low overhead: the traces are binary, and go locklessly in RCU buffers
  in shared memory, where they are consumed (or not) by daemon processes,
- unified across the whole system (kernel and user-space processes share the
  same time source)
- stored in a compact binary format (CTF)

CTF stands for Common Trace Format, this format was designed to support being

  - read directly from memory (mmap)
  - overwritten in memory in a ring buffer (in a mode dubbed "flightrecorder"
    in LTTng parlance)
  - flushed to disk
  - streamed over the network

In the following tutorial, we'll simply flush the traces to the filesystem and
view them (on a another computer) either as text (converted with
the babeltrace tool) or in Eclipse (version Juno, using the "tracing"
perspective from the "linux tools").

The LTTng project is documented there http://lttng.org/documentation.

.. note::

  While tracing can only be done on linux, the trace analysis tools are
  cross-platform.

.. note::

  Newer version of LTTng should work on BSD too, and maybe even on mac os X.
  Nobody ever tried though.

.. note::

  You do not need to build you project in Eclipse to analyse the traces in
  Eclipse. Keep using your favorite IDE!

