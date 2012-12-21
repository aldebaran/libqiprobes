#ifdef WITH_PROBES
#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER @_provider@

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "@_tp_h@"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(@_tp_h_reinclusion_protection@) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define @_tp_h_reinclusion_protection@

#include <lttng/tracepoint.h>

@_tp_def_contents@
#endif /* @_tp_h_reinclusion_protection@ */

/* This part must be outside ifdef protection */
#include <lttng/tracepoint-event.h>

#ifdef __cplusplus
}
#endif
#else /* WITH_PROBES */
/* empty macro to ensure build will succeed */
#define tracepoint(provider, name, ...)
#endif /* WITH_PROBES */
