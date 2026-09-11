# Transport Idle Timeout Oneway Test

This test exercises `-ORBTransportIdleTimeout` with a oneway request.

The timeout is configured to 1 second on the server only. The client does not configure an idle timeout.

Test sequence:

1. The client sends a single `oneway ping()` request.
2. The servant verifies that the server transport cache contains one transport while handling the request.
3. The client continues running its ORB for 2 seconds so it can process the connection close initiated by the server. Afterwards, the client transport cache must be empty.
4. The server continues running its ORB for 3 seconds. With the receive-cancels-idle-timer behavior under test, the received oneway request cancels the server-side idle timer and no reply is sent to restart it. The server transport cache is therefore expected to still contain one transport.

The different expected client and server cache sizes demonstrate the oneway case directly without making a second remote invocation that could reopen a connection and affect the result.

Expected final cache sizes:

- client: 0
- server: 1

Run the test with:

    perl run_test.pl

Use `-debug` or `-cdebug` to enable server or client ORB debug logging respectively.
