// -*- c++ -*-
static char _client_cc_[]
= "client.cc,v 2.3 1992/06/15 19:13:17 mnl Exp";

// client.cc,v
// Revision 2.3  1992/06/15  19:13:17  mnl
// Fixed a few bugs, clarified interface.
//
// Revision 2.2  1992/06/13  14:27:32  mnl
// Adapted to (patched) gcc-2.2. Fixed several bugs.
//
// Revision 2.1.1.1  1992/03/08  13:28:45  mnl
// Initial mnl version.
//

#include <stream.h>
#include "rpc++/stub.h"

#include "calcsvc.h"

class CalcStub : public RpcStub
{
public:
  inline CalcStub (u_long prognum, u_long versnum,
		   char* hostname = "localhost",
		   timeval timeout = defaultTimeout, bool connect = TRUE)
    : RpcStub (prognum, versnum, hostname, timeout, connect) {}

  inline void Reset ()
    { Call (CalcRequests::Reset); }
  inline int Inc (int i)
    { return *(int*)Call (CalcRequests::Inc, &i); }
  inline char* IToA (int i)
    { return *(char**)Call (CalcRequests::IToA, &i); }
  inline int Add (int s1, int s2)
    { return *(int*)Call (CalcRequests::Add, &s1, &s2); }

  inline void Msg (char* msg)
    { Call (CalcRequests::Msg, &msg); }
  inline void Sleep (int secs)
    { Call (CalcRequests::Sleep, &secs); }
};

main (int argc, char* argv[])
{
  char* server = argv[1];

  CalcStub svc (CALCSVC, 1, server);
  svc.Reset ();
  svc.Msg ("Hello server.\n");
  int i = 0, o;
  while (i < 10)
    {
      o = svc.Inc (i);
      cout << svc.IToA (o) << '\n';
      i = o;
    }
  i = svc.Add (12, 23);
  cout << "add (" << 12 << ", " << 23 << ") = " << i << endl;
  cout << "Calling Sleep (5) asynchronously.\n";
  svc.Sleep (5);
  cout << "Sleep call completed.\n";
  svc.Call (CalcRequests::Invalid);
}
