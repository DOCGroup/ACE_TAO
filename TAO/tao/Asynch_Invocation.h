// This may look like C, but it's really -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Asynch_Invocation.h
//
// = DESCRIPTION
//    Encapsulate the logic for remote Asynchronous Invocations.
//
// = AUTHOR
//   Carlos O'Ryan <coryan@cs.wustl.edu> and Alexander Babu Arulanthu
//   <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ASYNCH_INVOCATION_H
#define TAO_ASYNCH_INVOCATION_H
#include "ace/pre.h"

#include "tao/Invocation.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/MessagingC.h"

#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

class TAO_Export TAO_GIOP_Twoway_Asynch_Invocation : public TAO_GIOP_Invocation
{
  // = TITLE
  //   Sends a two-way request does not expect the reply.
  //
  // = DESCRIPTION
  //   This class connects (or lookups a connection from the cache) to
  //   the remote server, builds the CDR stream for the Request, send
  //   the CDR stream and returns.
  //
public:
  TAO_GIOP_Twoway_Asynch_Invocation (TAO_Stub *data,
                                     const char *operation,
                                     CORBA::ULong opname_len_,
                                     TAO_ORB_Core* orb_core,
                                     const TAO_Reply_Handler_Skeleton &reply_handler_skel,
                                     Messaging::ReplyHandler_ptr reply_handler_ptr);
  // Constructor.

  void start (CORBA_Environment &TAO_IN_ENV =
              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Calls TAO_GIOP_Asynch_Invocation::start.

  int invoke (CORBA_Environment &TAO_IN_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Send request, block until any reply comes back, and unmarshal
  // reply parameters as appropriate.


  //   TAO_InputCDR &inp_stream (void);
  //   // Return the underlying input stream.
  //

  const IOP::ServiceContextList& reply_service_info (void) const;
  // Accessor to the reply ServiceContextList.

private:
  int invoke_i (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Implementation of the invoke() methods, handles the basic
  // send/reply code and the system exceptions.

private:
  TAO_Asynch_Reply_Dispatcher *rd_;
  // Reply dispatcher for the current synchronous Asynch_Invocation.
};

#endif /* TAO_HAS_AMI_CALLBACK == 1 || TAO_HAS_AMI_POLLER == 1 */


//***********************************************************************

#if (TAO_HAS_MINIMUM_CORBA == 0)

class TAO_Export TAO_GIOP_DII_Deferred_Invocation : public TAO_GIOP_Invocation
{
  // = TITLE
  //   Sends a two-way request does not expect the reply.
  //
  // = DESCRIPTION
  //   This class connects (or lookups a connection from the cache) to
  //   the remote server, builds the CDR stream for the Request, send
  //   the CDR stream and returns.
  //
public:
  TAO_GIOP_DII_Deferred_Invocation (TAO_Stub *data,
                                    TAO_ORB_Core* orb_core,
                                    const CORBA::Request_ptr req);
  // Constructor.

  void start (CORBA_Environment &TAO_IN_ENV =
              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Calls TAO_GIOP_Asynch_Invocation::start.

  int invoke (CORBA_Environment &TAO_IN_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Send request, block until any reply comes back, and unmarshal
  // reply parameters as appropriate.


  //   TAO_InputCDR &inp_stream (void);
  //   // Return the underlying input stream.
  //

  const IOP::ServiceContextList& reply_service_info (void) const;
  // Accessor to the reply ServiceContextList.

private:
  int invoke_i (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Implementation of the invoke() methods, handles the basic
  // send/reply code and the system exceptions.

private:
  TAO_DII_Deferred_Reply_Dispatcher *rd_;
  // Reply dispatcher for the current synchronous Asynch_Invocation.
};

#endif /* TAO_HAS_MINIMUM_CORBA */

#if defined (__ACE_INLINE__)
#  include "tao/Asynch_Invocation.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ASYNCH_INVOCATION_H */
