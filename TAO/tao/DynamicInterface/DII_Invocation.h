// This may look like C, but it's really -*- C++ -*-
// $Id$



// ============================================================================
//
// = LIBRARY
//    DynamicInterface
//
// = DESCRIPTION
//    The DII invocation classes.
//
// = AUTHOR
//    Carlos O'Ryan <coryan@cs.wustl.edu>
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_DII_INVOCATION_H
#define TAO_DII_INVOCATION_H
#include "ace/pre.h"

#include "tao/Invocation.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Asynch_Invocation.h"
#include "Unknown_User_Exception.h"
#include "DII_Reply_Dispatcher.h"
#include "Request.h"

class TAO_GIOP_DII_Invocation : public TAO_GIOP_Twoway_Invocation
{
  // = TITLE
  //    Sends a two-way request using DII.
  //
  // = DESCRIPTION
  //    This class replaces just one method - invoke - of its base
  //    class with one of a slightly different signature. The class
  //    is not exported because it is only instantiated by
  //    CORBA::Request in this library.
  //
public:
  TAO_GIOP_DII_Invocation (TAO_Stub *data,
                           const char *operation,
                           CORBA::ULong opname_len,
                           CORBA::Boolean argument_flag,
                           TAO_ORB_Core *orb_core,
                           CORBA::Request_ptr host,
                           int byte_order = TAO_ENCAP_BYTE_ORDER);
  // Constructor.

  int invoke (CORBA::ExceptionList_ptr exceptions
              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,CORBA::UnknownUserException));
  // Send request, block until any reply comes back, and unmarshal
  // reply parameters as appropriate.

private:
  CORBA::Request_ptr host_;
  // Back pointer to the DII request that created us.
};

class TAO_DynamicInterface_Export TAO_GIOP_DII_Deferred_Invocation
  : public TAO_GIOP_Asynch_Invocation
{
  // = TITLE
  //    Sends a two-way request using DII and does not wait for a reply.
  //
  // = DESCRIPTION
  //    This class connects (or looks up a connection from the cache) to
  //    the remote server, builds the CDR stream for the Request, send
  //    the CDR stream and returns.
  //
public:
  TAO_GIOP_DII_Deferred_Invocation (TAO_Stub *data,
                                    TAO_ORB_Core* orb_core,
                                    CORBA::Boolean argument_flag,
                                    const CORBA::Request_ptr req,
                                    int byte_order = TAO_ENCAP_BYTE_ORDER);
  // Constructor.

  int invoke (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Send request, block until any reply comes back, and unmarshal
  // reply parameters as appropriate.

private:
  int invoke_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Implementation of the invoke() methods, handles the basic
  // send/reply code and the system exceptions.

  TAO_DII_Deferred_Reply_Dispatcher *rd_;
  // Reply dispatcher for the current synchronous Asynch_Invocation.
};

#if defined (__ACE_INLINE__)
#  include "DII_Invocation.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ASYNCH_INVOCATION_H */
