# Transport Idle Timeout Oneway Test

This test exercises `-ORBTransportIdleTimeout` with a oneway request.

The timeout is configured to 1 second on the server only. The client does not configure an idle timeout.

Test sequence:

1. The client sends a single `oneway ping()` request.
2. The servant verifies that the server transport cache contains one transport while handling the request.
3. The server keeps processing for 3 seconds, allowing the 1-second idle timeout, the default 1-second scan interval, and scheduling margin. Completion of the oneway updates the server transport's monotonic activity timestamp even though no reply is sent. The server transport cache must be empty at the check.
4. The client continues running its ORB for 4 seconds, remaining alive during the server's check. Afterwards it expects an empty cache after observing server-side closure.

Keeping the client alive longer than the server-side cache check ensures that the server cannot report an empty cache simply because the client exited and closed the connection.

Expected final cache sizes:

- server after 3 seconds: 0
- client after 4 seconds: 0

This test requires idle-timer rescheduling after oneway dispatch completion. With receive-cancels-only behavior, the server cache remains at 1 and the test must fail. The servant's in-request cache expectation remains 1.

Run the test with:

    perl run_test.pl

Use `-debug` or `-cdebug` to enable server or client ORB debug logging respectively.
