TRACEPOINT_EVENT(qi_probes_examplenew_say, saying,
        TP_ARGS(int, message),
        TP_FIELDS(ctf_integer(int, message, message))
)
TRACEPOINT_EVENT(qi_probes_examplenew_say, done_saying,
        TP_ARGS(),
        TP_FIELDS()
)
