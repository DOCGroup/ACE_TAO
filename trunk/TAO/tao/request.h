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
#  define TAO_REQUEST_ H

// @@ Please comment everything in this file.

class TAO_Export CORBA_Request : public TAO_IUnknown
{
public:
  // @@ These inlines should be moved into a *.i file.
  CORBA::Object_ptr target (void) const { return target_; }
  const CORBA::Char *operation (void) const { return opname_; }
  CORBA::NVList_ptr arguments (void) { return args_; }
  CORBA::NamedValue_ptr result (void) { return result_; }
  CORBA::ExceptionList_ptr exceptions (void) { return &exceptions_; }
  CORBA::Environment *env (void) { return &env_; }

  void invoke (void);
  void send_oneway (void);

  // Stuff required for COM IUnknown support

  ULONG AddRef (void);
  ULONG Release (void);
  TAO_HRESULT QueryInterface (REFIID riid,
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

  CORBA::Object_ptr target_;
  const CORBA::Char *opname_;
  CORBA::NVList_ptr args_;
  CORBA::NamedValue_ptr result_;
  CORBA::Flags flags_;
  CORBA::Environment env_;
  CORBA::ExceptionList exceptions_;

  ACE_SYNCH_MUTEX lock_;
  u_int refcount_;
};

#endif /* TAO_REQUEST_H */
