#!/usr/bin/python
"""
Simple templating engine designed to mimic the configure_file command from
CMake.

Call it with -h to get some usage info.

"""

import sys
import re
import optparse
import warnings

epilog = """

For instance, given the two files example.in.h and tpdef.in.h, with the
following content:

example.in.h:

  #if !defined(@reincl_protection@)
  #define @reincl_protection@
  #include <lttng/tracepoint.h>
  @contents@
  #endif /* @reincl_protection@ */

tpdef.in.h:

  TRACEPOINT_EVENT(qi_dcm, state,
          TP_ARGS(char, state),
          TP_FIELDS(ctf_integer(char, state, state))
  )

the following command

  ./tpl.py -d reincl_protection EXAMPLE_H -i contents tpdef.in.h example.in.h

would print

  #if !defined(EXAMPLE_H)
  #define EXAMPLE_H
  #include <lttng/tracepoint.h>
  TRACEPOINT_EVENT(qi_dcm, state,
          TP_ARGS(char, state),
          TP_FIELDS(ctf_integer(char, state, state))
  )

  #endif /* EXAMPLE_H */

"""

class IndentedHelpFormatterRawEpilog(optparse.IndentedHelpFormatter):

    # override format_epilog to _not_ wrap the epilog
    def format_epilog(self, epilog):
        return epilog

class Substitutor(object):

    def __init__(self):
        self._map = {}

    def _check(self, name):
        if name in self._map:
            warnings.warn("Duplicate entry for {0}".format(name))

    def add_definition(self, name, value):
        self._check(name)
        self._map[name] = value

    def add_file(self, name, filename):
        self._check(name)
        self._map[name] = open(filename, 'r').read()

    def __call__(self, match):
        try:
            return self._map[match.group(1)]
        except KeyError:
            # we have no replacement, send back the original string
            return match.group(0)


if __name__ == "__main__":

    parser = optparse.OptionParser(
            formatter=IndentedHelpFormatterRawEpilog(),
            usage='%prog [options] template',
            description='Simple templating engine designed to mimic the '
            'configure_file command from CMake.',
            epilog=epilog)
    parser.add_option('-d', nargs=2, metavar=('NAME', 'VALUE'),
            action='append',
            help='Add a variable definition: @NAME@ will be substitued '
            'by VALUE in the template.')
    parser.add_option('-i', nargs=2, metavar=('NAME', 'FILE'),
            action='append',
            help='Add a variable definition: @NAME@ will be substitued '
            'by the content of the FILE in the template.')
    parser.add_option('-o', metavar='OUTPUT',
            default=None,
            help='Output file, defaults to stdout.')
    options, args = parser.parse_args()
    if len(args) != 1:
        parser.print_help()
        exit()
    else:
        template = args[0]
    repl = Substitutor()

    if options.d:
        for (name, value) in options.d:
            repl.add_definition(name, value)
    if options.i:
        for (name, filename) in options.i:
            repl.add_file(name, filename)
    with open(args[0], 'r') as template:
        if options.o:
            output = open(options.o, 'w')
        else:
            output = sys.stdout
        with output as f:
            _re = re.compile(r"\@([^\s]+)\@")
            f.write(_re.sub(repl, template.read()))
