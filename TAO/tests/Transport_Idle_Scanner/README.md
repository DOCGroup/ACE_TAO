# Periodic transport idle scanner

This raw-GIOP regression uses a server-only idle timeout Y=2 seconds and a
non-default scan interval X=2 seconds. It checks idle closure, retention of a
partial header and an incomplete fragmented request beyond Y+X, and eventual
closure after completing/cancelling that input. The server destroys its ORB
normally to exercise scanner teardown. Run `perl run_test.pl` after building.

The existing Transport_Idle_Timeout, Transport_Idle_Timeout_server,
Transport_Idle_Timeout_Oneway and Transport_Idle_Timeout_Long_Request tests
provide complementary request/reply and oneway coverage. Expiry assertions
must allow the scan interval and reactor scheduling margin.

The scanner uses one timer per thread-lane cache, a referenced transport
snapshot, and a nonblocking attempt to acquire each transport's output lock.
It never holds the cache lock while acquiring a transport lock or closing a
socket. Input callback admission and the final idle decision share a separate
mutex; active callbacks are conservatively skipped. CSD/AMH deferred completion
is not tracked and is unsupported. Configure Y above maximum synchronous call
duration even though active input callbacks receive additional protection.
