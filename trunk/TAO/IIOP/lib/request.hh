// @(#)request.hh	1.2 95/10/02
// Copyright 1994-1995 by Sun Microsystems, Inc.
//
// Header file for Win32 C/C++/COM interface to CORBA's Dynamic
// Invocation Interface "Request" type.
//
#if !defined(ACE_ROA_REQUEST_H)
#  define ACE_ROA_REQUEST_H

void                     CORBA_release (CORBA_Request_ptr req);
CORBA_Boolean            CORBA_is_nil (CORBA_Request_ptr req);

typedef CORBA_SEQUENCE <CORBA_TypeCode_ptr> CORBA_ExceptionList;
typedef CORBA_ExceptionList *CORBA_ExceptionList_ptr;

extern const IID         IID_CORBA_Request;

class _EXPCLASS CORBA_Request : public IUnknown
{
public:
  //
  // XXX these should not be inlined
  //
  CORBA_Object_ptr		target () const { return _target; }
  const CORBA_Char		*operation () const { return _opname; }
  CORBA_NVList_ptr		arguments () { return _args; }
  CORBA_NamedValue_ptr        result () { return _result; }
  CORBA_ExceptionList_ptr	exceptions () { return &_exceptions; }
  CORBA_Environment           *env () { return &_env; }

  void                        invoke ();
  void                        send_oneway ();

  //
  // Stuff required for COM IUnknown support
  //
  ULONG __stdcall             AddRef ();
  ULONG __stdcall             Release ();
  HRESULT __stdcall           QueryInterface (
					      REFIID      riid,
					      void        **ppv
					      );

private:
  friend class CORBA_Object;

  CORBA_Request (
		 CORBA_Object_ptr	    obj,
		 const CORBA_Char        *op,
		 CORBA_NVList_ptr        args,
		 CORBA_NamedValue_ptr    result,
		 CORBA_Flags             flags
		 );

  CORBA_Request (
		 CORBA_Object_ptr	    obj,
		 const CORBA_Char	    *op
		 );

  virtual ~CORBA_Request ();

  CORBA_Object_ptr            _target;
  const CORBA_Char		*_opname;
  CORBA_NVList_ptr		_args;
  CORBA_NamedValue_ptr        _result;
  CORBA_Flags			_flags;
  CORBA_Environment           _env;
  CORBA_ExceptionList         _exceptions;

  ACE_Thread_Mutex lock_;
  unsigned                    _refcount;
};
#endif
