// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Invocation.h
//
// = DESCRIPTION
//    Encapsulate the logic for remote invocations, oneways or
//    twoways.
//
//   THREADING NOTE: Threads should never manipulate another
//   thread's invocations.  In this implementation, all data
//   structures used to represent invocations (and parts of them)
//   are owned by the thread which created them.  Multiple threads
//   may make of course concurrent invocations safely, since the
//   GIOP code is reentrant.
//
// = AUTHOR
//    Carlos O'Ryan <coryan@cs.wustl.edu> and Alexander Babu Arulanthu
//    <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_INVOCATION_H
#define TAO_INVOCATION_H

#include "tao/CDR.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB_Core.h"
#include "tao/GIOP.h"
#include "tao/Any.h"
#include "tao/Reply_Dispatcher.h"

struct TAO_Exception_Data;
class TAO_Profile;
class TAO_Transport;

enum TAO_Invoke_Status
{
  TAO_INVOKE_OK,
  // invoke() call successful.

  TAO_INVOKE_RESTART,
  // The request must be restarted, a temporary failure has ocurred.

  TAO_INVOKE_EXCEPTION
  // An exception was raised.
};

// ****************************************************************

class TAO_Export TAO_GIOP_Invocation
{
  // = TITLE
  //     Encapsulates common behavior for both oneway and twoway
  //     invocations.
  //
  // = DESCRIPTION
  //     This class connects (or lookups a connection from the cache)
  //     to the remote server, builds the CDR stream for the Request,
  //     send the CDR stream and expects the response and interprets
  //     the incoming CDR stream.

public:
  TAO_GIOP_Invocation (TAO_Stub *data,
                       const char *operation,
                       TAO_ORB_Core* orb_core);
  // Constructor.

  virtual ~TAO_GIOP_Invocation (void);
  // This destructor is virtual so that the derived synchronous
  // invocation classes can call <idle> method on the <Transport>, if
  // they want to. All the synchronous invocations <idle> the
  // Transport, but asynchronous invocations do not do that.

  void put_param (CORBA::TypeCode_ptr tc,
                  void *value,
                  CORBA_Environment &ACE_TRY_ENV =
                        TAO_default_environment ());
  // Encodes the value into the undelying CDR stream based on the
  // TypeCode parameter.

  IOP::ServiceContextList& request_service_info (void);
  // Accessor to the request ServiceContextList.  Only valid when
  // sending a request message.

  TAO_OutputCDR &out_stream (void);
  // Return the underlying output stream.

protected:
  void start (CORBA_Environment &ACE_TRY_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Establishes a connection to the remote server, initializes
  // the GIOP and Request headers in the output CDR.
  // The <message_size> field of the GIOP header is left blank and
  // must be filled later.
  // The function only returns once a connection has been succesfully
  // established *OR* all profiles have been tried.  In that case it
  // raises the CORBA::TRANSIENT exception.

  int invoke (CORBA::Boolean is_roundtrip,
              CORBA_Environment &ACE_TRY_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Sends the request, does not wait for the response.
  // Returns TAO_INVOKE_RESTART if the write call failed and the
  // request must be re-attempted.
  // Notice that the same profile is tried again because it may be
  // that the server closed the connection simply to release
  // resources.

  int close_connection (void);
  // resets the forwarding profile and behaves like we are fowarded
  // (to the same server)

  int location_forward (TAO_InputCDR &inp_stream,
                        CORBA_Environment &ACE_TRY_ENV =
                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Helper method, the response for a Request or LocateRequest was a
  // LOCATION_FORWARD or TAO_GIOP_OBJECT_FORWARD.
  // In any case we must demarshal the object reference and setup the
  // profiles.
  // It returns TAO_INVOKE_RESTART unless an exception is raised.

protected:
  TAO_Stub *stub_;
  // The object on which this invocation is going.

  const char *opname_;
  // Name of the operation being invoked.

  CORBA::ULong request_id_;
  // Request ID of this operation.

  char buffer [ACE_CDR::DEFAULT_BUFSIZE];
  // Buffer used for both the output and input CDR streams, this is
  // "safe" because we only one of the streams at a time.

  TAO_OutputCDR out_stream_;
  // Stream into which the response is placed.

  TAO_ORB_Core* orb_core_;
  // The orb_core context where we make this invocation.

  TAO_Transport *transport_;
  // This invocation is using this transport, may change...

  TAO_Profile *profile_;
  // This invocation is using this transport, may change...

  IOP::ServiceContextList request_service_info_;
  // The ServiceContextList sent to the server side.  Only valid
  // when sending a request.

  ACE_Time_Value max_wait_time_value_;
  ACE_Time_Value *max_wait_time_;
  // The timeout remaining for this request, it is initialized in
  // start() and updated as required.
};

// ****************************************************************

class TAO_Export TAO_GIOP_Twoway_Invocation : public TAO_GIOP_Invocation
{
  // = TITLE
  //   Sends a two-way request, and expects the reply.
  //
  // = DESCRIPTION
  //   This class connects (or lookups a connection from the cache) to
  //   the remote server, builds the CDR stream for the Request, send
  //   the CDR stream and expects the response and interprets the
  //   incoming CDR stream.
  //
public:
  TAO_GIOP_Twoway_Invocation (TAO_Stub *data,
                              const char *operation,
                              TAO_ORB_Core* orb_core);
  // Constructor.

  ~TAO_GIOP_Twoway_Invocation (void);
  // Destructor.

  void start (CORBA_Environment &TAO_IN_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Calls TAO_GIOP_Invocation::start.

  int invoke (CORBA::ExceptionList &exceptions,
              CORBA_Environment &ACE_TRY_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,CORBA::UnknownUserException));
  // Send request, block until any reply comes back, and unmarshal
  // reply parameters as appropriate.

  int invoke (TAO_Exception_Data *excepts,
              CORBA::ULong except_count,
              CORBA_Environment &ACE_TRY_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::Exception));
  // Special purpose invoke method used by the interpretive stubs. This
  // accomplishes the same task as the normal invoke except that
  // Exceptions are allocated and decoded here. This reduces the
  // footprint of the generated stubs.

  void get_value (CORBA::TypeCode_ptr tc,
                  void *value,
                  CORBA_Environment &ACE_TRY_ENV =
                        TAO_default_environment ());
  // No CORBA::Context support (deprecated).

 IOP::ServiceContextList& reply_service_info (void);
  // Accessor to the reply ServiceContextList.

  TAO_InputCDR &inp_stream (void);
  // return the underlying input stream

private:
  int invoke_i (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Implementation of the invoke() methods, handles the basic
  // send/reply code and the system exceptions.

private:
  // TAO_GIOP_Message_State message_state_;
  // Stream into which the reply is placed.

  TAO_Synch_Reply_Dispatcher rd_;
  // Reply dispatcher for the current synchronous invocation.
};

// ****************************************************************

class TAO_Export TAO_GIOP_Oneway_Invocation : public TAO_GIOP_Invocation
{
  // = TITLE
  //   Sends a oneway request.
  //
public:
  TAO_GIOP_Oneway_Invocation (TAO_Stub *data,
                              const char *operation,
                              TAO_ORB_Core* orb_core);
  // Constructor.

  ~TAO_GIOP_Oneway_Invocation (void);
  // Destructor.

  void start (CORBA_Environment &TAO_IN_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Call TAO_GIOP_Invocation::start()

  int invoke (CORBA_Environment &ACE_TRY_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Send request, without blocking for any response.
};

// ****************************************************************

class TAO_Export TAO_GIOP_Locate_Request_Invocation : public TAO_GIOP_Invocation
{
  // = TITLE
  //   Sends a locate request.
  //
public:
  TAO_GIOP_Locate_Request_Invocation (TAO_Stub *data,
                                      TAO_ORB_Core* orb_core);
  // Constructor.

  ~TAO_GIOP_Locate_Request_Invocation (void);
  // Destructor.

  void start (CORBA_Environment &TAO_IN_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Calls TAO_GIOP_Invocation::start.

  int invoke (CORBA_Environment &ACE_TRY_ENV =
                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Send request, without blocking for any response.

  TAO_InputCDR &inp_stream (void);
  // return the underlying input stream

private:
  // TAO_GIOP_Message_State message_state_;
  // Stream into which the request is placed.

  TAO_Synch_Reply_Dispatcher rd_;
  // Reply dispatcher for the current synchronous invocation.
};

#if defined (__ACE_INLINE__)
# include "tao/Invocation.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_INVOCATION_H */
