# Periodic transport idle timeout regression

Enabled scenarios configure both server and client with idle age Y=3 seconds
and scan interval X=2 seconds. The launcher passes those values as -t and -x
to the clients. Expiry waits allow Y+X+1 seconds, not an exact per-transport
timer deadline. Reactor waits use a monotonic clock.

The single-server client covers idle closure, reconnection, and spaced reuse
across scans, followed by retention before Y and eventual closure. A separate
run disables expiry in both processes while still configuring X=2.

The multiple-server client also covers a nested CORBA call. Its two one-second
waits remain below Y; both transports in the intermediate server must remain
cached during that operation. It then checks eventual idle closure.

These tests intentionally cover client-side expiry as well. The sibling
Transport_Idle_Timeout_server, Transport_Idle_Timeout_Long_Request and
Transport_Idle_Timeout_Oneway directories isolate server-side expiry.

Generate projects using MPC, build, and run `perl run_test.pl`.
Use `-debug` or `-cdebug` for ORB logging.
