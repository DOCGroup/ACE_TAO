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

static char _rpcpp_service_cc_[]
= "service.cc,v 2.3 1992/06/15 19:12:44 mnl Exp";

// service.cc,v
// Revision 2.3  1992/06/15  19:12:44  mnl
// Fixed a few bugs, clarified interface.
//
// Revision 2.2  1992/06/13  14:27:02  mnl
// Adapted to (patched) gcc-2.2. Fixed several bugs.
//
// Revision 2.1.1.1  1992/03/08  13:28:42  mnl
// Initial mnl version.
//

#ifdef __GNUG__
#pragma implementation
#endif

#include <assert.h>
#include <stream.h>
#include <memory.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <rpc/rpc.h>
#include <rpc/svc.h>
#include "rpc++/service.h"
#include "rpc++/request.h"

extern "C" {
  extern int getdtablesize();
}

//
// RpcRegistered
//

class RpcRegistered
{
public:
  RpcRequest request;
  AnyRpcCallback* callback;

  inline RpcRegistered (const RpcRequest& req, AnyRpcCallback* cb)
    : request (req), callback (cb) { }
  inline ~RpcRegistered ()
 { delete callback; }
};

//
// RpcService
//

RpcService* RpcService::me = 0;

RpcService::RpcService (u_long pnum, u_long vnum)
{
  init ();
  prog = pnum;
  vers = vnum;
  // find out if we have been started by inetd and set transp accoringly.
  sockaddr_in sa;
  int sasz = sizeof (sa);
  // if started by inetd, stdin is a socket
  if (! getsockname (0, (sockaddr*)&sa, &sasz))
    { // yup, inetd is our parent
      xprt = svctcp_create(0, 0, 0);
    }
  else
    { // we are standalone
      (void) pmap_unset(prog, vers);
      xprt = svctcp_create(RPC_ANYSOCK, 0, 0);
    }
  if (xprt == NULL)
    {
      HandleError (cantCreateTCPService);
      return;
    }
  if (!svc_register(xprt, prog, vers, RpcServiceCallback, IPPROTO_TCP))
    {
      HandleError (cantRegisterService);
      return;
    }
}

RpcService::RpcService (u_long vnum)
{
  init ();
  vers = vnum;
  prog = 0x40000000;
  xprt = svctcp_create(RPC_ANYSOCK, 0, 0);
  if (xprt == NULL)
    {
      HandleError (cantCreateTCPService);
      return;
    }
  while (! pmap_set (prog, vers, IPPROTO_TCP, xprt->xp_port))
    prog++;
  if (!svc_register(xprt, prog, vers, RpcServiceCallback, IPPROTO_TCP))
    {
      HandleError (cantRegisterService);
      return;
    }
}

void RpcService::init ()
{
  errorState = noError;
  if (me)
    {
      HandleError (reconstructionAttempt);
      return;
    }
  me = this;
  handlers = 0;
  maxHandlerIndex = -1;
  inbuf = 0;
  inmax = 0;
}

RpcService::~RpcService ()
{
  svc_unregister (prog, vers);
  for (int i = 0; i <= maxHandlerIndex; i++)
    delete handlers[i];
  delete [] handlers;
  delete [] inbuf;
  me = 0;
}

void RpcService::HandleError ()
{
  switch (errorState)
    {
    case reconstructionAttempt:
      cerr << "rpc++: Attempt to construct another instance of RpcService.\n";
      exit (1);
    case cantCreateTCPService:
      cerr << "rpc++: can't create tcp service.\n";
      exit(1);
    case cantRegisterService:
      cerr << form ("rpc++: can't register (%d, %d, tcp).", prog, vers);
      exit(1);
    case cantSendReply:
      cerr << "rpc++: can't reply to RPC call.\n";
      break;
    case invalidResult:
      cerr << "rpc++: registered routine has return NULL pointer.\n";
      abort ();
    case notRegistered:
      cerr << "rpc++: requested RPC routine not registered.\n";
      break;
    case cantGetArgs:
      cerr << "rpc++: can't get procedure arguments.\n";
      break;
    case cantFreeArgs:
      cerr << "rpc++: can't free XDR arguments.\n";
      break;
    }
  errorState = noError;
}

void RpcService::Register (const RpcRequest& req, const AnyRpcCallback& cb)
{
  assert (req.Params () == -1 || cb.Params () == -1
	  || req.Params () == cb.Params ());
  AnyRpcCallback* cbp = cb.CopyToHeap ();
  cbp->SetService (this);
  if (req.RequestNumber () > maxHandlerIndex)
    {
      RpcRegistered** reg = new RpcRegistered*[req.RequestNumber () + 10];
      memcpy (reg, handlers, (maxHandlerIndex + 1) * sizeof (RpcRegistered*));
      memset (&reg[maxHandlerIndex + 1], 0,
	      (req.RequestNumber () + 10 - (maxHandlerIndex + 1))
	      * sizeof (RpcRegistered*));
      delete handlers;
      handlers = reg;
      maxHandlerIndex = req.RequestNumber () + 10 - 1;
    }
  handlers[req.RequestNumber ()] = new RpcRegistered (req, cbp);
}

void RpcService::Dispatch (svc_req* req, SVCXPRT* transp)
{
  xprt = transp;
  if (req->rq_proc == NULLPROC)
    {
      if (! svc_sendreply (xprt, xdr_void, 0))
	{
	  svcerr_systemerr (xprt);
	  HandleError (cantSendReply);
	}
      return;
    }
  RpcRegistered* handler = ((req->rq_proc > maxHandlerIndex)
			    ? 0
			    : handlers[req->rq_proc]);
  if (! handler)
    {
      svcerr_noproc (xprt);
      HandleError (notRegistered);
      return;
    }
  rpcreq = &handler->request;

  int insz = rpcreq->ParamSize ();
  if (insz > inmax) // does in-data fit in available buffer?
    { // if not, increase buffer space
      delete [] inbuf;
      inbuf = new char[inmax = insz];
    }
  void* dataps[rpcreq->Params ()];
  void** dp = dataps;
  *dp = inbuf;
  for (XdrInfo** ip = rpcreq->InInfo(); *ip; ip++, dp++)
    dp[1] = (char*)dp[0] + (*ip)->Size ();

  memset (inbuf, 0, insz);
  XdrSeqInfo xsi = { rpcreq->InInfo (), dataps };
  if (!svc_getargs (xprt, Xdr::XdrParams, &xsi))
    {
      if (rpcreq->Type () == RpcRequest::normal) // errors can be reported
	svcerr_decode (xprt);   // only if the client waits for a result
      HandleError (cantGetArgs);
      return;
    }
  haveReplied = FALSE;
  void* res = handler->callback->Do (dataps);
  if (! haveReplied)
    Reply (res);
  if (!svc_freeargs (xprt, Xdr::XdrParams, &xsi))
    HandleError (cantFreeArgs);
  xprt = 0;
}

void RpcService::Reply (void* res)
{
  haveReplied = TRUE;
  if (rpcreq->Type () == RpcRequest::normal) // i.e., result expected
    {
      xdrproc_t outproc = rpcreq->OutInfo()->Proc ();
      if (outproc == (xdrproc_t)0)
	{
	  cerr << "rpc++: RpcRequest has invalid xdrproc_t (0) in out-Info";
	  abort ();
	}
      if (res == 0 && outproc != (xdrproc_t)xdr_void)
	{
	  svcerr_systemerr (xprt);
	  HandleError ();
	}
      else if (!svc_sendreply
	       (xprt, rpcreq->OutInfo()->Proc (), res))
	{
	  svcerr_systemerr (xprt);
	  HandleError (cantSendReply);
	}
    }
}

void RpcService::Provide ()
{
  int dtbsz = getdtablesize();
  fd_set readfds;

  quitLoop = FALSE;
  while (! quitLoop)
    {
      readfds = svc_fdset;
      switch(select (dtbsz, &readfds, 0, 0, 0))
	{
	case -1:
	  if (errno != EBADF)
	    continue;
	  cerr << "PRC++: select: " << sys_errlist[errno] << '\n';
	  return;
	  break;
	case 0:
	  continue;
	default:
	  svc_getreqset (&readfds);
	  break;
	}
    }
}

char* RpcService::CallerName ()
{
  struct sockaddr_in *sa = Caller ();
  struct hostent* he = gethostbyaddr (&sa->sin_addr, sizeof (sa->sin_addr),
				      sa->sin_family);
  return he->h_name;
}
