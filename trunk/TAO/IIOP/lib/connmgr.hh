// @(#)connmgr.hh	1.2 95/09/24
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Simple threaded asymmetric TCP connection manager.
//

#ifndef	_CONNMGR_HH
#define	_CONNMGR_HH

#include <ace/OS.h>

#if	unix
#	include <sys/time.h>
#else	// __WIN32__
#	include <winsock.h>
#endif

#ifdef	DEBUG
#include	<stdio.h>
#endif

//
// Utility class that may not really belong here; it's currently used only
// with connection endpoints though.
//
// THREADING NOTE:  note that the pointer doesn't change, so two threads
// could share an "autorelease<T>" value if they collaborated about safe
// refcounting and destruction.
//
template <class T>
class autorelease {
  public:
    autorelease 	&operator = (T *ptr)
				{
				    if (_state) _state->release();
				    _state = ptr; return *this;
				}
			operator int () { return _state ? 1 : 0; }
    T			*operator -> () { return _state; }
			autorelease () { _state = 0; }
			~autorelease ()
				{ if (_state) _state->release (); }

  private:
    T			*_state;
};


//
// Client endpoints are acquired just by looking them up; they're created
// if needed, and may be closed when they're not in use.
//
// NOTE: timeout on lookup/create would be a nice quality-of-service
// knob to be able to tweak...
//
struct client_endpoint {
    ACE_HANDLE			fd;

    static client_endpoint	*lookup (
				    char		*host,
				    unsigned short	port,
				    CORBA_Environment	&env
				);

    void			release ();

#ifdef	DEBUG
    static void			dump (FILE *);
#endif

  private:
    char			*hostname;
    unsigned short		port;
    unsigned			refcount;
    client_endpoint		*next;

#ifdef	__GNUG__
    //
    // G++ (even 2.6.3) stupidly thinks instances can't be
    // created  This de-warns.
    //
    friend class everyone_needs_a_friend;
#endif
};


//
// Server endpoints start out by waiting passively for input on a port.
// New connections may be created.  Old ones may be disconnected.
//
// NOTE:  this version doesn't bind to specific host addresses on
// multihomed hosts, so it's not yet suitable for use on firewalls.
//
// NOTE:  this version also doesn't listen on more than one port at a time.
//
class server_endpoint {
  public:
    ACE_HANDLE			fd;

    static server_endpoint	*initialize (
				    unsigned short	&port,
				    // XXX char *ifname,
				    CORBA_Environment	&env
				);

    server_endpoint		*block_for_connection (
				    CORBA_Boolean	eager,
				    timeval		*timeout,
				    CORBA_Environment	&env
				);

    void			shutdown_connections (
				    // add a flag saying some, all?
				    void		(*close_conn)(
							    ACE_HANDLE &fd,
							    void *info
							),
				    void		*info
				);

    void			release ();

#ifdef	DEBUG
    static void			dump (FILE *);
#endif

  private:
    unsigned short		port;
    CORBA_Boolean		is_passive;
    unsigned			refcount;
    server_endpoint		*next;

#if	defined (__GNUG__)
    //
    // G++ (even 2.6.3) stupidly thinks instances can't be
    // created.  This de-warns.
    //
    friend class everyone_needs_a_friend;
#endif
};

#endif	// _CONNMGR_HH
