// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    svrrqst.h
//
// = DESCRIPTION
//     
//    Header file for Win32 C/C++/COM interface to CORBA's Dynamic
//    Server Skeleton Interface's "Server Request" type.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
//
// = VERSION
//     $Id$
// 
// ============================================================================

#if !defined (TAO_SVRRQST_H)
#define TAO_SVRRQST_H

#if 0
#include "ace/OS.h"
#include "tao/cdr.h"
#endif

class ACE_Svc_Export CORBA_ServerRequest : public IUnknown
{
  // = TITLE
  // XXX this is currently in an intermediate state; this is not
  // supposed to be IIOP-specific, or to expose quite so many
  // implementation details, but right now it is.
public:
  virtual void __stdcall params (CORBA::NVList_ptr list,
				 CORBA::Environment &env) = 0;
  // Implementation uses this to provide the ORB with the operation's
  // parameter list ... on return, their values are available; the
  // list fed in has typecodes and (perhap) memory assigned.

  virtual void __stdcall result (CORBA::Any_ptr value,
				 CORBA::Environment &env) = 0;
  // Implementation uses this to provide the operation result
  // ... illegal if exception() was called or params() was not called.
  //
  // XXX Implementation should be able to assume response has been
  // sent when this returns, and reclaim memory it allocated.

  virtual void __stdcall exception (CORBA::ExceptionType	type,
				    CORBA::Any_ptr value,
				    CORBA::Environment &env) = 0;
  // Implementation uses this to provide the exception value which is
  // the only result of this particular invocation.
  //
  // XXX Implementation should be able to assume response has been
  // sent when this returns, and reclaim memory it allocated.

  // = Get various universal call attributes.

  // e.g., who made the call, the target of the call, what ORB and OA
  // that target object uses.
  //
  // NOTE: none of these report exceptions; unavailability of any of
  // this stuff is a catastrophic error since this is all part of the
  // basic CORBA Object Model.
  //
  // XXX should not be not assuming all OAs implement the BOA API !!
  virtual CORBA::Principal_ptr __stdcall	caller (void) = 0;
  virtual CORBA::Object_ptr __stdcall target (void) = 0;
  virtual CORBA::String __stdcall op_name (void) = 0;
  virtual CORBA::BOA_ptr __stdcall oa (void) = 0;
  virtual CORBA::ORB_ptr __stdcall orb (void) = 0;
};

class ACE_Svc_Export IIOP_ServerRequest : public CORBA_ServerRequest
{
public:
  // Constructor, destructor

  IIOP_ServerRequest (CDR *msg,
		      CORBA::ORB_ptr the_orb,
		      CORBA::BOA_ptr the_boa);

  virtual ~IIOP_ServerRequest (void);

  // = General ServerRequest operations
  void __stdcall params (CORBA::NVList_ptr list,
			 CORBA::Environment &env);

  void __stdcall result (CORBA::Any_ptr value,
			 CORBA::Environment &env);

  void __stdcall exception (CORBA::ExceptionType	type,
			    CORBA::Any_ptr value,
			    CORBA::Environment &env);

  // = Request attributes

  CORBA::String __stdcall op_name (void);
  CORBA::Principal_ptr __stdcall	caller (void);
  CORBA::Object_ptr __stdcall target (void);
  CORBA::ORB_ptr __stdcall orb (void);
  CORBA::BOA_ptr __stdcall oa (void);

  // Stuff required for COM IUnknown support

  ULONG __stdcall AddRef (void);
  ULONG __stdcall Release (void);
  HRESULT __stdcall QueryInterface (REFIID riid,
				    void **ppv);

  // private:
  CORBA::String _opname;
  CDR *_incoming;
  CORBA::NVList_ptr _params;
  CORBA::Any_ptr _retval;
  CORBA::Any_ptr _exception;
  CORBA::ExceptionType _ex_type;

  void release (void) { refcount_--; }
  // Just drop the refcount, don't destroy the object; most of these
  // are stack-allocated.

private:
  ACE_SYNCH_MUTEX lock_;

  u_int refcount_;

  CORBA::ORB_ptr _orb;

  CORBA::BOA_ptr _boa;
};

#endif /* TAO_SVRRQST_H */
