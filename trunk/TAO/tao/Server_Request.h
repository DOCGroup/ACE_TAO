// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Server_Request.h
//
// = DESCRIPTION
//    Header file for CORBA's Dynamic Server Skeleton Interface's
//    "Server Request" type.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc. and Chris Cleeland.
//     Modifications by Aniruddha Gokhale based on CORBAv2.2 Feb 98
// ============================================================================

#ifndef TAO_SERVER_REQUEST_H
#define TAO_SERVER_REQUEST_H

#include "tao/corbafwd.h"
#include "tao/Object_KeyC.h"
#include "tao/GIOP.h"
#include "tao/Object.h"

class TAO_POA;
class TAO_ORB_Core;

class TAO_Param_Data_Skel
{
  // = TITLE
  //   Description of a single parameter.
  //
  // = DESCRIPTION
  //
  //   If value_size is nonzero for OUT, INOUT, or RETURN parameters,
  //   it's (a) an indicator that the ORB returns a pointer-to-value
  //   for this parameter, and also (b) is the size of the top-level
  //   of the value that's returned (e.g. ignoring nested sequence
  //   buffers).  That is, it moves CPU cycles from runtime -- some
  //   calls to tc->size() -- to compile time where they're
  //   cheap/free.
  //
  //   It _must_ only be set for cases where the ORB allocates the
  //   return value, which must then be ORB::free()d ... e.g. where
  //   the value is a pointer to data such as a struct, sequence, or
  //   union.  (The CORBA C++ mapping doesn't require that for all
  //   "out" structs; only those of "variable size".)  If this value
  //   is nonzero, the value passed to do_static_call() must be the address
  //   of a pointer.
public:
  CORBA::TypeCode_ptr tc;
  // Type of param.

  CORBA::ULong mode;
  // Its mode.

  CORBA::Boolean own;
  // whether we own it or not
};

class TAO_Call_Data_Skel
{
  // = TITLE
  //   Descriptions of operations, as used by the stub interpreter.
  //   Only interpretive marshaling/unmarshaling is used, and the
  //   stubs don't know what particular on-the-wire protocol is being
  //   used.
  //
  // = DESCRIPTION
  //   When using C++ exceptions, many C++ compilers will require the
  //   use of compiled code throw the exception.  As binary standards
  //   for exception throwing evolve, it may become practical to
  //   interpretively throw exceptions.
public:
  const char *opname;
  // Operation name.

  CORBA::Boolean is_roundtrip;
  // !oneway

  // When constructing tables of parameters, put them in the same
  // order they appear in the IDL spec: return value, then parameters
  // left to right.  Other orders may produce illegal IIOP protocol
  // messages.

  CORBA::ULong param_count;
  // # parameters.

  const TAO_Param_Data_Skel *params;
  // Their descriptions.

};

class TAO_Export CORBA_ServerRequest
{
  // = TITLE
  //    Class representing a CORBA ServerRequest object.
  //
  // = DESCRIPTION
  //   This is not supposed to be IIOP-specific, or to expose quite so
  //   many implementation details, but right now does.
public:
  static CORBA_ServerRequest *_duplicate (CORBA_ServerRequest *req);
  // the duplicate method for Pseudo Objects

  static CORBA_ServerRequest *_nil (void);
  // the standard _nil method on pseudo objects

#if !defined (TAO_HAS_MINIMUM_CORBA)

  virtual void arguments (CORBA::NVList_ptr &list,
                          CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) = 0;
  // Implementation uses this to provide the ORB with the operation's
  // parameter list ... on return, their values are available; the
  // list fed in has typecodes and (perhap) memory assigned.

  virtual void set_result (const CORBA::Any &value,
                           CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) = 0;
  // Implementation uses this to provide the operation result
  // ... illegal if exception() was called or params() was not called.
  //
  // XXX Implementation should be able to assume response has been
  // sent when this returns, and reclaim memory it allocated.

  virtual void set_exception (const CORBA::Any &value,
                              CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) = 0;
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

  virtual void dsi_marshal (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) = 0;
  // marshal outgoing parameters. Used by DSI

#endif /* TAO_HAS_MINIMUM_CORBA */

  virtual const char *operation (void) const = 0;
  // get the operation name

  virtual unsigned int operation_length (void) const = 0;
  // get the length of the operation name

  virtual void init_reply (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()) = 0;
  // Start a Reply message.

  //  CORBA::Context_ptr ctx (void) = 0;
  // return the context pointer

  // = Extensions.

  virtual TAO_POA *oa (void) = 0;
  // get the Object Adapter

  virtual CORBA::ORB_ptr  orb (void) = 0;
  // get the underlying ORB

  virtual void demarshal (CORBA_Environment &orb_env,
                          const TAO_Call_Data_Skel *info,
                          ...) = 0;
  // demarshal incoming parameters

  virtual void marshal (CORBA_Environment &orb_env,
                        //                        CORBA_Environment &skel_env,
                        const TAO_Call_Data_Skel *info,
                        ...) = 0;
  // marshal outgoing parameters

  virtual TAO_InputCDR &incoming (void) = 0;
  // Retrieve the incoming stream.

  virtual TAO_OutputCDR &outgoing (void) = 0;
  // Retrieve the outgoing stream.

  virtual CORBA::Boolean response_expected (void) const = 0;
  // is the response expected

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA::ServerRequest_ptr _ptr_type;
#endif /* __GNUC__ */
  // Useful for template programming.

};

class TAO_Export IIOP_ServerRequest : public CORBA_ServerRequest
{
  // = TITLE
  //    Class representing an IIOP ServerRequest object.
public:
  // = Initialization and termination methods.
  IIOP_ServerRequest (TAO_InputCDR &input,
                      TAO_OutputCDR &output,
                      TAO_ORB_Core *orb_core,
                      CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Constructor
  IIOP_ServerRequest (CORBA::ULong &request_id,
                      CORBA::Boolean &response_expected,
                      TAO_ObjectKey &object_key,
                      const ACE_CString &operation,
                      TAO_OutputCDR &output,
                      TAO_ORB_Core *orb_core,
                      CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  virtual ~IIOP_ServerRequest (void);
  // Destructor.

#if !defined (TAO_HAS_MINIMUM_CORBA)

  // = General ServerRequest operations
  void arguments (CORBA::NVList_ptr &list,
                  CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  void set_result (const CORBA::Any &value,
                   CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  void set_exception (const CORBA::Any &value,
                      CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  virtual void dsi_marshal (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // does the marshaling of outgoing parameters and is used by the DSI based
  // scheme

#endif /* TAO_HAS_MINIMUM_CORBA */

  // = Request attributes.

  const char *operation (void) const;
  // return the operation name

  unsigned int operation_length (void) const;
  // return the legnth of the operation

  //  CORBA::Context_ptr ctx (void);
  // return the context pointer

  // = TAO extensions

  CORBA::ORB_ptr  orb (void);
  // return the underlying ORB

  TAO_POA *oa (void);
  // return the Object Adapter

  //
  // = TAO extensions
  // meant to be used internally.
  //

  virtual void demarshal (CORBA_Environment &orb_env,
                          const TAO_Call_Data_Skel *info,
                          ...);
  // demarshal incoming parameters. Used by the SII skeleton (i.e., the IDL
  // compiler generated skeleton)

  virtual void marshal (CORBA_Environment &orb_env,
                        //                        CORBA_Environment &skel_env,
                        const TAO_Call_Data_Skel *info,
                        ...);
  // marshal outgoing parameters and return value. This is used by the SSI
  // i.e., by the IDL compiler generated skeletons.

  virtual void init_reply (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // start a Reply message

  virtual TAO_InputCDR &incoming (void);
  // Retrieve the incoming stream.

  virtual TAO_OutputCDR &outgoing (void);
  // Retrieve the outgoing stream.

  virtual CORBA::Boolean response_expected (void) const;
  // is the response expected

  virtual CORBA::Principal_ptr principal (void) const;

  virtual const TAO_ObjectKey &object_key (void) const;

  virtual const TAO_GIOP_ServiceContextList &service_info (void) const;

  // The pseudo object methods, not really needed because the class is
  // not in the spec, but we add them for the sake of completeness.
  static IIOP_ServerRequest* _duplicate (IIOP_ServerRequest*);
  static IIOP_ServerRequest* _nil (void);

  // To handle System Exceptions at the lowest level,
  // a method returning the request_id_ is needed.

  CORBA::ULong request_id (void);

  CORBA::Object_ptr forward_location (void);
  // get the forward_location

  CORBA::ULong exception_type (void);
  // get the exception type

private:
   void parse_header (CORBA::Environment &TAO_IN_ENV);
  // Parse the request header and store the result on this object.

  void parse_header_std (CORBA::Environment &TAO_IN_ENV);
  // Parse the standard IIOP request header and store the result on
  // this object.

  void parse_header_lite (CORBA::Environment &TAO_IN_ENV);
  // Parse the lightweight version of the IIOP request header and
  // store the result on this object.

private:
  ACE_CString operation_;
  // Operation name.

  CORBA::Object_var forward_location_;

  TAO_InputCDR *incoming_;
  // Incoming stream.

  TAO_OutputCDR *outgoing_;
  // Outgoing stream.

  CORBA::Boolean response_expected_;
  // is it oneway or twoway

#if !defined (TAO_HAS_MINIMUM_CORBA)

  CORBA::NVList_ptr params_;
  // Incoming parameters.

#endif /* TAO_HAS_MINIMUM_CORBA */

  CORBA::Any_ptr retval_;
  // Return value.

  CORBA::Any_ptr exception_;
  // Any exception which might be raised.

  //  TAO_GIOP_ReplyStatusType exception_type_;
  CORBA::ULong exception_type_;
  // exception type (will be NO_EXCEPTION in the majority of the cases)

  TAO_ORB_Core* orb_core_;
  // A pointer to the ORB Core for the context where the request was
  // created.

  TAO_GIOP_ServiceContextList service_info_;
  // The service context for the request (CORBA Reference?)

  CORBA::ULong request_id_;
  // Unique identifier for a request

  TAO_ObjectKey object_key_;
  // The object key of the destination object.

  CORBA::Principal_ptr requesting_principal_;
  // Identifies the requester
};

#if defined (__ACE_INLINE__)
# include "tao/Server_Request.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_SERVER_REQUEST_H */
