// @(#)svrrqst.hh	1.3 95/11/04
// Copyright 1994-1995 by Sun Microsystems, Inc.
// All Rights Reserved
//
// Header file for Win32 C/C++/COM interface to CORBA's Dynamic
// Server Skeleton Interface's "Server Request" type.
//

#ifndef	_svrrqst_hh
#define	_svrrqst_hh

#include    "runtime/cdr.hh"

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
				    TOA_ptr		the_toa
				)
				: _incoming (msg), _params (0), _retval (0),
					_exception (0),
					_ex_type (NO_EXCEPTION),
					_refcount (1),
					_orb (the_orb),
					_toa (the_toa)
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
    TOA_ptr __stdcall			oa ();

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
    CORBA_ORB_ptr		_orb;
    TOA_ptr			_toa;
};

#endif	// _svrrqst_hh

