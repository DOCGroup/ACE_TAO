// -*- c++ -*-
#ifndef _CALCSERVICE_H_
#define _CALCSERVICE_H_
static char _calcsvc_h_[]
= "calcsvc.h,v 2.3 1992/06/15 19:13:15 mnl Exp";

// calcsvc.h,v
// Revision 2.3  1992/06/15  19:13:15  mnl
// Fixed a few bugs, clarified interface.
//

#ifdef __GNUG__
#pragma interface
#endif

#include "rpc++/request.h"

#define CALCSVC 0x20100001

struct CalcRequests
{
  static RpcRequest Add;
  static RpcRequest Sub;
  static RpcRequest Times;
  static RpcRequest Div;
  static RpcRequest Inc;
  static RpcRequest IToA;
  static RpcRequest Reset; // for testing proc without args
  static RpcRequest Sleep; // for testing async
  static RpcRequest Msg;   // for testing string passing
  static RpcRequest Invalid; // for testing error handling
};

#endif
