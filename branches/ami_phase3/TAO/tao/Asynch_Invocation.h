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

#include "tao/Invocation.h"
#include "tao/MessagingC.h"

class TAO_Export TAO_GIOP_Asynch_Invocation : public TAO_GIOP_Invocation
{
  // = TITLE
  //     Encapsulates common behavior for Asynch_Invocations. 
  //
  // = DESCRIPTION
  //
public:

  // = Initialization and termination methods.
  
  TAO_GIOP_Asynch_Invocation (TAO_Stub *data,
                              const char *operation,
                              TAO_ORB_Core* orb_core);
  // Constructor.

  ~TAO_GIOP_Asynch_Invocation (void);
  // Destructor.
};

// ****************************************************************

class TAO_Export TAO_GIOP_Twoway_Asynch_Invocation : public TAO_GIOP_Asynch_Invocation
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
                                     TAO_ORB_Core* orb_core,
                                     const TAO_Reply_Handler_Skeleton &reply_handler_skel,
                                     Messaging::ReplyHandler_ptr reply_handler_ptr);
  // Constructor.

  void start (CORBA_Environment &TAO_IN_ENV =
              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Calls TAO_GIOP_Asynch_Invocation::start.

  int invoke (CORBA::ExceptionList &exceptions,
              CORBA_Environment &TAO_IN_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,CORBA::UnknownUserException));
  // Send request, block until any reply comes back, and unmarshal
  // reply parameters as appropriate.

  int invoke (TAO_Exception_Data *excepts,
              CORBA::ULong except_count,
              CORBA_Environment &TAO_IN_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::Exception));
  // Special purpose invoke method used by the interpretive stubs. This
  // accomplishes the same task as the normal invoke except that
  // Exceptions are allocated and decoded here. This reduces the
  // footprint of the generated stubs.

  void get_value (CORBA::TypeCode_ptr tc,
                  void *value,
                  CORBA_Environment &TAO_IN_ENV =
                        TAO_default_environment ());
  // No CORBA::Context support (deprecated).

  TAO_InputCDR &inp_stream (void);
  // Return the underlying input stream.

private:
  int invoke_i (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Implementation of the invoke() methods, handles the basic
  // send/reply code and the system exceptions.

private:
  TAO_GIOP_Message_State *message_state_;
  // Stream into which the reply is placed.

  TAO_Asynch_Reply_Dispatcher *rd_;
  // Reply dispatcher for the current synchronous Asynch_Invocation.
};

#if defined (__ACE_INLINE__)
# include "tao/Asynch_Invocation.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_ASYNCH_INVOCATION_H */
