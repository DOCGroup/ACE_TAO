# Transport idle timeout regression test

This test covers the server-side `-ORBTransportIdleTimeout` behavior.

The client performs:

1. `ping()` — opens the transport.
2. Waits 1.5 seconds.
3. `ping()` again.
4. `shutdown()`.

The server runs with:

    -ORBTransportIdleTimeout 2

Before the fix, the server-side idle timer starts when the transport is
opened and is not restarted when the server receives the first `ping()`.
Consequently the transport can be closed after roughly two seconds from
opening, causing the second `ping()` to fail.

The fix restarts the idle timer when parsed incoming traffic is received,
so the second `ping()` succeeds.

## Build

Generate the projects with MPC in this directory, for example:

    $ACE_ROOT/bin/mwc.pl -type <your-type> Transport_Idle_Timeout_server.mpc

Then build the generated workspace/project.

## Run

    perl run_test.pl

For transport diagnostics:

    perl run_test.pl -debug
