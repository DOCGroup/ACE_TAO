// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Invocation.h
 *
 *  $Id$
 *
 *  Encapsulate the logic for   remote invocations, oneways or
 *  twoways.
 *
 * THREADING NOTE: Threads should never manipulate another
 * thread's invocations.  In this implementation, all   data
 * structures   used to represent invocations (and parts of them)
 * are owned by the thread which created them.  Multiple threads
 * may make of course   concurrent invocations safely, since the
 * GIOP code is reentrant.
 *
 *
 *  @author Carlos O'Ryan <coryan@cs.wustl.edu>
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_INVOCATION_H
#define TAO_INVOCATION_H
#include "ace/pre.h"

#include "tao/CDR.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Any.h"
#include "tao/Synch_Reply_Dispatcher.h"
#include "tao/TAOC.h"
#include "tao/operation_details.h"
#include "tao/target_specification.h"
#include "tao/Invocation_Endpoint_Selectors.h"

struct TAO_Exception_Data;
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

/**
 * @class TAO_GIOP_Invocation
 *
 * @brief Encapsulates common behavior for both oneway and twoway
 * invocations.
 *
 * This class connects (or      lookups a connection from the cache)
 * to the remote server, builds the CDR stream for      the Request,
 * send the CDR stream and      expects the response and interprets
 * the incoming CDR stream.
 */
class TAO_Export TAO_GIOP_Invocation
{

  friend class TAO_Endpoint_Selector_Factory;
  friend class TAO_Default_Endpoint_Selector;
  friend class TAO_Priority_Endpoint_Selector;
  friend class TAO_Bands_Endpoint_Selector;
  friend class TAO_Protocol_Endpoint_Selector;
  friend class TAO_Priority_Protocol_Selector;
  friend class TAO_Bands_Protocol_Selector;
  friend class TAO_Client_Priority_Policy_Selector;

public:
  /// Default constructor. This should never get        called, it is here
  /// only to appease older versions of g++.
  TAO_GIOP_Invocation (void);

  /// Constructor.
  TAO_GIOP_Invocation (TAO_Stub *data,
                       const char *operation,
                       CORBA::ULong opname_len,
                       CORBA::Boolean argument_flag,
                       TAO_ORB_Core *orb_core);

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
   * The function only returns once a connection has been succesfully
   * established *OR* all profiles have been tried.  In that case it
   * raises the CORBA::TRANSIENT exception.
   */
  void prepare_header (CORBA::Octet response_flags,
                       CORBA_Environment &ACE_TRY_ENV =
                         TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Accessor to the request ServiceContextList.  Only valid when
  /// sending a request message.
  IOP::ServiceContextList& service_info (void);

  /// Return the        request id of this invocation.
  CORBA::ULong request_id (void);

  /// Return the        underlying output stream.
  TAO_OutputCDR &out_stream (void);

  //  CORBA::Boolean restart_flag (void);
  /// Set the value for the  restart flag.
  void restart_flag (CORBA::Boolean flag);

  /// resets the        forwarding profile and behaves like we are fowarded
  /// (to the same server)
  int close_connection (void);

  /// Establishes a connection to the remote server, initializes
  /// the GIOP headers in the output CDR.
  void start (CORBA_Environment &ACE_TRY_ENV =
                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Dynamically allocate <inconsistent_policies_> PolicyList.
  void init_inconsistent_policies (CORBA_Environment &ACE_TRY_ENV =
                                   TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return PolicyList stored in <inconsistent_policies_> and give up
  /// its ownership.  User must deallocate memory.
  CORBA::PolicyList *get_inconsistent_policies (void);

protected:
  /**
   * Sends the request, does not wait for the response.
   * Returns TAO_INVOKE_RESTART if the write call failed and the
   * request must be re-attempted.
   * Notice that the same profile is tried again because it may be
   * that the server closed the connection simply to release
   * resources.
   */
  int invoke (CORBA::Boolean is_roundtrip,
              CORBA_Environment &ACE_TRY_ENV =
                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Helper method, the response for a Request or LocateRequest was a
   * LOCATION_FORWARD or TAO_GIOP_OBJECT_FORWARD.
   * In any case we must demarshal the object reference and setup the
   * profiles.
   * It returns TAO_INVOKE_RESTART unless an exception is raised.
   */
  int location_forward (TAO_InputCDR &inp_stream,
                        CORBA_Environment &ACE_TRY_ENV =
                          TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Create the IOP::IOR info. We will create the info atmost
   * once.  Return index of the profile we are using to make the
   * invocation.
   */
  CORBA::ULong create_ior_info (void);

  /**
   * Add RT-related context to the service context list if the
   * invocation target supports RTCORBA::CLIENT_PROPAGATED priority
   * model.
   */
  void add_rt_service_context (CORBA_Environment &ACE_TRY_ENV =
                               TAO_default_environment ());

protected:
  /// The object        on which this invocation is going.
  TAO_Stub *stub_;

  /// Buffer used for both the output and input CDR streams, this is
  /// "safe" because we only one        of the streams at a time.
  char buffer_ [ACE_CDR::DEFAULT_BUFSIZE];

  /// The relevant operation detail
  TAO_Operation_Details  op_details_;

  TAO_Target_Specification target_spec_;

  /// Stream into which the response is placed.
  TAO_OutputCDR out_stream_;

  /// The orb_core context where        we make this invocation.
  TAO_ORB_Core* orb_core_;

  /// This invocation is        using this transport, may change...
  TAO_Transport *transport_;

  /// Strategy for making decisions about which endpoint/profile        to use
  /// for invocation.
  TAO_Invocation_Endpoint_Selector *endpoint_selector_;

  /// Flag indicating whether <endpoint_selector_> has been
  /// initialized.
  int is_selector_initialized_;

#if (TAO_HAS_RT_CORBA == 1)

  /// Store information used by <endpoint_selector_> for        making
  /// endpoint selection        decisions.
  TAO_Endpoint_Selection_State endpoint_selection_state_;

#endif /* TAO_HAS_RT_CORBA == 1 */

  /**
   * If current effective policies cause the invocation to raise
   * CORBA::INV_POLICY exception, the conflicting/problematic policies
   * are stored in this list.  This is used by
   * <Object::_validate_connection> method to inform clients about
   * causes of invocation failure.
   *
   * Conflicting policies are only stored in this list if
   * <init_inconsistent_policies> method has been called prior to the
   * beginning of invocation.  This saves extra work of conflicting
   * policies 'logging' when it's not needed.
   *
   */
  CORBA::PolicyList_var inconsistent_policies_;

  /// This invocation is        using this profile.
  TAO_Profile *profile_;

  /// This invocation is        using this endpoint from <profile_>.
  TAO_Endpoint *endpoint_;

  /// The timeout remaining for this request, it        is initialized in
  /// start() and updated as required.
  ACE_Time_Value max_wait_time_value_;
  ACE_Time_Value *max_wait_time_;

  /**
   * The ior info. This would be needed for GIOP 1.2, as the clients
   * could receive an exception from the server asking for this
   * info. The exception that the client receives is
   * LOC_NEEDS_ADDRESSING_MODE. If we receive an exception we will
   * fill up this data atmost *once* and send it to the server.
   */
  IOP::IOR ior_info_;

  /**
   * Flag indicating whether RTCORBA-specific service context list
   * processing has taken place.  This is needed because
   * <prepare_header> may get called multiple times, but we only need
   * to do the service context list processing once.
   */
  int rt_context_initialized_;

  /**
   * This flag is turned on when the previous invocation on an
   * endpoint or a profile returned a TAO_INVOKE_RESTART. FT CORBA
   * relies on this flag for guarenteeing unique id's during
   * reinvocations.
   */
  CORBA::Boolean restart_flag_;
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
  /// Default constructor. This should never get        called, it is here
  /// only to appease older versions of g++.
  TAO_GIOP_Synch_Invocation (void);

  /// Constructor.
  TAO_GIOP_Synch_Invocation (TAO_Stub *stub,
                             const char *operation,
                             CORBA::ULong opname_len,
                             CORBA::Boolean argument_flag,
                             TAO_ORB_Core *orb_core);

  /// Destructor.
  virtual ~TAO_GIOP_Synch_Invocation (void);

  /// Return the        underlying input stream. Called by the stub to demarshal
  /// the results of the        upcall into whatever return arguments there may be.
  TAO_InputCDR &inp_stream (void);

protected:
  /// Implementation of the invoke() methods, handles the basic
  /// send/reply        code and the system exceptions.
  int invoke_i (CORBA::Boolean is_locate_request,
                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Reply dispatcher for the current synchronous invocation.
  TAO_Synch_Reply_Dispatcher rd_;
};

// ****************************************************************

class TAO_Export TAO_GIOP_Twoway_Invocation
  : public TAO_GIOP_Synch_Invocation
{
  // = TITLE
  //    TAO_GIOP_Twoway_Invocation.
  //
  // = DESCRIPTION
  //    Sends a two-way request, and expects the reply.
  //    This class connects (or lookups a connection from the cache) to
  //    the remote server, builds the CDR stream for the Request, send
  //    the CDR stream and expects the response and interprets the
  //    incoming CDR stream.
  //
public:
  /// Constructor.
  TAO_GIOP_Twoway_Invocation (TAO_Stub *stub,
                              const char *operation,
                              CORBA::ULong opname_len,
                              CORBA::Boolean argument_flag,
                              TAO_ORB_Core *orb_core);

  /// Destructor.
  virtual ~TAO_GIOP_Twoway_Invocation (void);

  /// Establishes a connection to the remote server, initializes
  /// the GIOP headers in the output CDR.
  void start (CORBA_Environment &ACE_TRY_ENV =
                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Special purpose invoke method used by the generated stubs. This
   * accomplishes the same task as the normal invoke except that
   * Exceptions are allocated and decoded here. This reduces the
   * footprint of the generated stubs.
   */
  int invoke (TAO_Exception_Data *excepts,
              CORBA::ULong except_count,
              CORBA_Environment &ACE_TRY_ENV =
                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::Exception));
};

// ****************************************************************

class TAO_Export TAO_GIOP_Oneway_Invocation
  : public TAO_GIOP_Synch_Invocation
{
  // = TITLE
  //    TAO_GIOP_Oneway_Invocation
  //
  // = DESCRIPTION
  //    Sends a oneway request.
  //
public:
  /// Constructor.
  TAO_GIOP_Oneway_Invocation (TAO_Stub *stub,
                              const char *operation,
                              CORBA::ULong opname_len,
                              CORBA::Boolean argument_flag,
                              TAO_ORB_Core *orb_core);

  /// Destructor.
  virtual ~TAO_GIOP_Oneway_Invocation (void);

  /// Establishes a connection to the remote server, initializes
  /// the GIOP headers in the output CDR.
  void start (CORBA_Environment &ACE_TRY_ENV =
                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Send request, without blocking for        any response.
  int invoke (CORBA_Environment &ACE_TRY_ENV =
                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Acessor for private member.
  TAO::SyncScope sync_scope (void);
private:
  /// Our sync scope.
  TAO::SyncScope sync_scope_;
};

// ****************************************************************

class TAO_Export TAO_GIOP_Locate_Request_Invocation
  : public TAO_GIOP_Synch_Invocation
{
  // = TITLE
  //   Sends a locate request.
  //
public:
  /// Constructor.
  TAO_GIOP_Locate_Request_Invocation (TAO_Stub *data,
                                      TAO_ORB_Core *orb_core);

  /// Destructor.
  ~TAO_GIOP_Locate_Request_Invocation (void);

  /// Establishes a connection to the remote server, initializes
  /// the GIOP headers in the output CDR.
  void start (CORBA_Environment &ACE_TRY_ENV =
                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Send request, without blocking for        any response.
  int invoke (CORBA_Environment &ACE_TRY_ENV =
                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
};

// ****************************************************************

#if defined (__ACE_INLINE__)
# include "tao/Invocation.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_INVOCATION_H */
