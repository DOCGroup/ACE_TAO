// -*- c++ -*-
static char _calcsvc_cc_[]
= "calcsvc.cc,v 2.3 1992/06/15 19:13:13 mnl Exp";

// calcsvc.cc,v
// Revision 2.3  1992/06/15  19:13:13  mnl
// Fixed a few bugs, clarified interface.
//
// Revision 2.2  1992/06/13  14:27:28  mnl
// Adapted to (patched) gcc-2.2. Fixed several bugs.
//
// Revision 2.1.1.1  1992/03/08  13:28:45  mnl
// Initial mnl version.
//

#include <stream.h>
#include "calcsvc.h"

XdrInfo& Xmyint = Xdr::Xint;

RpcRequest CalcRequests::Add (1, &Xmyint, &Xdr::Xint, &Xdr::Xint);
RpcRequest CalcRequests::Sub (2, &Xdr::Xint, &Xdr::Xint, &Xdr::Xint);
RpcRequest CalcRequests::Times (3, &Xdr::Xint, &Xdr::Xint, &Xdr::Xint);
RpcRequest CalcRequests::Div (4, &Xdr::Xint, &Xdr::Xint, &Xdr::Xint);
RpcRequest CalcRequests::Inc (5, &Xdr::Xint, &Xdr::Xint);
RpcRequest CalcRequests::IToA (6, &Xdr::Xwrapstring, &Xdr::Xint);
RpcRequest CalcRequests::Reset (7, &Xdr::Xvoid);
RpcRequest CalcRequests::Sleep (8, &Xdr::Xnull, &Xdr::Xint, RpcRequest::async);
RpcRequest CalcRequests::Msg (9, &Xdr::Xvoid, &Xdr::Xwrapstring);
RpcRequest CalcRequests::Invalid (100, &Xdr::Xvoid, &Xdr::Xvoid);
