# Long request with periodic idle scanning

The server uses Y=1 second and X=1 second. The client explicitly disables idle
expiry. The old svc.conf is replaced by server.conf to avoid automatic loading
of server settings by the client.

The client invokes a synchronous request that runs for three seconds while the
servant processes reactor events. Multiple idle scans therefore fire while the
request is being dispatched. The request must complete successfully because an
active synchronous dispatch is not eligible for idle purging.

Completion refreshes transport activity before the active-request count returns
to zero. The client verifies that the connection is still cached immediately
after the reply, then waits up to Y+X+1 seconds for peer closure, reconnects with
ping(), and shuts down.

Active-request tracking takes the transport cache lock only when transport idle
expiry is enabled. With the default Y=0 configuration, request dispatch performs
no active-request bookkeeping and acquires no additional cache lock.

Run `perl run_test.pl`; use `-debug` or `-cdebug` for ORB logging.
