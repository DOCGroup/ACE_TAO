// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    request.h
//
// = DESCRIPTION
//     
//    Header file for Win32 C/C++/COM interface to CORBA's Dynamic
//    Invocation Interface "Request" type.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
// 
// ============================================================================

#if !defined (TAO_REQUEST_H)
#  define TAO_REQUEST_H

#if 0
#  include "ace/OS.h"

#  include "tao/sequence.h"
#  include "tao/except.h"
#endif

class ACE_Svc_Export CORBA_Request : public IUnknown
{
public:
  // XXX these should not be inlined

  CORBA::Object_ptr target (void) const { return _target; }
  const CORBA::Char *operation (void) const { return _opname; }
  CORBA::NVList_ptr arguments (void) { return _args; }
  CORBA::NamedValue_ptr result (void) { return _result; }
  CORBA::ExceptionList_ptr exceptions (void) { return &_exceptions; }
  CORBA::Environment *env (void) { return &_env; }

  void invoke (void);
  void send_oneway (void);

  // Stuff required for COM IUnknown support

  ULONG __stdcall AddRef (void);
  ULONG __stdcall Release (void);
  HRESULT __stdcall QueryInterface (REFIID riid,
				    void **ppv);

private:
  friend class CORBA_Object;

  CORBA_Request (CORBA::Object_ptr obj,
		 const CORBA::Char *op,
		 CORBA::NVList_ptr args,
		 CORBA::NamedValue_ptr result,
		 CORBA::Flags flags);

  CORBA_Request (CORBA::Object_ptr obj,
		 const CORBA::Char *op);

  virtual ~CORBA_Request (void);

  CORBA::Object_ptr _target;
  const CORBA::Char *_opname;
  CORBA::NVList_ptr _args;
  CORBA::NamedValue_ptr _result;
  CORBA::Flags _flags;
  CORBA::Environment _env;
  CORBA::ExceptionList _exceptions;

  ACE_SYNCH_MUTEX lock_;
  u_int refcount_;
};

#endif /* TAO_REQUEST_H */
