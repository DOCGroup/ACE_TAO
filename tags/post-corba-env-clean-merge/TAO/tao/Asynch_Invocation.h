// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file   Asynch_Invocation.h
 *
 *  $Id$
 *
 *  Encapsulate the logic for remote Asynchronous Invocations.
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_ASYNCH_INVOCATION_H
#define TAO_ASYNCH_INVOCATION_H
#include "ace/pre.h"

#include "tao/Invocation.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Asynch_Reply_Dispatcher.h"

#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)
#include "tao/MessagingC.h"
#endif /* (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1) == 0 */

/**
 * @class TAO_GIOP_Asynch_Invocation
 *
 * @brief Base class for TAO_GIOP_Twoway_Asynch_Invocation and
 *  TAO_GIOP_DII_Deferred_Invocation.
 */
class TAO_Export TAO_GIOP_Asynch_Invocation
  : public TAO_GIOP_Invocation
{
public:

  /// Default constructor. This should never get called, it is here
  /// only to appease older versions of g++.
  TAO_GIOP_Asynch_Invocation (void);

  /// Constructor.
  TAO_GIOP_Asynch_Invocation (TAO_Stub *stub,
                              const char *operation,
                              CORBA::ULong opname_len,
                              CORBA::Boolean argument_flag,
                              TAO_ORB_Core *orb_core,
                              int byte_order = TAO_ENCAP_BYTE_ORDER);

  /// Send request, without blocking until any reply comes back.
  virtual int invoke (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Establishes a connection to the remote server, initializes
  /// headers etc.
  void start (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Must be overridden.
  virtual int invoke_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  /// Reply dispatcher for the current asynchronous invocation.
  TAO_Asynch_Reply_Dispatcher_Base *rd_;
};

#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

/**
 * @class TAO_GIOP_Twoway_Asynch_Invocation
 *
 * @brief Sends a two-way request does not expect the reply.  This
 *        class connects (or lookups a connection from the cache) to
 *        the remote server, builds the CDR stream for the Request,
 *        send the CDR stream and returns.
 */
class TAO_Export TAO_GIOP_Twoway_Asynch_Invocation
  : public TAO_GIOP_Asynch_Invocation
{

public:

  /// Constructor.
  TAO_GIOP_Twoway_Asynch_Invocation (
      TAO_Stub *stub,
      const char *operation,
      CORBA::ULong opname_len_,
      CORBA::Boolean argument_flag,
      TAO_ORB_Core* orb_core,
      const TAO_Reply_Handler_Skeleton &reply_handler_skel,
      Messaging::ReplyHandler_ptr reply_handler_ptr
    );

protected:

  /// Implementation of the invoke() methods, handles the basic
  /// send/reply code and the system exceptions.
  virtual int invoke_i (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#endif /* (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1) == 0 */

#if defined (__ACE_INLINE__)
#  include "tao/Asynch_Invocation.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ASYNCH_INVOCATION_H */
