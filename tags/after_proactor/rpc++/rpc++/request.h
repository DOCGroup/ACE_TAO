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

#ifndef _RPCREQUEST_H_
#define _RPCREQUEST_H_
static char _rpcpp_request_h_[]
= "request.h,v 2.3 1992/06/15 19:13:28 mnl Exp";

// request.h,v
// Revision 2.3  1992/06/15  19:13:28  mnl
// Fixed a few bugs, clarified interface.
//
// Revision 2.2  1992/06/13  14:27:39  mnl
// Adapted to (patched) gcc-2.2. Fixed several bugs.
//
// Revision 2.1.1.1  1992/03/08  13:28:43  mnl
// Initial mnl version.
//

#ifdef __GNUG__
#pragma interface
#endif

#include "xdr++.h"

// RpcRequest is a class that specifies an individual request that is
// part of a service. Three parameters are required to specify a request:
//  - the request number
//  - the serializer (XdrInfo) for the input to the request
//  - the serializer (XdrInfo) for the output from the request
class RpcRequest
{
private:
  void init (u_long req, int pars, int parsz,
	     const XdrInfo* out, const XdrInfo** in, int rt);

public:
  // Construct a new request from a request id, the information about
  // the input data and the information about the output data.
  // Note that requests that are registered for a service are stored
  // in an array using the request id as the index, so keep indices
  // small.
  typedef enum { normal, batched, async } ReqType;
  // No input arg:
  RpcRequest (u_long req, const XdrInfo* out, int t = normal);
  // One input arg:
  RpcRequest (u_long req, const XdrInfo* out, const XdrInfo* in,
	      int t = normal);
  // Two input args:
  RpcRequest (u_long req, const XdrInfo* out, const XdrInfo*, const XdrInfo*,
	      int t = normal);
  // ...
  RpcRequest (u_long req, const XdrInfo* out, const XdrInfo*, const XdrInfo*,
	      const XdrInfo*, int t = normal);
  RpcRequest (u_long req, const XdrInfo* out, const XdrInfo*, const XdrInfo*,
	      const XdrInfo*, const XdrInfo*, int t = normal);
  RpcRequest (u_long req, const XdrInfo* out, const XdrInfo*, const XdrInfo*,
	      const XdrInfo*, const XdrInfo*, const XdrInfo*, int t = normal);
  RpcRequest (u_long req, const XdrInfo* out, const XdrInfo*, const XdrInfo*,
	      const XdrInfo*, const XdrInfo*, const XdrInfo*, const XdrInfo*,
	      int t = normal);
  RpcRequest (u_long req, const XdrInfo* out, const XdrInfo*, const XdrInfo*,
	      const XdrInfo*, const XdrInfo*, const XdrInfo*, const XdrInfo*,
	      const XdrInfo*, int t = normal);
  // N input args, conversion routines given as a NULL terminated array
  // of XdrInfo*:
  RpcRequest (u_long req, const XdrInfo* out, const XdrInfo**, int t = normal);
  ~RpcRequest ();
  int RequestNumber () const;
  const XdrInfo** InInfo ();
  const XdrInfo* OutInfo ();
  ReqType Type ();
  int Params () const;
  int ParamSize ();

protected:
  int params;
  int parmsz;
  u_long reqnum;
  const XdrInfo** ininfo;
  const XdrInfo* outinfo;
  ReqType reqtype;
};

inline RpcRequest::~RpcRequest ()
{ delete [] ininfo; }

inline int RpcRequest::Params () const
{ return params; }

inline int RpcRequest::RequestNumber () const
{ return reqnum; }

inline const XdrInfo** RpcRequest::InInfo ()
{ return ininfo; }

inline const XdrInfo* RpcRequest::OutInfo ()
{ return outinfo; }

inline RpcRequest::ReqType RpcRequest::Type ()
{ return reqtype; }

#endif
