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
#include "ace/pre.h"
#define TAO_INVOCATION_H

#include "tao/CDR.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB_Core.h"
#include "tao/Any.h"
#include "tao/Reply_Dispatcher.h"
#include "tao/TAOC.h"
#include "tao/operation_details.h"

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
                       CORBA::ULong opname_len,
                       TAO_ORB_Core* orb_core);
  // Constructor.

  virtual ~TAO_GIOP_Invocation (void);
  // This destructor is virtual so that the derived synchronous
  // invocation classes can call <idle> method on the <Transport>, if
  // they want to. All the synchronous invocations <idle> the
  // Transport, but asynchronous invocations do not do that.

  void prepare_header (CORBA::Octet response_flags,
                       CORBA_Environment &ACE_TRY_ENV =
                         TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Initialize the Request header.
  // The <message_size> field of the GIOP header is left blank and
  // must be filled later.
  // The function only returns once a connection has been succesfully
  // established *OR* all profiles have been tried.  In that case it
  // raises the CORBA::TRANSIENT exception.

  IOP::ServiceContextList& service_info (void);
  // Accessor to the request ServiceContextList.  Only valid when
  // sending a request message.

  CORBA::ULong request_id (void);
  // Return the request id of this invocation.

  TAO_OutputCDR &out_stream (void);
  // Return the underlying output stream.

  TAO_Profile *select_profile_based_on_policy (
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Select the profile we will use to in this invocation, based on
  // policies for this object.  (Currently the decision is based on a
  // single Policy, TAO::Client_Priority_Policy.)  I think this
  // function may be more appropriate in TAO_Stub class.
protected:
  void start (CORBA_Environment &ACE_TRY_ENV =
                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Establishes a connection to the remote server, initializes
  // the GIOP headers in the output CDR.

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

  char buffer [ACE_CDR::DEFAULT_BUFSIZE];
  // Buffer used for both the output and input CDR streams, this is
  // "safe" because we only one of the streams at a time.

  TAO_Operation_Details  op_details_;
  // The relevant operation detail
  
  TAO_Target_Specification target_spec_;

  TAO_OutputCDR out_stream_;
  // Stream into which the response is placed.

  TAO_ORB_Core* orb_core_;
  // The orb_core context where we make this invocation.

  TAO_Transport *transport_;
  // This invocation is using this transport, may change...

  TAO_Profile *profile_;
  // This invocation is using this transport, may change...

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
                              CORBA::ULong opname_len,
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

  TAO_InputCDR &inp_stream (void);
  // return the underlying input stream
private:
  int invoke_i (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Implementation of the invoke() methods, handles the basic
  // send/reply code and the system exceptions.

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
                              CORBA::ULong opname_len,
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

  TAO::SyncScope sync_scope (void);
  // Acessor for private member.
private:
  TAO::SyncScope sync_scope_;
  // Our sync scope.
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
  TAO_Synch_Reply_Dispatcher rd_;
  // Reply dispatcher for the current synchronous invocation.
};

#if defined (__ACE_INLINE__)
# include "tao/Invocation.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_INVOCATION_H */
