# Long server request and transport idle timeout regression

The server uses:

    -ORBTransportIdleTimeout 1

`long_request()` sleeps for two seconds before returning.

The test succeeds only when the server can keep the transport alive while the
request is actively being processed and send the reply afterwards.

This exposes a weakness in a patch that merely calls `schedule_idle_timer()`
when a request is received: that schedules a new one-second timer, but does
not protect a request whose processing takes longer than one second.

A robust implementation should cancel the server-side idle timer before
processing a request and schedule a new timer after request processing has
completed.
