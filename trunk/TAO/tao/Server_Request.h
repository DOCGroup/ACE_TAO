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

#ifndef TAO_CORBA_SERVER_REQUEST_H
#define TAO_CORBA_SERVER_REQUEST_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_CORBA == 0)

#include "tao/TAO_Server_Request.h"
#include "tao/Context.h"

class TAO_Export CORBA_ServerRequest
{
  // = TITLE
  //    Class representing the CORBA ServerRequest pseudo-object.
  //
  // = DESCRIPTION
  //    Instantiated by the POA for DSI requests and passed up
  //    to the application. Contains a reference to the instance
  //    of TAO_ServerRequest that is passed up to the POA from
  //    the ORB.
public:
  CORBA_ServerRequest (TAO_ServerRequest &orb_server_request);
  // Constructor.

  ~CORBA_ServerRequest (void);
  // Destructor.

  static CORBA_ServerRequest *_duplicate (CORBA_ServerRequest *req);
  // The duplicate method for Pseudo Objects.

  static CORBA_ServerRequest *_nil (void);
  // The standard _nil method on pseudo objects.

  void arguments (CORBA::NVList_ptr &list,
                  CORBA_Environment &ACE_TRY_ENV =
                    TAO_default_environment ());
  // Implementation uses this to provide the ORB with the operation's
  // parameter list ... on return, their values are available; the
  // list fed in has typecodes and (perhap) memory assigned.

  void set_result (const CORBA::Any &value,
                   CORBA_Environment &ACE_TRY_ENV =
                     TAO_default_environment ());
  // Implementation uses this to provide the operation result
  // ... illegal if exception() was called or params() was not called.
  //
  // XXX Implementation should be able to assume response has been
  // sent when this returns, and reclaim memory it allocated.

  void set_exception (const CORBA::Any &value,
                      CORBA_Environment &ACE_TRY_ENV =
                        TAO_default_environment ());
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

  void dsi_marshal (CORBA_Environment &ACE_TRY_ENV =
                      TAO_default_environment ());
  // Marshal outgoing parameters.

  CORBA::Context_ptr ctx (void) const;
  // Accessor for the Context member.

  void ctx (CORBA::Context_ptr);
  // Mutator for the Context member.

  const char *operation (void) const;
  // Get the operation name.

  void _tao_lazy_evaluation (int lazy_evaluation);
  // Set the lazy evaluation flag.

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA::ServerRequest_ptr _ptr_type;
#endif /* __GNUC__ */
  // Useful for template programming.

private:
  int lazy_evaluation_;
  // If zero then the NVList is evaluated ASAP.

  CORBA::Context_ptr ctx_;
  // Context associated with this request.

  CORBA::NVList_ptr params_;
  // Incoming parameters.

  CORBA::Any_ptr retval_;
  // Return value.

  CORBA::Any_ptr exception_;
  // Any exception which might be raised.

  TAO_ServerRequest &orb_server_request_;
  // Request from the ORB.
};

#if defined (__ACE_INLINE__)
# include "tao/Server_Request.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_MINIMUM_CORBA */

#include "ace/post.h"
#endif /* TAO_CORBA_SERVER_REQUEST_H */
