# Oneway completion and periodic idle scanning

Only the server enables expiry: Y=1 second, X=2 seconds. The client explicitly
disables expiry in client.conf.

The client sends one oneway ping. The servant checks that one transport is
cached and records receipt. The server waits up to 30 seconds for that receipt,
then runs its reactor for Y+X+1 seconds after dispatch has returned and checks
that the cache is empty. No received request is a failure, not an empty-cache pass.

The client keeps its reactor running for six seconds after sending, longer than
the server's four-second observation window, and expects to observe peer closure.
Both cache assertions must pass: client closure alone could otherwise be caused
by server teardown. The observation window assumes timely delivery of the small
local test request; heavily delayed delivery can fail the timing budget.

Completion refreshes the monotonic activity timestamp despite there being no
reply. No per-transport timer is cancelled or restarted.

Run `perl run_test.pl`; use `-debug` or `-cdebug` for ORB logging.
