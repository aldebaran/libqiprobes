#! /usr/bin/python
"""
Script which runs all the tests binaries in every configuration and sets up
and tears down the LTTng daemons, sessions etc.

It will also display the recorded traces and **silently erase everything in
~/lttng-traces**!

Possible improvements

 * We could convert this test into a unit test by checking the recorded traces
   are conform to expectations, either with a few regexps or with the
   babeltrace python module.

 * We could use the lttng-tools python module to start/stop session etc.

 * We could avoid purging ~/lttng-traces and use a temporary directory instead.

 * We could use per-test session names.

 * We should not hard-code the SDK directory (shame!)

"""

import os
import subprocess
import optparse

traces_dir = os.path.expanduser('~/lttng-traces')
sdk_dir = os.path.expanduser('~/ar/m/build-linux64/qiprobes/sdk')

def call_with_env(args, env=None):
    """Call a subprocess while updating its environment.
    """
    _env = os.environ.copy()
    if env:
        _env.update(env)
    p = subprocess.Popen(args, env=_env)
    p.wait()

def cleanup_traces():
    subprocess.call(['rm', '-rf', traces_dir])

def view_traces():
    subprocess.call(['babeltrace', traces_dir])

def setup_daemons():
    # create a session (it spawns daemons)
    subprocess.call(['lttng', 'create'])
    # enable all userspace events
    subprocess.call(['lttng', 'enable-event', '-u', '-a'])
    # start recording
    subprocess.call(['lttng', 'start'])

def teardown_daemons():
    # stop recording
    subprocess.call(['lttng', 'stop'])
    # destroy the session
    subprocess.call(['lttng', 'destroy'])
    # kill the daemons
    daemons = ['lttng-sessiond', 'lttng-consumerd']
    subprocess.call(['killall'] + daemons)
    # kill them again, for sure
    daemons.reverse()
    subprocess.call(['killall'] + daemons)

def print_separator(msg, test, config):
    line = "==({0}{1}-{2})==".format(msg, test, config)
    print(line + "="*(79-len(line)))

def run_test(test, config):
    assert(config in ['off', 'shared', 'static', 'builtin'])
    # the examplehello test is only built for 'off' or 'shared'
    assert(test != 'examplehello' or config in ['off', 'shared'])

    print_separator("running ", test, config)

    if config != 'off':
        cleanup_traces()
        setup_daemons()

    if config == 'shared':
        # setup the env for LD_PRELOAD the probes
        if test == 'examplehello':
            providers = ['tp_example_hello', 'tp_example_say']
        else:
            providers = ['tp_' + test]
        preload = " ".join(["{0}/lib/probes/lib{1}.so".format(sdk_dir, tp)
                for tp in providers])
        env = {"LD_PRELOAD": preload}
    else:
        env = None
    if test == 'examplehello':
        test_bin = '{0}/bin/{1}'.format(sdk_dir, test)
    else:
        test_bin = '{0}/bin/{1}-{2}'.format(sdk_dir, test, config)
    call_with_env([test_bin], env=env)

    if config != 'off':
        teardown_daemons()
        print_separator("viewing ", test, config)
        view_traces()
    print_separator("done ", test, config)

if __name__ == "__main__":
    run_test('examplehello', 'off')
    #run_test('examplehello', 'shared')
    tests = ('hello', 'subdirhello')
    configs = ('off', 'shared', 'builtin') # todo: add 'static'
    #tests = [tests[1]]
    #configs = [configs[1]]
    for test in tests:
        for config in configs:
            run_test(test, config)
