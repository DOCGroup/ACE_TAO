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

#if !defined (TAO_INVOCATION_H)
#  define TAO_INVOCATION_H

#include "tao/CDR.h"

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
  TAO_GIOP_Invocation (IIOP_Object *data,
		       const char *operation);
  ~TAO_GIOP_Invocation (void);

  void put_param (CORBA::TypeCode_ptr tc,
                  void *value,
                  CORBA::Environment &env);
  // Encodes the value into the undelying CDR stream based on the
  // TypeCode parameter

  TAO_OutputCDR &out_stream (void);
  // return the underlying output stream

protected:
  void start (CORBA::Boolean is_roundtrip,
	      CORBA::Environment &env);
  // Locates the right Client_Connection_Handler and initializes the
  // CDR stream.

  TAO_GIOP_ReplyStatusType invoke (CORBA::Boolean is_roundtrip,
				   CORBA::Environment &env);
  // Sends the request, does not wait for the response.

protected:
  IIOP_Object *data_;
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

  TAO_Client_Connection_Handler *handler_;
  // The handler for the client's connection.
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
  TAO_GIOP_Twoway_Invocation (IIOP_Object *data,
			      const char *operation);

  void start (CORBA::Environment &env);
  // Calls TAO_GIOP_Invocation::start.

  TAO_GIOP_ReplyStatusType invoke (CORBA::ExceptionList &exceptions,
                                   CORBA::Environment &env);
  // Send request, block until any reply comes back, and unmarshal
  // reply parameters as appropriate.

  TAO_GIOP_ReplyStatusType invoke (TAO_Exception_Data *excepts,
                                   CORBA::ULong except_count,
                                   CORBA::Environment &env);
  // Special purpose invoke method used by the stubs. This
  // accomplishes the same task as the normal invoke except that
  // Exceptions are allocated and decoded here. This keeps the size of
  // the stubs small and abstracts all the common code here.

  void get_value (CORBA::TypeCode_ptr tc,
                  void *value,
                  CORBA::Environment &env);
  // No CORBA::Context support (deprecated).

  TAO_InputCDR &inp_stream (void);
  // return the underlying input stream

private:
  TAO_GIOP_ReplyStatusType location_forward (CORBA::Environment &env);
  // do the location forwarding, which means exchanging the profile

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
  TAO_GIOP_Oneway_Invocation (IIOP_Object *data,
			      const char *operation);

  void start (CORBA::Environment &env);
  // Call TAO_GIOP_Invocation::start()

  TAO_GIOP_ReplyStatusType invoke (CORBA::Environment &env);
  // Send request, without blocking for any response.
};

#if defined (__ACE_INLINE__)
# include "tao/Invocation.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_INVOCATION_H */
