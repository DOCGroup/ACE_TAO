# Long request with periodic idle scanning

The server uses Y=4 seconds and X=1 second. The client explicitly disables idle
expiry. The old svc.conf is replaced by server.conf to avoid automatic loading
of server settings by the client.

After a ping, the client processes reactor events for three seconds and verifies
the connection is still cached. It then calls long_request(), which processes
server reactor events for two seconds. The call spans the old idle deadline
while remaining shorter than Y, as required by the supported configuration.

After the reply the client waits three seconds without making another call.
The cache must still contain the connection: synchronous completion must refresh
activity. It then waits up to Y+X+1 seconds for peer closure, checks the empty
cache, reconnects with ping(), and shuts down.

This replaces the old Y=1/two-second-servant scenario, which was outside the
documented maximum-request-duration contract. The scanner can conservatively
retain active input callbacks, but this test does not require overlong calls.

Run `perl run_test.pl`; use `-debug` or `-cdebug` for ORB logging.
