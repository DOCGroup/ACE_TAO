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

static char _rpcpp_request_cc_[]
= "request.cc,v 2.3 1992/06/15 19:12:39 mnl Exp";

// request.cc,v
// Revision 2.3  1992/06/15  19:12:39  mnl
// Fixed a few bugs, clarified interface.
//
// Revision 2.2  1992/06/13  14:27:01  mnl
// Adapted to (patched) gcc-2.2. Fixed several bugs.
//
// Revision 2.1.1.1  1992/03/08  13:28:42  mnl
// Initial mnl version.
//

#ifdef __GNUG__
#pragma implementation
#endif

#include "rpc++/request.h"

#include <assert.h>

void RpcRequest::init (u_long req, int pars, int parsz,
		       const XdrInfo* out, const XdrInfo** in, int rt)
{
  params = pars;
  parmsz = parsz;
  reqnum = req;
  ininfo = in;
  ininfo[params] = (XdrInfo*)0;
  outinfo = out;
  reqtype = rt;
  assert (rt == normal || outinfo->Proc () == 0);
}

RpcRequest::RpcRequest(u_long req, const XdrInfo* out, int t)
{
  init (req, 0, 0, out, new XdrInfo*[1], t);
}

RpcRequest::RpcRequest(u_long req, const XdrInfo* out, const XdrInfo* in,
		       int t)
{
  const XdrInfo** a = new XdrInfo*[2];
  a[0] = in;
  init (req, 1, -1, out, a, t);
}

RpcRequest::RpcRequest(u_long req, const XdrInfo* out, const XdrInfo* in0,
		       const XdrInfo* in1, int t)
{
  const XdrInfo** a = new XdrInfo*[3];
  a[0] = in0;
  a[1] = in1;
  init (req, 2, -1, out, a, t);
}

RpcRequest::RpcRequest(u_long req, const XdrInfo* out, const XdrInfo* in0,
		       const XdrInfo* in1, const XdrInfo* in2, int t)
{
  const XdrInfo** a = new XdrInfo*[4];
  a[0] = in0;
  a[1] = in1;
  a[2] = in2;
  init (req, 3, -1, out, a, t);
}

RpcRequest::RpcRequest(u_long req, const XdrInfo* out, const XdrInfo* in0,
		       const XdrInfo* in1, const XdrInfo* in2,
		       const XdrInfo* in3, int t)
{
  const XdrInfo** a = new XdrInfo*[5];
  a[0] = in0;
  a[1] = in1;
  a[2] = in2;
  a[3] = in3;
  init (req, 4, -1, out, a, t);
}

RpcRequest::RpcRequest(u_long req, const XdrInfo* out, const XdrInfo* in0,
		       const XdrInfo* in1, const XdrInfo* in2,
		       const XdrInfo* in3, const XdrInfo* in4, int t)
{
  const XdrInfo** a = new XdrInfo*[6];
  a[0] = in0;
  a[1] = in1;
  a[2] = in2;
  a[3] = in3;
  a[4] = in4;
  init (req, 5, -1, out, a, t);
}

RpcRequest::RpcRequest(u_long req, const XdrInfo* out, const XdrInfo* in0,
		       const XdrInfo* in1, const XdrInfo* in2,
		       const XdrInfo* in3, const XdrInfo* in4,
		       const XdrInfo* in5, int t)
{
  const XdrInfo** a = new XdrInfo*[7];
  a[0] = in0;
  a[1] = in1;
  a[2] = in2;
  a[3] = in3;
  a[4] = in4;
  a[5] = in5;
  init (req, 6, -1, out, a, t);
}

RpcRequest::RpcRequest(u_long req, const XdrInfo* out, const XdrInfo* in0,
		       const XdrInfo* in1, const XdrInfo* in2,
		       const XdrInfo* in3, const XdrInfo* in4,
		       const XdrInfo* in5, const XdrInfo *in6, int t)
{
  const XdrInfo** a = new XdrInfo*[8];
  a[0] = in0;
  a[1] = in1;
  a[2] = in2;
  a[3] = in3;
  a[4] = in4;
  a[5] = in5;
  a[6] = in6;
  init (req, 7, -1, out, a, t);
}

RpcRequest::RpcRequest(u_long req, const XdrInfo* out, const XdrInfo** ins,
		       int t)
{
  int pars = 0;
  for (XdrInfo** p = ins; *p; p++)
    pars += 1;
  const XdrInfo** a = new XdrInfo* [pars + 1];
  memcpy (a, ins, (pars + 1) * sizeof (XdrInfo*));
  init (req, pars, -1, out, a, t);
}

int RpcRequest::ParamSize ()
{
  if (parmsz < 0) // not yet calculated
    {
      parmsz = 0;
      for (XdrInfo** ip = ininfo; *ip; ip++)
	parmsz += (*ip)->Size ();
    }
  return parmsz;
}
