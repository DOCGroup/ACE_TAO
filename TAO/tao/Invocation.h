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
//   Encapsulate the logic for remote invocations, oneways or twoways.
//
//   THREADING NOTE: Threads should never manipulate another
//   thread's invocations.  In this implementation, all data
//   structures used to represent invocations (and parts of them)
//   are owned by the thread which created them.  Multiple threads
//   may make of course concurrent invocations safely, since the
//   GIOP code is reentrant.
//
// = AUTHOR
//   Carlos O'Ryan <coryan@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_INVOCATION_H
#define TAO_INVOCATION_H

#include "tao/CDR.h"
#include "tao/ORB_Core.h"
#include "tao/GIOP.h"
#include "tao/Any.h"

struct TAO_Exception_Data;
class TAO_Profile;
class TAO_Transport;

class TAO_Export TAO_GIOP_Invocation
{
  // = TITLE
  //   Encapsulates common behavior for both oneway and twoway
  //   invocations.
  //
  // = DESCRIPTION
  //   This class connects (or lookups a connection from the cache) to
  //   the remote server, builds the CDR stream for the Request, send
  //   the CDR stream and expects the response and interprets the
  //   incoming CDR stream.
  //
public:
  // = Initialization and termination methods.
  TAO_GIOP_Invocation (STUB_Object *data,
                       const char *operation,
                       TAO_ORB_Core* orb_core);
  ~TAO_GIOP_Invocation (void);

  void put_param (CORBA::TypeCode_ptr tc,
                  void *value,
                  CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Encodes the value into the undelying CDR stream based on the
  // TypeCode parameter

  TAO_OutputCDR &out_stream (void);
  // return the underlying output stream

protected:
  void start (CORBA::Boolean is_roundtrip,
              TAO_GIOP::Message_Type message_type,
              CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Locates the right Client_Connection_Handler and initializes the
  // CDR stream.
  // The message_type tells how to initialize the output CDR stream

  TAO_GIOP_ReplyStatusType invoke (CORBA::Boolean is_roundtrip,
                                   CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Sends the request, does not wait for the response.

  TAO_GIOP_ReplyStatusType invoke_i (TAO_Profile *profile,
                                     TAO_Transport *transport,
                                     CORBA::Boolean is_roundtrip,
                                     CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // This method implements invoke(), using a pre-determined profile and transport.

  TAO_GIOP_ReplyStatusType close_connection (void);
  // resets the forwarding profile and behaves like
  // we are fowarded (to the same server)

  TAO_GIOP_ReplyStatusType location_forward (TAO_InputCDR &inp_stream,
                                             CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // do the location forwarding, which means exchanging the profile


private:

  CORBA::Boolean
  write_request_header (const TAO_GIOP_ServiceContextList& svc_ctx,
                        CORBA::ULong request_id,
                        CORBA::Boolean is_roundtrip,
                        const TAO_opaque* key,
                        const char* opname,
                        CORBA::Principal_ptr principal);
  CORBA::Boolean
  write_request_header_std (const TAO_GIOP_ServiceContextList& svc_ctx,
                            CORBA::ULong request_id,
                            CORBA::Boolean is_roundtrip,
                            const TAO_opaque* key,
                            const char* opname,
                            CORBA::Principal_ptr principal);
  CORBA::Boolean
  write_request_header_lite (const TAO_GIOP_ServiceContextList& svc_ctx,
                             CORBA::ULong request_id,
                             CORBA::Boolean is_roundtrip,
                             const TAO_opaque* key,
                             const char* opname,
                             CORBA::Principal_ptr principal);
  // Encode the header for the Request, assuming that the GIOP header
  // is already there.
  // TAO support either the standard IIOP request header or a lighter
  // weight version.

protected:
  STUB_Object *data_;
  // The object on which this invocation is going.

  const char *opname_;
  // Name of the operation being invoked.

  CORBA::ULong my_request_id_;
  // Request ID of this operation.

  char buffer [CDR::DEFAULT_BUFSIZE];
  // Buffer used for both the output and input CDR streams, this is
  // "safe" because we only one of the streams at a time.

  TAO_OutputCDR out_stream_;
  // Stream into which the response is placed.

  TAO_ORB_Core* orb_core_;
  // The orb_core context where we make this invocation.
};

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
  // = Initialization and termination methods.
  TAO_GIOP_Twoway_Invocation (STUB_Object *data,
                              const char *operation,
                              TAO_ORB_Core* orb_core);

  void start (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Calls TAO_GIOP_Invocation::start.

  TAO_GIOP_ReplyStatusType invoke (CORBA::ExceptionList &exceptions,
                                   CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Send request, block until any reply comes back, and unmarshal
  // reply parameters as appropriate.

  TAO_GIOP_ReplyStatusType invoke (TAO_Exception_Data *excepts,
                                   CORBA::ULong except_count,
                                   CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Special purpose invoke method used by the interpretive stubs. This
  // accomplishes the same task as the normal invoke except that
  // Exceptions are allocated and decoded here. This keeps the size of
  // the stubs small and abstracts all the common code here.

  void get_value (CORBA::TypeCode_ptr tc,
                  void *value,
                  CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // No CORBA::Context support (deprecated).

  TAO_InputCDR &inp_stream (void);
  // return the underlying input stream

private:
  TAO_InputCDR inp_stream_;
  // Stream into which the request is placed.
};

class TAO_Export TAO_GIOP_Oneway_Invocation : public TAO_GIOP_Invocation
{
  // = TITLE
  //   Sends a oneway request.
  //
public:
  // = Initialization and termination methods.
  TAO_GIOP_Oneway_Invocation (STUB_Object *data,
                              const char *operation,
                              TAO_ORB_Core* orb_core);

  void start (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Call TAO_GIOP_Invocation::start()

  TAO_GIOP_ReplyStatusType invoke (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Send request, without blocking for any response.
};


class TAO_Export TAO_GIOP_Locate_Request_Invocation : public TAO_GIOP_Invocation
{
  // = TITLE
  //   Sends a locate request.
  //
public:
  // = Initialization and termination methods.
  TAO_GIOP_Locate_Request_Invocation (STUB_Object *data,
                                      TAO_ORB_Core* orb_core);

  void start (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Calls TAO_GIOP_Invocation::start.

  TAO_GIOP_ReplyStatusType invoke (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Send request, without blocking for any response.

private:
  TAO_InputCDR inp_stream_;
  // Stream into which the request is placed.
};


#if defined (__ACE_INLINE__)
# include "tao/Invocation.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_INVOCATION_H */
