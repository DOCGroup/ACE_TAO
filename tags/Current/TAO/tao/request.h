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

class TAO_Export CORBA_Request
{
public:
  CORBA::Object_ptr target (void) const { return target_; }
  // Return the target of this request.
  
  const CORBA::Char *operation (void) const { return opname_; }
  // Return the operation name for the request.
  
  CORBA::NVList_ptr arguments (void) { return args_; }
  // Return the arguments for the request.
  
  CORBA::NamedValue_ptr result (void) { return result_; }
  // Return the result for the request.
  
  CORBA::ExceptionList_ptr exceptions (void) { return &exceptions_; }
  // Return the exceptions resulting from this request.
  
  CORBA::Environment *env (void) { return &env_; }
  // Return the <Environment> for this request.

  void invoke (void);
  // Perform method resolution and invoke an appropriate method. If
  // the method returns successfully, its result is placed in the
  // result argument specified on <create_request>. The behavior is
  // undefined if this <Request> has already been used with a previous
  // call to <invoke>, <send>, or <send_multiple_requests>.
  
  void send_oneway (void);
  // Send a oneway request.

  // = Stuff required for COM IUnknown support

  CORBA::ULong AddRef (void);
  CORBA::ULong Release (void);

private:
  friend class CORBA_Object;

  CORBA_Request (CORBA::Object_ptr obj,
		 const CORBA::Char *op,
		 CORBA::NVList_ptr args,
		 CORBA::NamedValue_ptr result,
		 CORBA::Flags flags);

  CORBA_Request (CORBA::Object_ptr obj,
		 const CORBA::Char *op);

  ~CORBA_Request (void);

  CORBA::Object_ptr target_;
  const CORBA::Char *opname_;
  CORBA::NVList_ptr args_;
  CORBA::NamedValue_ptr result_;
  CORBA::Flags flags_;
  CORBA::Environment env_;
  CORBA::ExceptionList exceptions_;

  CORBA::ULong refcount_;
};

#endif /* TAO_REQUEST_H */
