// -*- C++ -*-

//=============================================================================
/**
 *  @file    Invocation.h
 *
 *  $Id$
 *
 *  Encapsulate the logic for remote invocations, oneways or
 *  twoways.
 *
 * THREADING NOTE: Threads should never manipulate another
 * thread's invocations.  In this implementation, all data structures
 * used to represent invocations (and parts of them) are owned by the
 * thread which created them.  Multiple threads may make of course
 * concurrent invocations safely, since the GIOP code is reentrant.
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_INVOCATION_H
#define TAO_INVOCATION_H
#include "ace/pre.h"

#include "CDR.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Stub.h"
#include "Any.h"
#include "Synch_Reply_Dispatcher.h"
#include "TAOC.h"
#include "operation_details.h"
#include "target_specification.h"
#include "Invocation_Endpoint_Selectors.h"

struct TAO_Exception_Data;
class TAO_Transport;

enum TAO_Invoke_Status
{
  /// invoke() call successful.
  TAO_INVOKE_OK,

  /// The request must be restarted, a temporary failure has ocurred.
  TAO_INVOKE_RESTART,

  /// An exception was raised.
  TAO_INVOKE_EXCEPTION
};

// ****************************************************************

/**
 * @class TAO_GIOP_Invocation
 *
 * @brief Encapsulates common behavior for both oneway and twoway
 * invocations.
 *
 * This class connects (or lookups a connection from the cache) to the
 * remote server, builds the CDR stream for the Request, send the CDR
 * stream and expects the response and interprets the incoming CDR
 * stream.
 */
class TAO_Export TAO_GIOP_Invocation
{
public:
  /// Default constructor. This should never get called, it is here
  /// only to appease older versions of g++.
  TAO_GIOP_Invocation (void);

  /// Constructor.
  TAO_GIOP_Invocation (TAO_Stub *data,
                       const char *operation,
                       CORBA::ULong opname_len,
                       CORBA::Boolean argument_flag,
                       TAO_ORB_Core *orb_core,
                       int byte_order = TAO_ENCAP_BYTE_ORDER);

  /**
   * This destructor is virtual so that the derived synchronous
   * invocation classes can call <idle> method on the <Transport>, if
   * they want to. All the synchronous invocations <idle> the
   * Transport, but asynchronous invocations do not do that.
   */
  virtual ~TAO_GIOP_Invocation (void);

  /**
   * Initialize the Request header.
   * The <message_size> field of the GIOP header is left blank and
   * must be filled later.
   * @par
   * The function only returns once a connection has been succesfully
   * established *OR* all profiles have been tried.  In that case it
   * raises the CORBA::TRANSIENT exception.
   */
  void prepare_header (CORBA::Octet response_flags
                       TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Accessor to the request ServiceContextList.  Only valid when
  /// sending a request message.
  TAO_Service_Context& request_service_context (void);

  /// Accessor to the reply ServiceContextList.  Only valid when
  /// after receiving a reply message.
  TAO_Service_Context& reply_service_context (void);

  /// Return the request id of this invocation.
  CORBA::ULong request_id (void) const;

  /// Return the name of the operation being invoked.
  const char *operation (void);

  /// Return the underlying output stream.
  TAO_OutputCDR &out_stream (void);

  //  CORBA::Boolean restart_flag (void);
  /// Set the value for the  restart flag.
  void restart_flag (CORBA::Boolean flag);

  /// Resets the forwarding profile and behaves like we are fowarded
  /// (to the same server).
  int close_connection (void);

  /// Establishes a connection to the remote server, initializes
  /// the GIOP headers in the output CDR.
  void start (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Called by the invocation endpoint selector for each selected
  /// endpoint.
  int perform_call (TAO_Transport_Descriptor_Interface &desc
                    TAO_ENV_ARG_DECL);

  /// Dynamically allocate \param inconsistent_policies_ PolicyList.
  void init_inconsistent_policies (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return PolicyList stored in \param inconsistent_policies_ and
  /// give up its ownership.  User must deallocate memory.
  CORBA::PolicyList *get_inconsistent_policies (void);

  /**
   * Add the given object reference to the list of forward profiles.
   * This basically emulates a LOCATION_FORWARD reply from the
   * server.  It is needed by such things as the
   * PortableInterceptor::ForwardRequest exception.
   * @par
   * The forward object reference can be retrieved by invoking the
   * forward_reference() method in this class.
   * It returns TAO_INVOKE_RESTART unless an exception is raised.
   */
  int location_forward (CORBA::Object_ptr forward
                        TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the object reference returned in the LOCATION_FORWARD
  /// reply.  The return reference is only valid if the reply status
  /// for the current reply is TAO_INVOKE_RESTART.
  CORBA::Object_ptr forward_reference (void);

  /// Returns true if a location forward was
  CORBA::Boolean received_location_forward (void) const;

  /// Accessor for the stub associated with this invocation.
  TAO_Stub * stub(void);

  /// Return the TAO_Transport in use for the current Invocation
  /// object.
  TAO_Transport *&transport (void);

  /// Accessor for the stub associated with this invocation.
  CORBA::PolicyList_var& inconsistent_policies();

  /// Endpoint Accessor
  TAO_Endpoint *endpoint (void);

  void endpoint (TAO_Endpoint *ep);

  /// Profile Accessor
  TAO_Profile *profile(void);

  /// Profile Accessor
  void profile (TAO_Profile *p);

  /// ORB_Core Accessor
  TAO_ORB_Core *orb_core (void);

  /// Return the current timeout value for the current invocation.
  ACE_Time_Value *max_wait_time (void);

  CORBA::ULong& profile_index (void);

protected:

  //  int valid_end_point_found_;
  //  int client_protocol_index

protected:

  /// Sends the request, does not wait for the response.
  /**
   * Returns TAO_INVOKE_RESTART if the write call failed and the
   * request must be re-attempted.
   *
   * @param is_synchronous If set invoke() does not return until the
   *        message is completely delivered to the underlying
   *        transport mechanism, or an error is detected.
   *
   * Notice that the same profile is tried again because it may be
   * that the server closed the connection simply to release
   * resources.
   */
  int invoke (CORBA::Boolean is_synchronous
              TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Helper method, the response for a Request or LocateRequest was a
   * LOCATION_FORWARD or TAO_GIOP_OBJECT_FORWARD.
   * In any case we must demarshal the object reference and setup the
   * profiles.  The returned forward object reference can be retrieved
   * by invoking the forward_reference() method in this class.
   * It returns TAO_INVOKE_RESTART unless an exception is raised.
   */
  int location_forward (TAO_InputCDR &inp_stream
                        TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Helper method that factors out code common to the
   * location_forward() above, and adds the given object reference to
   * the list of a forward profiles.
   *
   * The forward object reference can be retrieved by invoking the
   * forward_reference() method in this class.
   */
  void location_forward_i (TAO_Stub *stubobj
                           TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));


protected:

  /// The object on which this invocation is going.
  TAO_Stub *stub_;

  /// Buffer used for both the output and input CDR streams, this is
  /// "safe" because we only one of the streams at a time.
  char buffer_[ACE_CDR::DEFAULT_BUFSIZE];

  /// The relevant operation detail
  TAO_Operation_Details op_details_;

  TAO_Target_Specification target_spec_;

  /// Stream into which the response is placed.
  TAO_OutputCDR out_stream_;

  /// The orb_core context where we make this invocation.
  TAO_ORB_Core* orb_core_;

  /// This invocation is using this transport.
  /// @note may change...
  TAO_Transport *transport_;

  /// Strategy for making decisions about which endpoint/profile to
  /// use for invocation.
  TAO_Invocation_Endpoint_Selector *endpoint_selector_;

  /**
   * If current effective policies cause the invocation to raise
   * CORBA::INV_POLICY exception, the conflicting/problematic policies
   * are stored in this list.  This is used by \param
   * Object::_validate_connection method to inform clients about
   * causes of invocation failure.
   * @par
   * Conflicting policies are only stored in this list if \param
   * init_inconsistent_policies method has been called prior to the
   * beginning of invocation.  This saves extra work of conflicting
   * policies 'logging' when it's not needed.
   */
  CORBA::PolicyList_var inconsistent_policies_;

  /// This invocation is using this profile.
  TAO_Profile *profile_;

  /// This invocation is using this endpoint from \param profile_.
  TAO_Endpoint *endpoint_;

  /// Keep track of the remaining time for this invocation.
  ACE_Countdown_Time *countdown_;

  /// The timeout remaining for this request.  It is initialized in
  /// start() and updated as required.
  //@{
  ACE_Time_Value max_wait_time_value_;
  ACE_Time_Value *max_wait_time_;
  //@}

  /**
   * This flag is turned on when the previous invocation on an
   * endpoint or a profile returned a TAO_INVOKE_RESTART.  FT CORBA
   * relies on this flag for guarenteeing unique id's during
   * reinvocations.
   */
  CORBA::Boolean restart_flag_;

  /**
   * Object reference returned in a LOCATION_FORWARD reply.  This
   * reference is only valid when the reply status is
   * TAO_INVOKE_RESTART and
   */
  CORBA::Object_var forward_reference_;

  /// Flag is true when a LOCATION_FORWARD reply is received.
  CORBA::Boolean received_location_forward_;

  /// Keep the position of the  profile that should be used in
  /// performing the invocation.
  CORBA::ULong profile_index_;

};

// ****************************************************************

/**
 * @class TAO_GIOP_Synch_Invocation
 *
 * @brief TAO_GIOP_Synch_Invocation.
 *
 * Base class for TAO_GIOP_Twoway_Invocation and
 * TAO_GIOP_Oneway_Invocation.
 */
class TAO_Export TAO_GIOP_Synch_Invocation : public TAO_GIOP_Invocation
{
public:

  /// Default constructor. This should never get called, it is here
  /// only to appease older versions of g++.
  TAO_GIOP_Synch_Invocation (void);

  /// Constructor.
  TAO_GIOP_Synch_Invocation (TAO_Stub *stub,
                             const char *operation,
                             CORBA::ULong opname_len,
                             CORBA::Boolean argument_flag,
                             TAO_ORB_Core *orb_core,
                             int byte_order = TAO_ENCAP_BYTE_ORDER);

  /// Destructor.
  virtual ~TAO_GIOP_Synch_Invocation (void);

  /// Return the underlying input stream.  Called by the stub to
  /// demarshal the results of the upcall into whatever return
  /// arguments there may be.
  TAO_InputCDR &inp_stream (void);

protected:

  /// Implementation of the invoke() methods, handles the basic
  /// send/reply code and the system exceptions.
  int invoke_i (CORBA::Boolean is_locate_request
                TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Reply dispatcher for the current synchronous invocation.
  TAO_Synch_Reply_Dispatcher rd_;
};

// ****************************************************************

/**
 * @class TAO_GIOP_Twoway_Invocation
 *
 * @brief Encapsulate a two-way invocation.
 *
 * Sends a two-way request, and expects the reply.  This class
 * connects (or lookups a connection from the cache) to the remote
 * server, builds the CDR stream for the Request, send the CDR stream
 * and expects the response and interprets the incoming CDR stream.
 */
class TAO_Export TAO_GIOP_Twoway_Invocation
  : public TAO_GIOP_Synch_Invocation
{
public:

  /// Constructor.
  TAO_GIOP_Twoway_Invocation (TAO_Stub *stub,
                              const char *operation,
                              CORBA::ULong opname_len,
                              CORBA::Boolean argument_flag,
                              TAO_ORB_Core *orb_core,
                              int byte_order = TAO_ENCAP_BYTE_ORDER);

  /// Destructor.
  virtual ~TAO_GIOP_Twoway_Invocation (void);

  /// Establishes a connection to the remote server, initializes
  /// the GIOP headers in the output CDR.
  void start (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Special purpose invoke method used by the generated stubs. This
   * accomplishes the same task as the normal invoke except that
   * Exceptions are allocated and decoded here. This reduces the
   * footprint of the generated stubs.
   */
  int invoke (TAO_Exception_Data *excepts,
              CORBA::ULong except_count
              TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::Exception));

  /**
   * Special purpose method. This is used by services that restart
   * invocation  on a different target because of a failure of the
   * first target like the FT_service. This method is used to clear
   * off any state that are associated with the previous invocation
   */
  void reset_states (void);
};

// ****************************************************************

/**
 * @class TAO_GIOP_Oneway_Invocation
 *
 * @brief Encapsulate a one-way invocation.
 */
class TAO_Export TAO_GIOP_Oneway_Invocation
  : public TAO_GIOP_Synch_Invocation
{
public:

  /// Constructor.
  TAO_GIOP_Oneway_Invocation (TAO_Stub *stub,
                              const char *operation,
                              CORBA::ULong opname_len,
                              CORBA::Boolean argument_flag,
                              TAO_ORB_Core *orb_core,
                              int byte_order = TAO_ENCAP_BYTE_ORDER);

  /// Destructor.
  virtual ~TAO_GIOP_Oneway_Invocation (void);

  /// Establishes a connection to the remote server, initializes
  /// the GIOP headers in the output CDR.
  void start (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Send request, without blocking for any response.
  int invoke (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Accessor for private member.
  TAO::SyncScope sync_scope (void);

private:

  /// Our sync scope.
  TAO::SyncScope sync_scope_;
};

// ****************************************************************

/**
 * @class TAO_GIOP_Locate_Request_Invocation
 *
 * @brief Sends a locate request message.
 */
class TAO_Export TAO_GIOP_Locate_Request_Invocation
  : public TAO_GIOP_Synch_Invocation
{
public:
  /// Constructor.
  TAO_GIOP_Locate_Request_Invocation (TAO_Stub *data,
                                      TAO_ORB_Core *orb_core);

  /// Destructor.
  ~TAO_GIOP_Locate_Request_Invocation (void);

  /// Establishes a connection to the remote server, initializes
  /// the GIOP headers in the output CDR.
  void start (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Send request, without blocking for any response.
  int invoke (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

// ****************************************************************

#if defined (__ACE_INLINE__)
# include "Invocation.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_INVOCATION_H */
