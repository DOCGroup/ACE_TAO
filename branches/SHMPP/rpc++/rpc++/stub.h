// -*- c++ -*-
/* 
Copyright (C) 1991 Peter Bersen

This file is part of the rpc++ Library.  This library is free
software; you can redistribute it and/or modify it under the terms of
the GNU Library General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your
option) any later version.  This library is distributed in the hope
that it will be useful, but WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU Library General Public License for more details.
You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

Modified and partially rewritten March 1992 by Michael N. Lipp,
mnl@dtro.e-technik.th-darmstadt.de. The original copyright terms and
conditions apply without change to any modified or new parts.
*/

#ifndef _RPCSTUB_H_
#define _RPCSTUB_H_
static char _rpcpp_stub_h_[]
= "stub.h,v 2.3 1992/06/15 19:13:31 mnl Exp";

// stub.h,v
// Revision 2.3  1992/06/15  19:13:31  mnl
// Fixed a few bugs, clarified interface.
//
// Revision 2.2  1992/06/13  14:27:43  mnl
// Adapted to (patched) gcc-2.2. Fixed several bugs.
//
// Revision 2.1.1.1  1992/03/08  13:28:43  mnl
// Initial mnl version.
//

#ifdef __GNUG__
#pragma interface
#endif

#undef TRUE
#undef FALSE
#include <bool.h>
#include <String.h>
#include <sys/time.h>
#include "request.h"

class RpcStub 
{
protected:
  static timeval defaultTimeout;
  static void* res;
  static size_t resmax;
  static xdrproc_t resproc;
  void init (u_long prognum, u_long versnum,
	     char* hostname, timeval timeout, bool connect);

public:
  typedef enum
    { noError, notConnected, cantCreate, cantCall,
    } errorCode;

  // Construct a new stub
  RpcStub (u_long prognum, u_long versnum,
	   char* hostname = "localhost",
	   timeval timeout = defaultTimeout, bool connect = TRUE);
  RpcStub (u_long prognum, u_long versnum,
	   char* hostname = "localhost",
	   bool connect = TRUE, timeval timeout = defaultTimeout);
  virtual ~RpcStub ();

  // Reconnect (in case of failure or delayed connection)
  void Reconnect ();

  // Various inquiries
  virtual bool OK ();
  CLIENT* Service ();

  // Get/set timeout
  timeval GetTimeout () const;
  void SetTimeout (timeval& timo);

  // Make a call, either with or without an argument. If handle_errors
  // is true, "Call" will call the error handler in case of an error.
  // Else, it returns 0 as result and it is up to the client to handle
  // the error.
  // Call with one arg:
  void* Call (RpcRequest&, bool handle_errors = TRUE);
  // Call with two args:
  void* Call (RpcRequest&, void* in, bool handle_errors = TRUE);
  // ...
  void* Call (RpcRequest& req, void*, void*, bool handle_errors = TRUE);
  void* Call (RpcRequest& req, void*, void*, void*, bool handle_errors = TRUE);
  void* Call (RpcRequest& req, void*, void*, void*, void*,
	      bool handle_errors = TRUE);
  void* Call (RpcRequest& req, void*, void*, void*, void*, void*,
	      bool handle_errors = TRUE);
  void* Call (RpcRequest& req, void*, void*, void*, void*, void*, void*,
	      bool handle_errors = TRUE);
  void* Call (RpcRequest& req, void*, void*, void*, void*, void*, void*, void*,
	      bool handle_errors = TRUE);
  // Call with N args:
  void* Call (RpcRequest& req, void**, bool handle_errors = TRUE);

protected:
  void* HandleError (errorCode e);
  errorCode errorState;
  u_long program;
  u_long version;
  String server;
  timeval timeout;
  CLIENT* svc;
  // Default error handling prints a message and exit(2)s.
  virtual void* HandleError ();
  void* DoCall (RpcRequest& req, void** args, bool handle_errors);
};

inline RpcStub::RpcStub (u_long prognum, u_long versnum,
			 char* hostname, timeval timeout, bool connect)
{ init (prognum, versnum, hostname, timeout, connect); }

inline RpcStub::RpcStub (u_long prognum, u_long versnum,
			 char* hostname, bool connect, timeval timeout)
{ init (prognum, versnum, hostname, timeout, connect); }

inline virtual bool RpcStub::OK ()
{ return errorState == noError; }

inline CLIENT* RpcStub::Service ()
{ return svc; }

inline timeval RpcStub::GetTimeout () const
{ return timeout; }

inline void RpcStub::SetTimeout (timeval& timo)
{ timeout = timo; }

inline void* RpcStub::Call (RpcRequest& req, bool handle_errors = TRUE)
{ return Call (req, (void*)0, handle_errors); }

inline void* RpcStub::HandleError (errorCode e)
{ errorState = e; return HandleError (); }

#endif
