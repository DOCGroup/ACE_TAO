// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Request.h
//
// = DESCRIPTION
//    Header file for CORBA's Dynamic Invocation Interface "Request"
//    type.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
//
// ============================================================================

#if !defined (TAO_REQUEST_H)
#  define TAO_REQUEST_ H

class TAO_Export CORBA_Request
{
  // = TITLE
  //   CORBA_Request
  // = DESCRIPTION
  //   Provides a way to create requests and populate it with parameters for
  //   use in the Dynamic Invocation Interface
public:
  CORBA::Object_ptr target (void) const;
  // Return the target of this request.

  const CORBA::Char *operation (void) const;
  // Return the operation name for the request.

  CORBA::NVList_ptr arguments (void);
  // Return the arguments for the request.

  CORBA::NamedValue_ptr result (void);
  // Return the result for the request.

  CORBA::ExceptionList_ptr exceptions (void);
  // Return the exceptions resulting from this request.

  CORBA::Environment *env (void);
  // Return the <Environment> for this request.

  void invoke (void);
  // Perform method resolution and invoke an appropriate method. If
  // the method returns successfully, its result is placed in the
  // result argument specified on <create_request>. The behavior is
  // undefined if this <Request> has already been used with a previous
  // call to <invoke>, <send>, or <send_multiple_requests>.

  void send_oneway (void);
  // Send a oneway request.

  // = Required for COM IUnknown support
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

private:
  friend class CORBA_Object;

  // following are not allowed

  CORBA_Request (CORBA::Object_ptr obj,
                 const CORBA::Char *op,
                 CORBA::NVList_ptr args,
                 CORBA::NamedValue_ptr result,
                 CORBA::Flags flags);

  CORBA_Request (CORBA::Object_ptr obj,
                 const CORBA::Char *op);

  ~CORBA_Request (void);

  CORBA::Object_ptr target_;
  // target object

  const CORBA::Char *opname_;
  // operation name

  CORBA::NVList_ptr args_;
  // parameter list

  CORBA::NamedValue_ptr result_;
  // result of the operation

  CORBA::Flags flags_;
  // invocation flags

  CORBA::Environment env_;
  // holds exceptions

  CORBA::ExceptionList exceptions_;
  // list of exceptions raised by the operation

  CORBA::ULong refcount_;
  // reference counting
};

#if defined (__ACE_INLINE__)
# include "tao/Request.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_REQUEST_H */
