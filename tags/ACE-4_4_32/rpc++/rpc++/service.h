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

#ifndef _RPCSERVICE_H_
#define _RPCSERVICE_H_
static char _rpcpp_service_h_[]
= "service.h,v 2.3 1992/06/15 19:13:30 mnl Exp";

// service.h,v
// Revision 2.3  1992/06/15  19:13:30  mnl
// Fixed a few bugs, clarified interface.
//
// Revision 2.2  1992/06/13  14:27:41  mnl
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
#include "request.h"
#include "callback.h"

class RpcRegistered;

class RpcService
{
public:
  typedef enum
    { noError, reconstructionAttempt, cantCreateTCPService,
      cantRegisterService, notRegistered, cantGetArgs,
      invalidResult, cantSendReply, cantFreeArgs, selectError,
    } errorCode;

  // Get state
  inline virtual bool OK ()
    { return errorState == noError; }
  // Construct a service object for service prog, version vers
  RpcService (u_long prog, u_long vers);
  // Construct a transient service object for version vers
  RpcService (u_long vers);
  // Destruct the service
  virtual ~RpcService ();

  // Get the program number (normally used after construction of transient)
  u_long Program ();
  
  // Register an object and its method to be called on request
  virtual void Register (const RpcRequest&, const AnyRpcCallback&);

  // The link to RPC
  virtual void Dispatch (svc_req* req, SVCXPRT* transp);
  // Provide the service. Never returns.
  void Provide ();

  // Get caller. May be called during execution of a service routine.
  inline struct sockaddr_in* Caller ()
    { return svc_getcaller (xprt); }
  char* CallerName ();
  // Reply before return
  void Reply (void* res);
  void Reply ();
  // Quit provide loop
  void Interrupt ();

private:
  // Save the address of the one and only RpcService in the process.
  // There may be only one RpcService, because we can register a program
  // with svc_register (method RpcServiceCallback) but we can't make
  // the svc function give an argument to this function when doing a
  // callback, which means that we can't have it distinguish between
  // various instances of RpcService.
  static RpcService* me;
  static inline void RpcServiceCallback (svc_req* req, SVCXPRT* transp)
    { RpcService::me->Dispatch (req, transp); }

protected:
  void init ();
  void HandleError (errorCode e);
  errorCode errorState;
  u_long prog;
  u_long vers;
  RpcRegistered** handlers;
  int maxHandlerIndex;
  SVCXPRT* xprt;
  RpcRequest* rpcreq;
  bool quitLoop;
  char* inbuf;
  int inmax;
  bool haveReplied;

  // Default error handling prints a message and exit(2)s.
  virtual void HandleError ();
};

inline void RpcService::HandleError (errorCode e)
{ errorState = e; HandleError (); }

inline u_long RpcService::Program ()
{ return prog; }

inline void RpcService::Reply ()
{ Reply (0); }

inline void RpcService::Interrupt ()
{ quitLoop = TRUE; }

#endif
