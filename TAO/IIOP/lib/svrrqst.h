// @(#)svrrqst.hh	1.5 95/09/25
//
// Header file for Win32 C/C++/COM interface to CORBA's Dynamic
// Server Skeleton Interface's "Server Request" type.
//
// XXX this is currently in an intermediate state; this is not
// supposed to be IIOP-specific, or to expose quite so many
// implementation details, but right now it is.
//

#ifndef SVRRQST_HDR
#  define SVRRQST_HDR

#include    "cdr.h"

void                              CORBA_release (CORBA_ServerRequest_ptr req);
CORBA_Boolean                     CORBA_is_nil (CORBA_ServerRequest_ptr req);

extern const IID                  IID_CORBA_ServerRequest;

class _EXPCLASS CORBA_ServerRequest : public IUnknown
{
  public:
    //
    // Implementation uses this to provide the ORB with the operation's
    // parameter list ... on return, their values are available; the list
    // fed in has typecodes and (perhap) memory assigned.
    //
    virtual void __stdcall	params (
				    CORBA_NVList_ptr	list,
				    CORBA_Environment	&env
				) = 0;

    //
    // Implementation uses this to provide the operation result ... illegal
    // if exception() was called or params() was not called.
    //
    // XXX Implementation should be able to assume response has been sent
    // when this returns, and reclaim memory it allocated.
    //
    virtual void __stdcall	result (
				    CORBA_Any_ptr	value,
				    CORBA_Environment       &env
				) = 0;

    //
    // Implementation uses this to provide the exception value which is the
    // only result of this particular invocation.
    //
    // XXX Implementation should be able to assume response has been sent
    // when this returns, and reclaim memory it allocated.
    //
    virtual void __stdcall	exception (
				    CORBA_ExceptionType	type,
				    CORBA_Any_ptr           value,
				    CORBA_Environment       &env
				) = 0;

    //
    // Get various universal call attributes:  who made the call, the
    // target of the call, what ORB and OA that target object uses.
    //
    // NOTE:  none of these report exceptions; unavailability of any of
    // this stuff is a catastrophic error since this is all part of
    // the basic CORBA Object Model.
    //
    // XXX should not be not assuming all OAs implement the BOA API !!
    //
    virtual CORBA_Principal_ptr __stdcall	caller () = 0;
    virtual CORBA_Object_ptr __stdcall		target () = 0;
    virtual CORBA_String __stdcall		op_name () = 0;
    virtual CORBA_BOA_ptr __stdcall			oa () = 0;
    virtual CORBA_ORB_ptr __stdcall		orb () = 0;
};

extern const IID                  IID_IIOP_ServerRequest;

class _EXPCLASS IIOP_ServerRequest : public CORBA_ServerRequest
{
public:
  //
  // Constructor, destructor
  //
  IIOP_ServerRequest (
		      CDR			*msg,
		      CORBA_ORB_ptr	the_orb,
		      CORBA_BOA_ptr		the_boa
		      )
    : _incoming (msg), _params (0), _retval (0),
      _exception (0),
      _ex_type (NO_EXCEPTION),
      _refcount (1),
      _orb (the_orb),
      _boa (the_boa)
  { }

  virtual ~IIOP_ServerRequest ();

  //
  // General ServerRequest operations
  //
  void __stdcall		params (
					CORBA_NVList_ptr	list,
					CORBA_Environment	&env
					);

  void __stdcall		result (
					CORBA_Any_ptr	value,
					CORBA_Environment	&env
					);

  void __stdcall		exception (
					   CORBA_ExceptionType	type,
					   CORBA_Any_ptr       value,
					   CORBA_Environment   &env
					   );

  //
  // Request attributes
  //
  CORBA_String __stdcall		op_name ();
  CORBA_Principal_ptr __stdcall	caller ();
  CORBA_Object_ptr __stdcall		target ();
  CORBA_ORB_ptr __stdcall		orb ();
  CORBA_BOA_ptr __stdcall			oa ();

  //
  // Stuff required for COM IUnknown support
  //
  ULONG __stdcall		AddRef ();
  ULONG __stdcall		Release ();
  HRESULT __stdcall		QueryInterface (
						REFIID      riid,
						void        **ppv
						);

  // private:
  CORBA_String		_opname;
  CDR				*_incoming;
  CORBA_NVList_ptr		_params;
  CORBA_Any_ptr		_retval;
  CORBA_Any_ptr		_exception;
  CORBA_ExceptionType		_ex_type;

  //
  // Just drop the refcount, don't destroy the object; most
  // of these are stack-allocated.
  //
  void			release () { _refcount--; }

private:
  unsigned			_refcount;
  ACE_Thread_Mutex lock_;
  CORBA_ORB_ptr		_orb;
  CORBA_BOA_ptr			_boa;
};
#endif
