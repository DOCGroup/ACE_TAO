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

#  include "ace/OS.h"

#  include "sequence.h"

void ACE_Svc_Export CORBA_release (CORBA_Request_ptr req);
CORBA_Boolean ACE_Svc_Export CORBA_is_nil (CORBA_Request_ptr req);

typedef CORBA_SEQUENCE <CORBA_TypeCode_ptr> CORBA_ExceptionList;
typedef CORBA_ExceptionList *CORBA_ExceptionList_ptr;

extern const IID IID_CORBA_Request;

class ACE_Svc_Export CORBA_Request : public IUnknown
{
public:
  // XXX these should not be inlined

  CORBA_Object_ptr target (void) const { return _target; }
  const CORBA_Char *operation (void) const { return _opname; }
  CORBA_NVList_ptr arguments (void) { return _args; }
  CORBA_NamedValue_ptr result (void) { return _result; }
  CORBA_ExceptionList_ptr exceptions (void) { return &_exceptions; }
  CORBA_Environment *env (void) { return &_env; }

  void invoke (void);
  void send_oneway (void);

  // Stuff required for COM IUnknown support

  ULONG __stdcall AddRef (void);
  ULONG __stdcall Release (void);
  HRESULT __stdcall QueryInterface (REFIID riid,
				    void **ppv);

private:
  friend class CORBA_Object;

  CORBA_Request (CORBA_Object_ptr obj,
		 const CORBA_Char *op,
		 CORBA_NVList_ptr args,
		 CORBA_NamedValue_ptr result,
		 CORBA_Flags flags);

  CORBA_Request (CORBA_Object_ptr obj,
		 const CORBA_Char *op);

  virtual ~CORBA_Request (void);

  CORBA_Object_ptr _target;
  const CORBA_Char *_opname;
  CORBA_NVList_ptr _args;
  CORBA_NamedValue_ptr _result;
  CORBA_Flags _flags;
  CORBA_Environment _env;
  CORBA_ExceptionList _exceptions;

  ACE_Thread_Mutex lock_;
  u_int _refcount;
};

#endif /* TAO_REQUEST_H */
