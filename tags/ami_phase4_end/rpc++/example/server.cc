// -*- c++ -*-
static char _server_cc_[]
= "server.cc,v 2.3 1992/06/15 19:13:18 mnl Exp";

// server.cc,v
// Revision 2.3  1992/06/15  19:13:18  mnl
// Fixed a few bugs, clarified interface.
//
// Revision 2.2  1992/06/13  14:27:33  mnl
// Adapted to (patched) gcc-2.2. Fixed several bugs.
//
// Revision 2.1.1.1  1992/03/08  13:28:45  mnl
// Initial mnl version.
//

#include <unistd.h>
#include <stream.h>
#include <string.h>
#include <malloc.h>

#include "rpc++/service.h"
#include "calcsvc.h"

class Calc
{
public:
  void* Add (void*, void*);
  void* Sub (void*, void*);
  void* Times (void*, void*);
  void* Div (void*, void*);
  void* Inc (void*);
  void* IToA (void**);
  void Reset (RpcService*);
};

void* Calc::Add (void* in1, void* in2)
{
  static int res;
  res = *(int*)in1 + *(int*)in2;
  return &res;
}

void* Calc::Sub (void* in1, void* in2)
{
  static int res;
  res = *(int*)in1 - *(int*)in2;
  return &res;
}

void* Calc::Times (void* in1, void* in2)
{
  static int res;
  res = *(int*)in1 * *(int*)in2;
  return &res;
}

void* Calc::Div (void* in1, void* in2)
{
  static int res;
  res = *(int*)in1 / *(int*)in2;
  return &res;
}

void* Calc::Inc (void* in)
{
  static int res;
  res = *(int*)in + 1;
  return &res;
}

void* Calc::IToA (void** in)
{
  static char *s = 0;
  delete s;
  char *t = form ("%d", *(int*)in[0]);
  s = new char[strlen (t) + 1];
  strcpy (s, t);
  return &s;
}

void Calc::Reset (RpcService* svc)
{
  cout << "Received reset from " << svc->CallerName () << ".\n";
}

void printMessage (void* in)
{
  cout << *(char**)in;
}

void doSleep (void* in)
{
  cout << form ("Sleeping %d secs.\n", *(int*)in);
  sleep (*(int*)in);
  cout << "Woke up.\n";
}

main ()
{
  RpcService svc (CALCSVC, 1);
  Calc calc;
  svc.Register (CalcRequests::Add, RpcMethodCall<Calc> (&calc, &calc.Add));
  svc.Register (CalcRequests::Sub, RpcMethodCall<Calc> (&calc, &calc.Sub));
  svc.Register (CalcRequests::Times, RpcMethodCall<Calc> (&calc, &calc.Times));
  svc.Register (CalcRequests::Div, RpcMethodCall<Calc> (&calc, &calc.Div));
  svc.Register (CalcRequests::Inc, RpcMethodCall<Calc> (&calc, &calc.Inc));
  svc.Register (CalcRequests::IToA,RpcMethodCall<Calc>(&calc, &calc.IToA));
  svc.Register (CalcRequests::Reset, RpcMethodCall<Calc> (&calc, &calc.Reset));
  svc.Register (CalcRequests::Msg, RpcCallback (printMessage));
  svc.Register (CalcRequests::Sleep, RpcCallback (doSleep));
  svc.Provide ();
}
