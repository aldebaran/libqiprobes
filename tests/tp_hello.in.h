TRACEPOINT_EVENT(qi_probes_test_hello, counting,
        TP_ARGS(int, counter),
        TP_FIELDS(ctf_integer(int, counter, counter))
)
