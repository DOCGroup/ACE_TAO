# Periodic transport idle scanner

This raw-GIOP regression uses a server-only idle timeout Y=2 seconds and
explicitly overrides the 30-second default with scan interval X=2 seconds. It
checks idle closure, retention of a partial header and an incomplete fragmented
request beyond Y+X, and eventual closure after completing/cancelling that
input. The server destroys its ORB
normally to exercise scanner teardown. Run `perl run_test.pl` after building.

The existing Transport_Idle_Timeout, Transport_Idle_Timeout_server,
Transport_Idle_Timeout_Oneway and Transport_Idle_Timeout_Long_Request tests
provide complementary request/reply and oneway coverage. Expiry assertions
must allow the scan interval and reactor scheduling margin.

Each thread-lane cache owns one scanner timer, starts it when the cache is
constructed with idle expiry enabled, and cancels it when the cache closes. A
scan checks entries under the cache lock and retains transports selected for
purging. It releases the cache lock before closing a socket. Activity timestamp
updates use a relaxed atomic monotonic-millisecond value.
Incoming-state checks are not synchronized with receive processing, and active
input callbacks do not receive additional protection. Configure Y above the
maximum request duration, including blocking, queuing and scheduling delays.

CSD (including Dynamic TP), AMH and other deferred dispatch configurations are
allowed, but their deferred completion is not tracked. A transport can therefore
be closed while work is queued or executing or an asynchronous reply is pending.
Short operations with bounded delays may work when Y exceeds the full delay
through completion; the scanner does not guarantee this.
