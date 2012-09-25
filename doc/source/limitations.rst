Limitations
===========

Build dependency
----------------

When you change the tracepoint definition file (``tp_hal.in.h`` in the example)
you need to run ``qibuild configure`` again. ``qibuild make`` would not notice
the change by itself.

