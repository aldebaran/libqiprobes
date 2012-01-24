#ifdef WITH_PROBES
#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER @_provider@

#if !defined(_TRACEPOINT_@_tp_h_reinclusion_protection@) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _TRACEPOINT_@_tp_h_reinclusion_protection@

#ifdef __cplusplus
extern "C" {
#endif


#include <lttng/tracepoint.h>

@_tp_def_contents@

#endif /* _TRACEPOINT_@_tp_h_reinclusion_protection@ */

#undef TRACEPOINT_INCLUDE_FILE
#define TRACEPOINT_INCLUDE_FILE @_tp_h@

/* This part must be outside ifdef protection */
#include <lttng/tracepoint-event.h>

#ifdef __cplusplus
}
#endif
#else /* WITH_PROBES */
/* empty macro to ensure build will succeed */
#define tracepoint(provider, name, ...)
#endif /* WITH_PROBES */
