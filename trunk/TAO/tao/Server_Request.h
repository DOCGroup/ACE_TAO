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
#include "ace/pre.h"
#define TAO_SERVER_REQUEST_H

#include "tao/corbafwd.h"
#include "tao/IOPC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object_KeyC.h"
#include "tao/Object.h"

class TAO_POA;
class TAO_ORB_Core;
class TAO_Transport;
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
                          CORBA_Environment &ACE_TRY_ENV =
                              TAO_default_environment ()) = 0;
  // Implementation uses this to provide the ORB with the operation's
  // parameter list ... on return, their values are available; the
  // list fed in has typecodes and (perhap) memory assigned.

  virtual void set_result (const CORBA::Any &value,
                           CORBA_Environment &ACE_TRY_ENV =
                               TAO_default_environment ()) = 0;
  // Implementation uses this to provide the operation result
  // ... illegal if exception() was called or params() was not called.
  //
  // XXX Implementation should be able to assume response has been
  // sent when this returns, and reclaim memory it allocated.

  virtual void set_exception (const CORBA::Any &value,
                              CORBA_Environment &ACE_TRY_ENV =
                                  TAO_default_environment ()) = 0;
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

  virtual void dsi_marshal (CORBA_Environment &ACE_TRY_ENV =
                                TAO_default_environment ()) = 0;
  // marshal outgoing parameters. Used by DSI

#endif /* TAO_HAS_MINIMUM_CORBA */

  virtual const char *operation (void) const = 0;
  // get the operation name

  virtual unsigned int operation_length (void) const = 0;
  // get the length of the operation name

  virtual CORBA::Object_ptr objref (CORBA_Environment &ACE_TRY_ENV =
                                      TAO_default_environment ()) = 0;
  // Return the object reference of the request.

  virtual void init_reply (CORBA_Environment &ACE_TRY_ENV =
                               TAO_default_environment ()) = 0;
  // Start a Reply message.

  virtual IOP::ServiceContextList &service_info (void) = 0;
  // Accessor to the underlying ServiceContextList for request/reply
  // message.

  // To invoke interceptors and handle System Exceptions at the lowest
  // level, a method returning the request_id_ is needed.  However,
  // request_id is GIOP specific, so I am not sure if this is the
  // right place to put it.  (nw)
  virtual CORBA::ULong request_id (void) = 0;

  //  CORBA::Context_ptr ctx (void) = 0;
  // return the context pointer

  // = Extensions.

  virtual TAO_POA *oa (void) = 0;
  // get the Object Adapter

  virtual CORBA::ORB_ptr  orb (void) = 0;
  // get the underlying ORB

  virtual TAO_InputCDR &incoming (void) = 0;
  // Retrieve the incoming stream.

  virtual TAO_OutputCDR &outgoing (void) = 0;
  // Retrieve the outgoing stream.

  virtual CORBA::Boolean response_expected (void) const = 0;
  // is the response expected

  virtual CORBA::Boolean sync_with_server (void) const = 0;
  // Should we return before dispatching the servant?

  virtual void _tao_lazy_evaluation (int lazy_evaluation) = 0;
  // Set the lazy evaluation flag

  virtual void send_no_exception_reply (TAO_Transport *transport) = 0;
  // Used with reliable oneway requests.

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA::ServerRequest_ptr _ptr_type;
#endif /* __GNUC__ */
  // Useful for template programming.

};

#if defined (__ACE_INLINE__)
# include "tao/Server_Request.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_SERVER_REQUEST_H */
