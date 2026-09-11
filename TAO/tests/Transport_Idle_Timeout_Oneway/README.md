# Transport Idle Timeout Oneway Test

This test exercises `-ORBTransportIdleTimeout` with a oneway request.

The timeout is configured to 1 second on the server only. The client does not configure an idle timeout.

Test sequence:

1. The client sends a single `oneway ping()` request.
2. The servant verifies that the server transport cache contains one transport while handling the request.
3. The server keeps processing for 2 seconds. At that point the 1-second idle timeout has expired, while the client is still running. With the receive-cancels-idle-timer behavior under test, the oneway request cancels the server-side idle timer and no reply is sent to restart it, so the server transport cache is expected to still contain one transport.
4. The client continues running its ORB for 3 seconds in total. This keeps the client connection alive while the server performs its 2-second cache check. Afterwards the client expects its transport cache to be empty because the server process has exited and closed the connection.

Keeping the client alive longer than the server-side cache check ensures that the server cannot report an empty cache simply because the client exited and closed the connection.

Expected final cache sizes:

- server after 2 seconds: 1
- client after 3 seconds: 0

Run the test with:

    perl run_test.pl

Use `-debug` or `-cdebug` to enable server or client ORB debug logging respectively.
