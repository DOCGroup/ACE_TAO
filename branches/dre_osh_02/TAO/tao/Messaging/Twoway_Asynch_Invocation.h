// This may look like C, but it's really -*- C++ -*-
//=============================================================================
/**
 *  @file Twoway_Asynch_Invocation.h
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


#ifndef TAO_TWOWAY_ASYNCH_INVOCATION_H
#define TAO_TWOWAY_ASYNCH_INVOCATION_H
#include "ace/pre.h"

#include "Messaging.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Asynch_Reply_Dispatcher.h"
#include "tao/Asynch_Invocation.h"

#if (TAO_HAS_AMI_CALLBACK == 1)

/**
 * @class TAO_GIOP_Twoway_Asynch_Invocation
 *
 * @brief Sends a two-way request does not expect the reply.  This
 *        class connects (or lookups a connection from the cache) to
 *        the remote server, builds the CDR stream for the Request,
 *        send the CDR stream and returns.
 */
class TAO_Messaging_Export TAO_GIOP_Twoway_Asynch_Invocation
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
  virtual int invoke_i (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#endif /* (TAO_HAS_AMI_CALLBACK == 1*/

#if defined (__ACE_INLINE__)
#  include "Twoway_Asynch_Invocation.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_TWOWAY_ASYNCH_INVOCATION_H */
