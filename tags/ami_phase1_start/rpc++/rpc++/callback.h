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

#ifndef _RPCCALLBACK_H_
#define _RPCCALLBACK_H_
static char _rpcpp_callback_h_[]
= "callback.h,v 2.3 1992/06/15 19:13:26 mnl Exp";

// callback.h,v
// Revision 2.3  1992/06/15  19:13:26  mnl
// Fixed a few bugs, clarified interface.
//
// Revision 2.2  1992/06/13  14:27:37  mnl
// Adapted to (patched) gcc-2.2. Fixed several bugs.
//
// Revision 2.1.1.1  1992/03/08  13:28:43  mnl
// Initial mnl version.
//

//
// The user interface to RpcCallback is quiet simple. The only function
// needed is the constructor "RpcCallback (T* o, void* m)". Given a class
// C with a method m and an object o of Type C, a callback to the method
// can be constructed with "RpcCallback<C> (&O, &C::m)".
//     As an alternative, an RpcExtCallback may be constructed with
// "RpcExtCallback (T* o, void* m, RpcService*)". In this case, the method
// is called with a pointer to the RpcService object as a third argument.
//
//     A pointer to a callback can be casted to AnyRpcCallback by
// whatever routine receives it and the call can be made by applying "Do"
// to the callback.
//

class RpcService;

class AnyRpcCallback
{
  friend class RpcService;

protected:
  int params;
  RpcService* svc;
  virtual void* Do (void** in) = 0;

public:
  inline virtual ~AnyRpcCallback () {}
  inline virtual AnyRpcCallback* CopyToHeap () const = 0;
  inline int Params () const
    { return params; }
  inline void SetService (RpcService* s)
    { svc = s; }
};

class RpcCallback : public AnyRpcCallback
{
private:
  typedef void* (*ProcN)(void**);
  typedef void* (*Proc0)();
  typedef void* (*Proc1)(void*);
  typedef void* (*Proc2)(void*, void*);
  typedef void* (*Proc3)(void*, void*, void*);
  typedef void* (*Proc4)(void*, void*, void*, void*);
  typedef void* (*Proc5)(void*, void*, void*, void*, void*);
  typedef void* (*Proc6)(void*, void*, void*, void*, void*, void*);
  typedef void* (*Proc7)(void*, void*, void*, void*, void*, void*, void*);

  typedef void (*ProcNv)(void**);
  typedef void (*Proc0v)();
  typedef void (*Proc1v)(void*);
  typedef void (*Proc2v)(void*, void*);
  typedef void (*Proc3v)(void*, void*, void*);
  typedef void (*Proc4v)(void*, void*, void*, void*);
  typedef void (*Proc5v)(void*, void*, void*, void*, void*);
  typedef void (*Proc6v)(void*, void*, void*, void*, void*, void*);
  typedef void (*Proc7v)(void*, void*, void*, void*, void*, void*, void*);

  typedef void* (*ProcNs)(void**, RpcService*);
  typedef void* (*Proc0s)(RpcService*);
  typedef void* (*Proc1s)(void*, RpcService*);
  typedef void* (*Proc2s)(void*, void*, RpcService*);
  typedef void* (*Proc3s)(void*, void*, void*, RpcService*);
  typedef void* (*Proc4s)(void*, void*, void*, void*, RpcService*);
  typedef void* (*Proc5s)(void*, void*, void*, void*, void*, RpcService*);
  typedef void* (*Proc6s)(void*, void*, void*, void*,
			       void*, void*, RpcService*);
  typedef void* (*Proc7s)(void*, void*, void*, void*,
			       void*, void*, void*, RpcService*);

  typedef void (*ProcNvs)(void**, RpcService*);
  typedef void (*Proc0vs)(RpcService*);
  typedef void (*Proc1vs)(void*, RpcService*);
  typedef void (*Proc2vs)(void*, void*, RpcService*);
  typedef void (*Proc3vs)(void*, void*, void*, RpcService*);
  typedef void (*Proc4vs)(void*, void*, void*, void*, RpcService*);
  typedef void (*Proc5vs)(void*, void*, void*, void*, void*, RpcService*);
  typedef void (*Proc6vs)(void*, void*, void*, void*,
			       void*, void*, RpcService*);
  typedef void (*Proc7vs)(void*, void*, void*, void*,
			       void*, void*, void*, RpcService*);

  typedef void* (RpcCallback::*DoPtr)(void**);
  DoPtr todo;
  union
    {
      ProcN procN;ProcNv procNv;ProcNs procNs;ProcNvs procNvs;
      Proc0 proc0;Proc0v proc0v;Proc0s proc0s;Proc0vs proc0vs;
      Proc1 proc1;Proc1v proc1v;Proc1s proc1s;Proc1vs proc1vs;
      Proc2 proc2;Proc2v proc2v;Proc2s proc2s;Proc2vs proc2vs;
      Proc3 proc3;Proc3v proc3v;Proc3s proc3s;Proc3vs proc3vs;
      Proc4 proc4;Proc4v proc4v;Proc4s proc4s;Proc4vs proc4vs;
      Proc5 proc5;Proc5v proc5v;Proc5s proc5s;Proc5vs proc5vs;
      Proc6 proc6;Proc6v proc6v;Proc6s proc6s;Proc6vs proc6vs;
      Proc7 proc7;Proc7v proc7v;Proc7s proc7s;Proc7vs proc7vs;
    };
  inline RpcCallback () {}
  inline void* Do (void** in)
    { return (this->*todo)(in); }

  inline void* DoN (void** in)
    { return (*procN)(in); }
  inline void* Do0 (void** in)
    { return (*proc0)(); }
  inline void* Do1 (void** in)
    { return (*proc1)(in[0]); }
  inline void* Do2 (void** in)
    { return (*proc2)(in[0], in[1]); }
  inline void* Do3 (void** in)
    { return (*proc3)(in[0], in[1], in[2]); }
  inline void* Do4 (void** in)
    { return (*proc4)(in[0], in[1], in[2], in[3]); }
  inline void* Do5 (void** in)
    { return (*proc5)(in[0], in[1], in[2], in[3], in[4]); }
  inline void* Do6 (void** in)
    { return (*proc6)(in[0], in[1], in[2], in[3], in[4], in[5]); }
  inline void* Do7 (void** in)
    { return (*proc7)(in[0], in[1], in[2], in[3], in[4], in[5], in[6]); }
  inline void* DoNv (void** in)
    { (*procNv)(in); return (void*)0; }
  inline void* Do0v (void** in)
    { (*proc0v)(); return (void*)0; }
  inline void* Do1v (void** in)
    { (*proc1v)(in[0]); return (void*)0; }
  inline void* Do2v (void** in)
    { (*proc2v)(in[0], in[1]); return (void*)0; }
  inline void* Do3v (void** in)
    { (*proc3v)(in[0], in[1], in[2]); return (void*)0; }
  inline void* Do4v (void** in)
    { (*proc4v)(in[0], in[1], in[2], in[3]); return (void*)0; }
  inline void* Do5v (void** in)
    { (*proc5v)(in[0], in[1], in[2], in[3], in[4]); return (void*)0; }
  inline void* Do6v (void** in)
    { (*proc6v)(in[0], in[1], in[2], in[3], in[4], in[5]); return (void*)0; }
  inline void* Do7v (void** in)
    { (*proc7v)(in[0], in[1], in[2], in[3], in[4], in[5], in[6]);
      return (void*)0; }

  inline void* DoNs (void** in)
    { return (*procNs)(in, svc); }
  inline void* Do0s (void** in)
    { return (*proc0s)(svc); }
  inline void* Do1s (void** in)
    { return (*proc1s)(in[0], svc); }
  inline void* Do2s (void** in)
    { return (*proc2s)(in[0], in[1], svc); }
  inline void* Do3s (void** in)
    { return (*proc3s)(in[0], in[1], in[2], svc); }
  inline void* Do4s (void** in)
    { return (*proc4s)(in[0], in[1], in[2], in[3], svc); }
  inline void* Do5s (void** in)
    { return (*proc5s)(in[0], in[1], in[2], in[3], in[4], svc); }
  inline void* Do6s (void** in)
    { return (*proc6s)(in[0], in[1], in[2], in[3], in[4], in[5], svc); }
  inline void* Do7s (void** in)
    { return (*proc7s)(in[0], in[1], in[2], in[3], in[4], in[5], in[6], svc); }
  inline void* DoNvs (void** in)
    { (*procNvs)(in, svc); return (void*)0; }
  inline void* Do0vs (void** in)
    { (*proc0vs)(svc); return (void*)0; }
  inline void* Do1vs (void** in)
    { (*proc1vs)(in[0], svc); return (void*)0; }
  inline void* Do2vs (void** in)
    { (*proc2vs)(in[0], in[1], svc); return (void*)0; }
  inline void* Do3vs (void** in)
    { (*proc3vs)(in[0], in[1], in[2], svc); return (void*)0; }
  inline void* Do4vs (void** in)
    { (*proc4vs)(in[0], in[1], in[2], in[3], svc); return (void*)0; }
  inline void* Do5vs (void** in)
    { (*proc5vs)(in[0], in[1], in[2], in[3], in[4], svc); return (void*)0; }
  inline void* Do6vs (void** in)
    { (*proc6vs)(in[0], in[1], in[2], in[3], in[4], in[5], svc);
      return (void*)0; }
  inline void* Do7vs (void** in)
    { (*proc7vs)(in[0], in[1], in[2], in[3],
		   in[4], in[5], in[6], svc); return (void*)0; }

public:
  inline RpcCallback (ProcN p)
    { todo = &this->DoN; procN = p; params = -1; }
  inline RpcCallback (Proc0 p)
    { todo = &this->Do0; proc0 = p; params = 0; }
  inline RpcCallback (Proc1 p)
    { todo = &this->Do1; proc1 = p; params = 1; }
  inline RpcCallback (Proc2 p)
    { todo = &this->Do2; proc2 = p; params = 2; }
  inline RpcCallback (Proc3 p)
    { todo = &this->Do3; proc3 = p; params = 3; }
  inline RpcCallback (Proc4 p)
    { todo = &this->Do4; proc4 = p; params = 4; }
  inline RpcCallback (Proc5 p)
    { todo = &this->Do5; proc5 = p; params = 5; }
  inline RpcCallback (Proc6 p)
    { todo = &this->Do6; proc6 = p; params = 6; }
  inline RpcCallback (Proc7 p)
    { todo = &this->Do7; proc7 = p; params = 7; }

  inline RpcCallback (ProcNv p)
    { todo = &this->DoNv; procNv = p; params = -1; }
  inline RpcCallback (Proc0v p)
    { todo = &this->Do0v; proc0v = p; params = 0; }
  inline RpcCallback (Proc1v p)
    { todo = &this->Do1v; proc1v = p; params = 1; }
  inline RpcCallback (Proc2v p)
    { todo = &this->Do2v; proc2v = p; params = 2; }
  inline RpcCallback (Proc3v p)
    { todo = &this->Do3v; proc3v = p; params = 3; }
  inline RpcCallback (Proc4v p)
    { todo = &this->Do4v; proc4v = p; params = 4; }
  inline RpcCallback (Proc5v p)
    { todo = &this->Do5v; proc5v = p; params = 5; }
  inline RpcCallback (Proc6v p)
    { todo = &this->Do6v; proc6v = p; params = 6; }
  inline RpcCallback (Proc7v p)
    { todo = &this->Do7v; proc7v = p; params = 7; }

  inline RpcCallback (ProcNs p)
    { todo = &this->DoNs; procNs = p; params = -1; }
  inline RpcCallback (Proc0s p)
    { todo = &this->Do0s; proc0s = p; params = 0; }
  inline RpcCallback (Proc1s p)
    { todo = &this->Do1s; proc1s = p; params = 1; }
  inline RpcCallback (Proc2s p)
    { todo = &this->Do2s; proc2s = p; params = 2; }
  inline RpcCallback (Proc3s p)
    { todo = &this->Do3s; proc3s = p; params = 3; }
  inline RpcCallback (Proc4s p)
    { todo = &this->Do4s; proc4s = p; params = 4; }
  inline RpcCallback (Proc5s p)
    { todo = &this->Do5s; proc5s = p; params = 5; }
  inline RpcCallback (Proc6s p)
    { todo = &this->Do6s; proc6s = p; params = 6; }
  inline RpcCallback (Proc7s p)
    { todo = &this->Do7s; proc7s = p; params = 7; }

  inline RpcCallback (ProcNvs p)
    { todo = &this->DoNvs; procNvs = p; params = -1; }
  inline RpcCallback (Proc0vs p)
    { todo = &this->Do0vs; proc0vs = p; params = 0; }
  inline RpcCallback (Proc1vs p)
    { todo = &this->Do1vs; proc1vs = p; params = 1; }
  inline RpcCallback (Proc2vs p)
    { todo = &this->Do2vs; proc2vs = p; params = 2; }
  inline RpcCallback (Proc3vs p)
    { todo = &this->Do3vs; proc3vs = p; params = 3; }
  inline RpcCallback (Proc4vs p)
    { todo = &this->Do4vs; proc4vs = p; params = 4; }
  inline RpcCallback (Proc5vs p)
    { todo = &this->Do5vs; proc5vs = p; params = 5; }
  inline RpcCallback (Proc6vs p)
    { todo = &this->Do6vs; proc6vs = p; params = 6; }
  inline RpcCallback (Proc7vs p)
    { todo = &this->Do7vs; proc7vs = p; params = 7; }

  inline virtual AnyRpcCallback* CopyToHeap () const
    {
      RpcCallback* p = new RpcCallback ();
      *p = *this;
      return p;
    }
};

template<class T> class RpcMethodCall : public AnyRpcCallback
{
private:
  typedef void* (T::*MethodN)(void**);
  typedef void* (T::*Method0)();
  typedef void* (T::*Method1)(void*);
  typedef void* (T::*Method2)(void*, void*);
  typedef void* (T::*Method3)(void*, void*, void*);
  typedef void* (T::*Method4)(void*, void*, void*, void*);
  typedef void* (T::*Method5)(void*, void*, void*, void*, void*);
  typedef void* (T::*Method6)(void*, void*, void*, void*, void*, void*);
  typedef void* (T::*Method7)(void*, void*, void*, void*, void*, void*, void*);

  typedef void (T::*MethodNv)(void**);
  typedef void (T::*Method0v)();
  typedef void (T::*Method1v)(void*);
  typedef void (T::*Method2v)(void*, void*);
  typedef void (T::*Method3v)(void*, void*, void*);
  typedef void (T::*Method4v)(void*, void*, void*, void*);
  typedef void (T::*Method5v)(void*, void*, void*, void*, void*);
  typedef void (T::*Method6v)(void*, void*, void*, void*, void*, void*);
  typedef void (T::*Method7v)(void*, void*, void*, void*, void*, void*, void*);

  typedef void* (T::*MethodNs)(void**, RpcService*);
  typedef void* (T::*Method0s)(RpcService*);
  typedef void* (T::*Method1s)(void*, RpcService*);
  typedef void* (T::*Method2s)(void*, void*, RpcService*);
  typedef void* (T::*Method3s)(void*, void*, void*, RpcService*);
  typedef void* (T::*Method4s)(void*, void*, void*, void*, RpcService*);
  typedef void* (T::*Method5s)(void*, void*, void*, void*, void*, RpcService*);
  typedef void* (T::*Method6s)(void*, void*, void*, void*,
			       void*, void*, RpcService*);
  typedef void* (T::*Method7s)(void*, void*, void*, void*,
			       void*, void*, void*, RpcService*);

  typedef void (T::*MethodNvs)(void**, RpcService*);
  typedef void (T::*Method0vs)(RpcService*);
  typedef void (T::*Method1vs)(void*, RpcService*);
  typedef void (T::*Method2vs)(void*, void*, RpcService*);
  typedef void (T::*Method3vs)(void*, void*, void*, RpcService*);
  typedef void (T::*Method4vs)(void*, void*, void*, void*, RpcService*);
  typedef void (T::*Method5vs)(void*, void*, void*, void*, void*, RpcService*);
  typedef void (T::*Method6vs)(void*, void*, void*, void*,
			       void*, void*, RpcService*);
  typedef void (T::*Method7vs)(void*, void*, void*, void*,
			       void*, void*, void*, RpcService*);

  typedef void* (RpcMethodCall::*DoPtr)(void**);
  DoPtr todo;
  T* object;
  union
    {
      MethodN methodN;MethodNv methodNv;MethodNs methodNs;MethodNvs methodNvs;
      Method0 method0;Method0v method0v;Method0s method0s;Method0vs method0vs;
      Method1 method1;Method1v method1v;Method1s method1s;Method1vs method1vs;
      Method2 method2;Method2v method2v;Method2s method2s;Method2vs method2vs;
      Method3 method3;Method3v method3v;Method3s method3s;Method3vs method3vs;
      Method4 method4;Method4v method4v;Method4s method4s;Method4vs method4vs;
      Method5 method5;Method5v method5v;Method5s method5s;Method5vs method5vs;
      Method6 method6;Method6v method6v;Method6s method6s;Method6vs method6vs;
      Method7 method7;Method7v method7v;Method7s method7s;Method7vs method7vs;
    };
  inline RpcMethodCall () {}
  inline void* Do (void** in)
    { return (this->*todo)(in); }

  inline void* DoN (void** in)
    { return (object->*methodN)(in); }
  inline void* Do0 (void** in)
    { return (object->*method0)(); }
  inline void* Do1 (void** in)
    { return (object->*method1)(in[0]); }
  inline void* Do2 (void** in)
    { return (object->*method2)(in[0], in[1]); }
  inline void* Do3 (void** in)
    { return (object->*method3)(in[0], in[1], in[2]); }
  inline void* Do4 (void** in)
    { return (object->*method4)(in[0], in[1], in[2], in[3]); }
  inline void* Do5 (void** in)
    { return (object->*method5)(in[0], in[1], in[2], in[3], in[4]); }
  inline void* Do6 (void** in)
    { return (object->*method6)(in[0], in[1], in[2], in[3], in[4], in[5]); }
  inline void* Do7 (void** in)
    { return (object->*method7)(in[0], in[1], in[2],
				in[3], in[4], in[5], in[6]); }
  inline void* DoNv (void** in)
    { (object->*methodNv)(in); return (void*)0; }
  inline void* Do0v (void** in)
    { (object->*method0v)(); return (void*)0; }
  inline void* Do1v (void** in)
    { (object->*method1v)(in[0]); return (void*)0; }
  inline void* Do2v (void** in)
    { (object->*method2v)(in[0], in[1]); return (void*)0; }
  inline void* Do3v (void** in)
    { (object->*method3v)(in[0], in[1], in[2]); return (void*)0; }
  inline void* Do4v (void** in)
    { (object->*method4v)(in[0], in[1], in[2], in[3]); return (void*)0; }
  inline void* Do5v (void** in)
    { (object->*method5v)(in[0], in[1], in[2], in[3], in[4]);
      return (void*)0; }
  inline void* Do6v (void** in)
    { (object->*method6v)(in[0], in[1], in[2], in[3], in[4], in[5]);
      return (void*)0; }
  inline void* Do7v (void** in)
    { (object->*method7v)(in[0], in[1], in[2], in[3], in[4], in[5], in[6]);
      return (void*)0; }

  inline void* DoNs (void** in)
    { return (object->*methodNs)(in, svc); }
  inline void* Do0s (void** in)
    { return (object->*method0s)(svc); }
  inline void* Do1s (void** in)
    { return (object->*method1s)(in[0], svc); }
  inline void* Do2s (void** in)
    { return (object->*method2s)(in[0], in[1], svc); }
  inline void* Do3s (void** in)
    { return (object->*method3s)(in[0], in[1], in[2], svc); }
  inline void* Do4s (void** in)
    { return (object->*method4s)(in[0], in[1], in[2], in[3], svc); }
  inline void* Do5s (void** in)
    { return (object->*method5s)(in[0], in[1], in[2], in[3], in[4], svc); }
  inline void* Do6s (void** in)
    { return (object->*method6s)(in[0], in[1], in[2],
				 in[3], in[4], in[5], svc); }
  inline void* Do7s (void** in)
    { return (object->*method7s)(in[0], in[1], in[2],
				 in[3], in[4], in[5], in[6], svc); }
  inline void* DoNvs (void** in)
    { (object->*methodNvs)(in, svc); return (void*)0; }
  inline void* Do0vs (void** in)
    { (object->*method0vs)(svc); return (void*)0; }
  inline void* Do1vs (void** in)
    { (object->*method1vs)(in[0], svc); return (void*)0; }
  inline void* Do2vs (void** in)
    { (object->*method2vs)(in[0], in[1], svc); return (void*)0; }
  inline void* Do3vs (void** in)
    { (object->*method3vs)(in[0], in[1], in[2], svc); return (void*)0; }
  inline void* Do4vs (void** in)
    { (object->*method4vs)(in[0], in[1], in[2], in[3], svc); return (void*)0; }
  inline void* Do5vs (void** in)
    { (object->*method5vs)(in[0], in[1], in[2], in[3], in[4], svc);
      return (void*)0; }
  inline void* Do6vs (void** in)
    { (object->*method6vs)(in[0], in[1], in[2], in[3], in[4], in[5], svc);
      return (void*)0; }
  inline void* Do7vs (void** in)
    { (object->*method7vs)(in[0], in[1], in[2], in[3],
			   in[4], in[5], in[6], svc); return (void*)0; }

public:
  inline RpcMethodCall (T* o, MethodN m)
    { todo = &this->DoN; object = o; methodN = m; params = -1; }
  inline RpcMethodCall (T* o, Method0 m)
    { todo = &this->Do0; object = o; method0 = m; params = 0; }
  inline RpcMethodCall (T* o, Method1 m)
    { todo = &this->Do1; object = o; method1 = m; params = 1; }
  inline RpcMethodCall (T* o, Method2 m)
    { todo = &this->Do2; object = o; method2 = m; params = 2; }
  inline RpcMethodCall (T* o, Method3 m)
    { todo = &this->Do3; object = o; method3 = m; params = 3; }
  inline RpcMethodCall (T* o, Method4 m)
    { todo = &this->Do4; object = o; method4 = m; params = 4; }
  inline RpcMethodCall (T* o, Method5 m)
    { todo = &this->Do5; object = o; method5 = m; params = 5; }
  inline RpcMethodCall (T* o, Method6 m)
    { todo = &this->Do6; object = o; method6 = m; params = 6; }
  inline RpcMethodCall (T* o, Method7 m)
    { todo = &this->Do7; object = o; method7 = m; params = 7; }

  inline RpcMethodCall (T* o, MethodNv m)
    { todo = &this->DoNv; object = o; methodNv = m; params = -1; }
  inline RpcMethodCall (T* o, Method0v m)
    { todo = &this->Do0v; object = o; method0v = m; params = 0; }
  inline RpcMethodCall (T* o, Method1v m)
    { todo = &this->Do1v; object = o; method1v = m; params = 1; }
  inline RpcMethodCall (T* o, Method2v m)
    { todo = &this->Do2v; object = o; method2v = m; params = 2; }
  inline RpcMethodCall (T* o, Method3v m)
    { todo = &this->Do3v; object = o; method3v = m; params = 3; }
  inline RpcMethodCall (T* o, Method4v m)
    { todo = &this->Do4v; object = o; method4v = m; params = 4; }
  inline RpcMethodCall (T* o, Method5v m)
    { todo = &this->Do5v; object = o; method5v = m; params = 5; }
  inline RpcMethodCall (T* o, Method6v m)
    { todo = &this->Do6v; object = o; method6v = m; params = 6; }
  inline RpcMethodCall (T* o, Method7v m)
    { todo = &this->Do7v; object = o; method7v = m; params = 7; }

  inline RpcMethodCall (T* o, MethodNs m)
    { todo = &this->DoNs; object = o; methodNs = m; params = -1; }
  inline RpcMethodCall (T* o, Method0s m)
    { todo = &this->Do0s; object = o; method0s = m; params = 0; }
  inline RpcMethodCall (T* o, Method1s m)
    { todo = &this->Do1s; object = o; method1s = m; params = 1; }
  inline RpcMethodCall (T* o, Method2s m)
    { todo = &this->Do2s; object = o; method2s = m; params = 2; }
  inline RpcMethodCall (T* o, Method3s m)
    { todo = &this->Do3s; object = o; method3s = m; params = 3; }
  inline RpcMethodCall (T* o, Method4s m)
    { todo = &this->Do4s; object = o; method4s = m; params = 4; }
  inline RpcMethodCall (T* o, Method5s m)
    { todo = &this->Do5s; object = o; method5s = m; params = 5; }
  inline RpcMethodCall (T* o, Method6s m)
    { todo = &this->Do6s; object = o; method6s = m; params = 6; }
  inline RpcMethodCall (T* o, Method7s m)
    { todo = &this->Do7s; object = o; method7s = m; params = 7; }

  inline RpcMethodCall (T* o, MethodNvs m)
    { todo = &this->DoNvs; object = o; methodNvs = m; params = -1; }
  inline RpcMethodCall (T* o, Method0vs m)
    { todo = &this->Do0vs; object = o; method0vs = m; params = 0; }
  inline RpcMethodCall (T* o, Method1vs m)
    { todo = &this->Do1vs; object = o; method1vs = m; params = 1; }
  inline RpcMethodCall (T* o, Method2vs m)
    { todo = &this->Do2vs; object = o; method2vs = m; params = 2; }
  inline RpcMethodCall (T* o, Method3vs m)
    { todo = &this->Do3vs; object = o; method3vs = m; params = 3; }
  inline RpcMethodCall (T* o, Method4vs m)
    { todo = &this->Do4vs; object = o; method4vs = m; params = 4; }
  inline RpcMethodCall (T* o, Method5vs m)
    { todo = &this->Do5vs; object = o; method5vs = m; params = 5; }
  inline RpcMethodCall (T* o, Method6vs m)
    { todo = &this->Do6vs; object = o; method6vs = m; params = 6; }
  inline RpcMethodCall (T* o, Method7vs m)
    { todo = &this->Do7vs; object = o; method7vs = m; params = 7; }

  inline virtual AnyRpcCallback* CopyToHeap () const
    {
      RpcMethodCall* p = new RpcMethodCall ();
      *p = *this;
      return p;
    }
};

#endif
