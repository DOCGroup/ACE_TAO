// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    connect.h
//
// = DESCRIPTION
//    Simple threaded asymmetric TCP connection manager.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
// 
// ============================================================================

#if !defined (TAO_CONNMGR_H)
#define	TAO_CONNMGR_H

#include "ace/OS.h"

#if	unix
#	include <sys/time.h>
#elif defined (VXWORKS)
#	include <time.h>
#else	// __WIN32__
#	include <winsock.h>
#endif

#ifdef	DEBUG
#include	<stdio.h>
#endif

template <class T>
class autorelease 
  // = TITLE
  // Utility class that may not really belong here; it's currently used
  // only with connection endpoints though.
  //
  // THREADING NOTE: note that the pointer doesn't change, so two
  // threads could share an "autorelease<T>" value if they collaborated
  // about safe refcounting and destruction.
{ 
public:
  autorelease &operator= (T *ptr)
  {
    if (_state) _state->release ();
    _state = ptr; 
    return *this;
  }

  operator int () { return _state ? 1 : 0; }
  T *operator -> () { return _state; }

  autorelease (void) { _state = 0; }
  ~autorelease (void)
  { if (_state) _state->release (); }

private:
  T *_state;
};

struct client_endpoint 
  // = TITLE
  // Client endpoints are acquired just by looking them up; they're
  // created if needed, and may be closed when they're not in use.
  //
  // NOTE: timeout on lookup/create would be a nice quality-of-service
  // knob to be able to tweak...
{
  // @@ Can you please change <fd> to <handle>?
  ACE_HANDLE fd;

  // @@ Is this API still required now that we've got the client-side
  // connection manager?
  static client_endpoint *lookup (char *host,
				  u_short port,
				  CORBA_Environment &env);
  void release (void);
  // @@ Please add comments.

#ifdef	DEBUG
  static void dump (FILE *);
#endif /* DEBUG */

private:
  // @@ Please add comments.
  char *hostname;
  u_short port;
  u_int refcount;
  client_endpoint *next;

#ifdef	__GNUG__
  //
  // G++ (even 2.6.3) stupidly thinks instances can't be
  // created  This de-warns.
  //
  friend class everyone_needs_a_friend;
#endif
};

// @@ Is this code still used?  If not, can we remove it?

class server_endpoint 
  // = TITLE
  // Server endpoints start out by waiting passively for input on a port.
  // New connections may be created.  Old ones may be disconnected.
  //
  // NOTE:  this version doesn't bind to specific host addresses on
  // multihomed hosts, so it's not yet suitable for use on firewalls.
  //
  // NOTE:  this version also doesn't listen on more than one port at a time.
{
public:
  // @@ Please change <fd> to <handle>.
  ACE_HANDLE fd;

  static server_endpoint *initialize (u_short &port,
				      CORBA_Environment	&env);

  server_endpoint *block_for_connection (CORBA_Boolean eager,
					 timeval *timeout,
					 CORBA_Environment &env);

  // Add a flag saying some, all?
  void shutdown_connections (void (*close_conn)(ACE_HANDLE &fd,
						void *info),
			     void *info);

  void release (void);

#ifdef	DEBUG
  static void dump (FILE *);
#endif

private:
  u_short port;
  CORBA_Boolean is_passive;
  u_int refcount;
  server_endpoint *next;

#if	defined (__GNUG__)
  //
  // G++ (even 2.6.3) stupidly thinks instances can't be
  // created.  This de-warns.
  //
  friend class everyone_needs_a_friend;
#endif
};

#endif /* TAO_CONNMGR_H */
