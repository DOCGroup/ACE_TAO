// @(#)connmgr.cpp	1.4 95/09/29
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// IIOP:	Simple asymmetric TCP connection manager
//
// This has been multithreaded with a very simple strategy, optimizing
// for "lightly threaded" clients rather than maximal sharing of system
// resources (connections, time) in concurrent environments.  Two locks
// are used, one each for client and server sides.
//
// The expectation is:  threads have a refcount on an endpoint only while
// a call's active.  Between calls, they release the endpoint record.  If
// need be, the file descriptor in the record may be set to a negative
// number, and the descriptor closed (e.g. on unrecoverable error).
//
// The tricky issues have been strongly avoided.  Particularly, on any
// given connection no multiplexing is done; that simplifies this code
// substantially, as well as the protocol code that'd otherwise need to
// dispatch IIOP replies to arbitrary client threads.  This costs most if
// several "long" (time-wise) calls are made concurrently.
//
// Similarly, condition variables aren't used to allow concurrent access
// to connection tables during "long" operations:  name service lookups,
// connection establishment, or both.  Initial connection establishment,
// including use of hostname aliases, pays this cost.
//

#include	<assert.h>
#include	<memory.h>
#include	<string.h>

#include <ace/OS.h>

#if	unix
#	include	<netdb.h>
#	include	<unistd.h>

#	include	<sys/types.h>
#	include	<sys/socket.h>
#	include	<sys/time.h>

#	include	<netinet/in.h>

#else	// unix
#	include	<winsock.h>

#endif	// unix

#include	<orb.hh>
#include	<stub.hh>

#include	"connmgr.hh"
#include	"thread.hh"
#include	"debug.hh"


//
// We tell the kernel to queue no more than LISTEN_LIMIT connection
// requests ... traditionally, BSD implementations max out at 5, but
// more recent implementations have no OS limit.
//
#define	LISTEN_LIMIT		5		// traditional maximum

//
// Lists holding the connections managed in this module:  one for outgoing
// connections (client list), the other for incoming ones (server list).
//
// NOTE:  with multiple OAs, it'll be desirable to let each OA have its own
// server endpoint list so OAs can manage requests (and their threading)
// separately.
//
static client_endpoint	*client_list;
static server_endpoint	*server_list;


#ifdef	ACE_HAS_THREADS
//
// If POSIX threads are available, set up locks covering access to
// both client and server side connection lists.  They're separate
// to avoid deadlocking, e.g. self-deadlock when a process calls to
// an object it implements.
//
static pthread_mutex_t		client_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t		server_lock = PTHREAD_MUTEX_INITIALIZER;

//
// We need to cleanly indicate to select()ing server threads that a
// a connection has been returned to their purview.  The simplest way
// is for them to wake up normally ... e.g. by data arriving.  We use
// writes on a private pipe:  that's what signal_fd is for.
//
static ACE_HANDLE			signal_fd;

//
// Conceptually, each TCP OA listens to a single "in" signal FD.  But
// we only support one of them for now.
//
static ACE_HANDLE			signal_in_fd;

#endif	// _POSIX_THREADS


//
// Release ... must be reentrant in threaded systems.
//
void
client_endpoint::release ()
{
#ifdef	ACE_HAS_THREADS
    Critical		region (&client_lock);
#endif	// _POSIX_THREADS

    assert (refcount == 1);
    refcount--;
}


//
// Gets or makes a connection to the indicated host@port, or reports an
// exception explaining why it couldn't.
//
client_endpoint *
client_endpoint::lookup (
    char		*host,
    unsigned short	port,
    CORBA_Environment	&env
)
{
    client_endpoint	*list;
    hostent		*hp = 0;

#ifdef	ACE_HAS_THREADS
    Critical		region (&client_lock);
#endif	// _POSIX_THREADS

    //
    // see if it's already in the list.  if we find it here, we've
    // saved a costly/remote name service lookup.
    //
    // THREADING NOTE:  a policy decision is made here to use a different
    // connection when an existing one is in use.  As with all policies,
    // there are cases where different decisions would be in some sense
    // more optimal.  The rationale is primarily that simpler MT code is
    // preferable; blocking until the connection is idle again can easily
    // deadlock mutually recursive invocations, and performance tradeoffs
    // don't argue universally for multiplexing connections.
    //
    for (list = client_list; list != 0; list = list->next) {
	if (list->port == port) {
	    if (list->fd == ACE_INVALID_HANDLE) {
		dmsg ("client, dead FD in endpoint table");
		continue;
	    }
	    if (strcmp (list->hostname, host) == 0) {
		if (list->refcount == 0) {
		    list->refcount++;
		    return list;
		} else {
		    //
		    // find/make a different connection, this one
		    // is busy for the moment
		    //
		    continue;
		}
	    }

	    // else maybe one's an address, one's a name
	    // or one's a FQDN, one's not fully qualified
	    // ...
	}
    }

    //
    // See if we can find the host's address.  This handles two styles
    // of hostname:  domain names (including partially qualified names,
    // which rely on some implicit location in the DNS hierarchy), and
    // "dotted-decimal" notation (e.g. "192.9.200.1").  Both forms are
    // required by Internet standards (and hence IIOP).
    //
    // THREADING NOTE:  gethostbyname is a "long" call, it'd often be worth
    // dropping the lock during this call.  It'd complicate control flow
    // though, so until heavily threaded clients are common it's left
    // to work in this simple way.
    //
    // XXX note that some platforms, particularly older ones no longer
    // being actively maintained, violate Internet standards and don't
    // accept dotted-decimal hostnames.
    //
    if (hp == 0) {
	while ((hp = ACE_OS::gethostbyname (host)) == 0) {
	    switch (h_errno) {
	      case TRY_AGAIN:		// soft error
	        // sleep (1);
	        continue;

	      case HOST_NOT_FOUND:	// hard NAK (not-exist)
	        dmsg1 ("gethostbyname '%s' --> No such host", host);
	        env.exception (new CORBA_OBJECT_NOT_EXIST (COMPLETED_NO));
		return 0;

	      case NO_RECOVERY:		// hard error
	      case NO_DATA:		// maybe found an MX record?
	      default:			// nonstandard error code
	        dmsg2 ("gethostbyname '%s' --> h_errno %d", host, h_errno);
	        env.exception (new CORBA_COMM_FAILURE (COMPLETED_NO));
		return 0;
	    }
	}
	//
	// Here we found the address associated with the hostname.
	//
	// NOTE: if we save addresses in the connection table, we might
	// rescan it on the grounds that maybe we got a hostname alias
	// (e.g. not the DNS CNAME).  No functionality lost if we don't,
	// but in some cases we'd save a connection.
	//
    }

    //
    // Here we've decided to grow the set of connections to satisfy
    // this request.  We get the record and then fill it out.
    //
    // NOTE: Should first shrink the list if it's very large!  We could
    // track time of last use to support LRU purging of connection cache,
    // with potential removing of duplicates.
    //
    list = new client_endpoint;

    if ((list->fd = ACE_OS::socket (AF_INET, SOCK_STREAM, 0)) == ACE_INVALID_HANDLE) {
	dsockerr ("client socket");
	delete list;

	env.exception (new CORBA_UNKNOWN (COMPLETED_NO));
	return 0;
    }

    //
    // SECURITY NOTE:  Some networks routinely configure bridges based on
    // source and destination port.  So it may be important to bind this
    // socket to some preestablished port before connecting, since without
    // doing so the traffic may not be passed through a firewall or bridge.
    //


    //
    // Connect to the desired server address.
    //
    // THREADING NOTE:  this is again a "long" call, during which it'll be
    // worth dropping the lock on the connection list some day when many
    // client threads contend on that lock.
    // 
    sockaddr_in		addr;

    ACE_OS::memset (&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr =  *(long *)hp->h_addr;
    addr.sin_port = htons (port);

    if (ACE_OS::connect (list->fd, (sockaddr *) &addr, sizeof addr) < 0) {
	dsockerr ("client connect");
	dmsg2 ("... connect failure was to '%s:%d'", host, port);
	delete list;

	env.exception (new CORBA_COMM_FAILURE (COMPLETED_NO));
	return 0;
    }

    list->hostname = ACE_OS::strdup (host);
    list->port = port;
    list->refcount = 1;
    list->next = client_list;

    client_list = list;

    return list;
}


#ifdef	DEBUG

void
client_endpoint::dump (FILE *file)
{
    client_endpoint	*list;

#ifdef	ACE_HAS_THREADS
    //
    // NOTE that although this lock is held for a _very_ long time
    // (terminal/stderr I/O is much slower than network I/O and this
    // does "lots" of it) we don't have much of an option because we
    // need to present a single, consistent view of this table.
    //
    Critical		region (&client_lock);
#endif	// _POSIX_THREADS

    ACE_OS::fprintf (file, "List of client-side connections:\n");

    for (list = client_list; list != 0; list = list->next) {
	ACE_OS::fprintf (file, "    %s @ %d\tfd %d\trefcnt %d\n",
		list->hostname, list->port, list->fd,
		list->refcount);
    }
    ACE_OS::fprintf (file, "\n");
}

#endif



//
// Release ... must be reentrant in threaded systems.
//
// NOTE:  this version actually does two things, which could be split
// into two separate routines with some TBD effect on performance.  It
// decrements the use count of this connection; and it informs other
// potential reading threads that it's OK to read incoming messages.
//
// Splitting these two apart could let the server issue Reply messages
// in arbitrary orders, at the potential cost of putting extra context
// switching into the critical path for request handling.
//
void
server_endpoint::release ()
{
#ifdef	ACE_HAS_THREADS
    Critical		region (&server_lock);
#endif	// _POSIX_THREADS

    assert (refcount == 1);
    refcount--;

#ifdef	ACE_HAS_THREADS
    //
    // Tell whoever's in block_for_input() that they can look again
    // at this connection, reading messages off of it and replying
    // to them as appropriate.
    //
    (void) ACE_OS::write (signal_fd, "b", 1);
#endif	// _POSIX_THREADS
}


//
// Initialize a server endpoint, at the specified port or, if that port
// number is zero, then at any available port.
//
// XXX at some point this will include an interface name, to facilitate
// its use on multihomed hosts such as firewalls.
//
server_endpoint	*
server_endpoint::initialize (
    unsigned short	&port,
    // XXX char *ifname
    CORBA_Environment	&env
)
{
#ifdef	ACE_HAS_THREADS
    Critical		region (&server_lock);
#endif	// _POSIX_THREADS

    //
    // XXX at this time, we only support one port/listener per process.
    // This restriction should be lifted sometime.
    //
    if (server_list != 0) {
	env.exception (new CORBA_INITIALIZE (COMPLETED_NO));
	return 0;
    }

    //
    // Initial "connection" record.
    //
    server_endpoint	*list;

    list = new server_endpoint;
    list->is_passive = CORBA_B_TRUE;
    list->port = port;
    list->next = 0;
    list->refcount = 0;

    //
    // Create the socket
    //
    if ((list->fd = ACE_OS::socket (AF_INET, SOCK_STREAM, 0)) == ACE_INVALID_HANDLE) {
	dsockerr ("server socket");
	delete list;

	env.exception (new CORBA_INITIALIZE (COMPLETED_NO));
	return 0;
    }

    //
    // Bind it to the requested port, if one was requested.
    //
    sockaddr_in	addr;

    if (port != 0) {
#ifdef SO_REUSEADDR
	//
	// In cases where servers abort and must be restarted, we
	// want to avoid TCP's mandatory 4 minute close-wait timeout. 
	// So we set SO_REUSEADDR only on the "listening" socket,
	// which never actually gets a connection; it's safe to be
	// "reusing" the address since the OS never reuses TCP ports
	// which are in the BOUND or LISTEN states.
	//
	// If we can't do this, it's not an error -- this is just an
	// optimization applicable to some failure cases, we can live
	// without it in all cases.  Applications might care; if so,
	// they should run on platforms supporting SO_REUSEADDR.
	//
	int	flag = 1; 

	if (ACE_OS::setsockopt (list->fd, SOL_SOCKET, SO_REUSEADDR,
			(char *) &flag, sizeof (flag)) < 0) {
	    dsockerr ("server setsockopt SO_REUSEADDR");
	}
#endif	// SO_REUSEADDR

	ACE_OS::memset (&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons (port);

	//
	// XXX someday, this is where we'll bind to specific interfaces
	// on multihomed hosts (e.g. firewalls) which do no routing.
	//
	addr.sin_addr.s_addr = htonl (INADDR_ANY);

	if (ACE_OS::bind (list->fd, (sockaddr *)&addr, sizeof addr) < 0) {
	    dsockerr ("server bind");
	    ACE_OS::closesocket (list->fd);
	    delete list;

	    env.exception (new CORBA_INITIALIZE (COMPLETED_NO));
	    return 0;
	}
    }

    //
    // Make it a listening (passive) socket
    //
    if (ACE_OS::listen (list->fd, LISTEN_LIMIT) < 0) {
	dsockerr ("server listen");
	ACE_OS::closesocket (list->fd);
	delete list;

	env.exception (new CORBA_INITIALIZE (COMPLETED_NO));
	return 0;
    }

    //
    // If we bound to a system-assigned port, find out which port
    // address the system assigned us.
    //
    if (port == 0) {
	int		size = sizeof (addr);

	if (ACE_OS::getsockname (list->fd, (sockaddr *) &addr, &size) < 0) {
	    dsockerr ("server getsockname");
	    ACE_OS::closesocket (list->fd);
	    delete list;

	    env.exception (new CORBA_INITIALIZE (COMPLETED_NO));
	    return 0;
	}
	port = list->port = ntohs (addr.sin_port);
    }

#ifdef	ACE_HAS_THREADS
    //
    // We need a clean way to have other threads signal ones that
    // are select()ing that there's another connection they need to
    // pay attention to.  So we set up a pipe for them to use.
    //
    ACE_HANDLE		pipefd [2];

    if (ACE_OS::pipe (pipefd) != 0) {
	dperror ("pipe for connection manager");
	ACE_OS::closesocket (list->fd);
	delete list;

	env.exception (new CORBA_INITIALIZE (COMPLETED_NO));
	return 0;
    }
    signal_in_fd = pipefd [0];
    signal_fd = pipefd [1];
#endif	// _POSIX_THREADS

    server_list = list;

    return list;
}


//
// Get a connection.  Unless "eager" is set, the connection returned
// will actually have data ready for input.  Normally, unthreaded
// environments can't be "eager", and threaded environments prefer to
// use that model to achieve better performance.  Threaded environments
// can of course not be "eager".
//
// THREADING NOTE:  It's undesirable to have more than one thread call this
// at the same time; the semantics of two threads that select() on the same
// file descriptor are undefined.  Hence the static flag that's tested.
//
server_endpoint	*
server_endpoint::block_for_connection (
    CORBA_Boolean	eager,
    timeval		*timeout,
    CORBA_Environment	&env
)
{
#ifdef	ACE_HAS_THREADS
    Critical		region (&server_lock);
#endif	// ACE_HAS_THREADS

    //
    // Head of the list is a passive file descriptor.  The rest is a list
    // of ones used for I/O to clients.  Only call block_for_input() on
    // endpoints returned by initialize().
    //
    assert (is_passive);

    //
    // Scan the list of server-side connections and generate the fd_set
    // we'd use in a select() call (or eagerly return a file descriptor,
    // without selecting first).  Make the call, examine the results;
    // maybe we return soon to the caller, maybe we don't.
    //
    // XXX if there are lots of connections here we should contemplate
    // shutting down several of them in order to gracefully reclaim the
    // OS resources associated with the connections.
    //
    for (;;) {
	fd_set			read_fdset;
	server_endpoint		*list, *previous;
	int			max_fd = 0;

	FD_ZERO (&read_fdset);
	for (list = this, previous = 0;
		list;
		previous = list, list = list->next) {

	    //
	    // Delete records for connections that were closed and
	    // which nobody is using.
	    //
	    if (list->fd < 0) {
		if (list->refcount != 0)
		    continue;

		assert (previous != 0);		// passive must exist!

		previous->next = list->next;
		delete list;
		list = previous;
		continue;
	    }

	    //
	    // If nobody else is reading from this connection, we work with
	    // it ... if the caller is "eager" we return it immediately
	    // (even with no data).  Else we prepare to select on it.
	    //
	    // Refcount is currently used to track if someone's reading,
	    // though it'd be easy to further distinguish "someone reading"
	    // from "someone needs".  A "needed" connection without someone
	    // currently reading could be assigned a thread to read it;
	    // that would enable out-of-order processing (a lock would be
	    // needed to ensure no interleaving of GIOP 1.1 fragments).
	    //
	    if (list->refcount == 0) {
		if (eager && !list->is_passive) {
		    list->refcount++;
		    return list;
		}
#if defined(_WIN32)
#  define FDSET_CAST (SOCKET)
#else
#  define FDSET_CAST
#endif
		FD_SET (FDSET_CAST list->fd, &read_fdset);
		if (list->fd > max_fd)
		    max_fd = FDSET_CAST list->fd;
	    }
	}

	//
	// Select until something interesting happens.
	//
	// THREADING NOTE:  We leave the critical section for the duration
	// of this select() since we'll normally be there a long time, and
	// other threads must grab server_lock while we block.  But we must
	// reenter it later to compare the select() output with the set of
	// legal server side connections.
	//
	// Also, since the semantics of multiple threads calling select()
	// on the same file descriptor(s) is undefined, we prevent that.
	//
	// We add the pipe file descriptor to the list so that when other
	// threads release connections, we can learn about this.
	//
#ifdef	ACE_HAS_THREADS
	static int		doing_select;	// = 0

	if (doing_select) {
	    dmsg ("concurrent block_for_input() calls");
	    env.exception (new CORBA_IMP_LIMIT (COMPLETED_NO));
	    return 0;
	} else
	    doing_select = 1;

	region.leave ();

	FD_SET (signal_in_fd, &read_fdset);
	if (signal_in_fd > max_fd)
	    max_fd = signal_in_fd;
#endif	// ACE_HAS_THREADS

	// This cheap hack needs to be changed later...of course,
	// we won't need it when everything becomes ACE-ified. ;-)
	int value;
	if (timeout)
	  {
	    ACE_Time_Value tv(*timeout);
	    value = ACE_OS::select (max_fd + 1, &read_fdset,
				    (fd_set*)NULL, (fd_set*)NULL, tv);
	  }
	else
	  value = ACE_OS::select (max_fd + 1, &read_fdset,
				  (fd_set*)NULL, (fd_set*)NULL, NULL);
	

#ifdef	ACE_HAS_THREADS
	region.enter ();
	doing_select = 0;
#endif	// ACE_HAS_THREADS

	if (value < 0) {
	    dsockerr ("server select");
	    env.exception (new CORBA_COMM_FAILURE (COMPLETED_NO));
	    return 0;
	} else if (value == 0) {
	    dmsg ("server select timed out");

	    return 0;
	}

	//
	// Check out the set of FDs we found out about in select() above.
	// If accept() is needed, do so and rescan.  Else return an entry
	// from the list.
	//
	// THREADING NOTE:  we read any byte written by another thread
	// to wake us up.  Rare to have more than one such byte!
	//
#ifdef	ACE_HAS_THREADS
	if (FD_ISSET (signal_in_fd, &read_fdset)) {
	    char	b;
	    (void) ACE_OS::read (signal_in_fd, &b, 1);
	    if (debug_level >= 5)
		dmsg ("block_for_input() woken up");
	}
#endif	// ACE_HAS_THREADS

	for (list = this; list; list = list->next) {
	    if (   list->fd == ACE_INVALID_HANDLE
		|| !FD_ISSET (list->fd, &read_fdset))
	        continue;

	    //
	    // If we've got one with incoming data, return it.
	    //
	    if (!list->is_passive) {
		list->refcount++;
		return list;
	    }

	    //
	    // Nyet ... incoming connection.  Accept it.
	    //
	    sockaddr_in	saddr;
	    int		saddr_siz = sizeof saddr;
	    ACE_HANDLE  new_fd;

	    if ((new_fd
		 = ACE_OS::accept (list->fd, (sockaddr *) &saddr, &saddr_siz))
		== ACE_INVALID_HANDLE) {
		dsockerr ("server accept");
		continue;	// what else?
	    }

	    server_endpoint	*new_clnt;

	    new_clnt = new server_endpoint;
	    new_clnt->fd = new_fd;
	    new_clnt->port = saddr.sin_port;
	    new_clnt->is_passive = CORBA_B_FALSE;
	    new_clnt->refcount = 0;

	    dmsg1 ("accepted new FD %d", fd);

	    //
	    // Splice it into list betwen here and next.  Since most
	    // systems can't piggyback data with the connection setup
	    // packet, there's probably no data here yet.  We can't
	    // find out when it arrives without blocking or polling.
	    //
	    new_clnt->next = list->next;
	    list->next = new_clnt;
	    list = new_clnt;

	    //
	    // One ramification of an "eager" model:  we treat the
	    // connection as having data immediately on connection
	    // establishment.  Lacking transactional TCP this will not
	    // often be the case ... but the basic "eager" model is
	    // that we spend LWPs (threads) to improve latencies.  
	    //
	    if (eager) {
		new_clnt->refcount++;
		return new_clnt;
	    }
	}
    }
}

void
server_endpoint::shutdown_connections (
    void 		(*close_conn) (ACE_HANDLE &, void *),
    void		*info
)
{
    server_endpoint	*list, *successor;

#ifdef	ACE_HAS_THREADS
    Critical		region (&server_lock);
#endif	// ACE_HAS_THREADS

    for (list = this; list != 0; list = successor) {
	if (list->is_passive)
	    (void) ACE_OS::closesocket (list->fd);
	else
	    close_conn (list->fd, info);

	successor = list->next;
	delete list;
    }
}


#ifdef	DEBUG

void
server_endpoint::dump (FILE *file)
{
    server_endpoint	*list;

#ifdef	ACE_HAS_THREADS
    //
    // NOTE the comment in client_endpoint::dump() re long lock times.
    //
    Critical		region (&client_lock);
#endif	// ACE_HAS_THREADS

    ACE_OS::fprintf (file, "List of server-side connections:\n");

    for (list = server_list; list != 0; list = list->next) {
	ACE_OS::fprintf (file, "    port %d%s\tfd %d\trefcnt %d\n",
		list->port, list->is_passive ? " (passive)" : "",
		list->fd, list->refcount);
    }
    ACE_OS::fprintf (file, "\n");
}

#endif

