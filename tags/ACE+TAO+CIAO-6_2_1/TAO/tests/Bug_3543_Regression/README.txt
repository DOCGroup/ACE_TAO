$Id$

This test is a reproducer for bug 3543. The master starts and waits until all servers
have registered. Then master starts pinging all the servers with 10 threads. At that
moment multiple threads are waiting for the same connection to a specific server. We
observed at that moment that in TAO_Transport::register_if_necessary the check for
is_registered returns false, then we get a thread switch and another thread then
registers the transport with the reactor, then register_handler returns 1 and this is
then handled as an error.
