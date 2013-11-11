// -*- C++ -*-

//=============================================================================
/**
*  @file    TAO_Server_Request.h
*
*  $Id$
*
*  Header file for CORBA's Server Skeleton Interface's
*  "Server Request" type.
*
*
* @author Copyright 1994-1995 by Sun Microsystems Inc.
* @author Chris Cleeland
* @author Aniruddha Gokhale
*/
//=============================================================================

#ifndef TAO_SERVER_REQUEST_H
#define TAO_SERVER_REQUEST_H

#include /**/ "ace/pre.h"
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Tagged_Profile.h"
#include "tao/Service_Context.h"
#include "tao/Object.h"
#include "tao/Transport_Selection_Guard.h"
#include "tao/GIOPC.h"

#if TAO_HAS_INTERCEPTORS == 1

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class PICurrent;
  class PICurrent_Impl;
}
TAO_END_VERSIONED_NAMESPACE_DECL

#include "tao/PortableInterceptorC.h"

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_GIOP_Message_Base;
class TAO_Transport;
class TAO_AMH_Response_Handler;

namespace CORBA
{
  class ORB;
  typedef ORB *ORB_ptr;

  class Exception;
}

namespace TAO
{
  namespace CSD
  {
    class FW_Server_Request_Wrapper;
  }
}

class TAO_Operation_Details;

/**
 * @class TAO_ServerRequest
 *
 * @brief Class representing a ServerRequest object.
 *
 * Encapsulates CDR, transport and pluggable messaging
 * components on the server side.
*/
class TAO_Export TAO_ServerRequest
{
public:

  /// Declare FW_Server_Request_Wrapper a friend
  /// This friendship makes the FW_Server_Request_Wrapper be able to
  /// clone the TAO_ServerRequest.
  friend class TAO::CSD::FW_Server_Request_Wrapper;

  /// Declare TAO_AMH_Response_Handler a friend
  /**
   * The TAO_AMH_Response_Handler class needs to copy part of the
   * state in a TAO_ServerRequest, however, we do not want to expose
   * that state as public members of this class, neither do we want to
   * add modifiers to the TAO_AMH_Response_Handler class that would
   * allow us (the TAO_ServerRequest class) to set the state.
   *
   * Lucky for us, C++ is a language for adult developers, and allow
   * us to use the evil "friend" declaration.
   *
   */
  friend class TAO_AMH_Response_Handler;

  // Constructors.

  TAO_ServerRequest (TAO_GIOP_Message_Base *mesg_base,
                     TAO_InputCDR &input,
                     TAO_OutputCDR &output,
                     TAO_Transport *transport,
                     TAO_ORB_Core *orb_core);

  TAO_ServerRequest (TAO_GIOP_Message_Base *mesg_base,
                     CORBA::ULong request_id,
                     CORBA::Boolean response_expected,
                     CORBA::Boolean deferred_flag,
                     TAO::ObjectKey &object_key,
                     const char *operation,
                     TAO_OutputCDR &output,
                     TAO_Transport *transport,
                     TAO_ORB_Core *orb_core,
                     int &parse_error);

  /// Constructor used by thru-POA collocated invocation path.
  TAO_ServerRequest (TAO_ORB_Core * orb_core,
                     TAO_Operation_Details const & details,
                     CORBA::Object_ptr target);

  /// Destructor.
  ~TAO_ServerRequest (void);

  /**
   * @name Request attributes.
   */
  //@{
  /// Return the operation name.
  const char *operation (void) const;

  /// Set the operation name.
  void operation (const char *operation, size_t length, int release);

  /// Return the length of the operation.
  size_t operation_length (void) const;
  //@}

  /// Return the underlying ORB.
  CORBA::ORB_ptr orb (void);

  /// Return the ORB core pointer member.
  TAO_ORB_Core *orb_core (void) const;

  /// Start a Reply message.
  void init_reply (void);

  /// Retrieve the incoming stream.
  TAO_InputCDR * incoming (void) const;

  /// Retrieve the outgoing stream.
  TAO_OutputCDR * outgoing (void) const;

  /// Is the response expected?
  CORBA::Boolean response_expected (void) const;

  /// Should the reply be deferred?
  CORBA::Boolean deferred_reply (void) const;

  /// Set the response expected flag.
  void response_expected (CORBA::Boolean response);

  /// Should we return before dispatching the servant?
  CORBA::Boolean sync_with_server (void) const;

  /// Set the sync_with_server flag.
  void sync_with_server (CORBA::Boolean sync_flag);

  /// Used with reliable oneway requests.
  void send_no_exception_reply (void);

  TAO::ObjectKey &object_key (void);

  /// Return the request TAO_Service_Context
  TAO_Service_Context &request_service_context (void);

  /// Return the reply TAO_Service_Context
  TAO_Service_Context &reply_service_context (void);

  IOP::ServiceContextList &reply_service_info (void);

  IOP::ServiceContextList &request_service_info (void);

  /// Return the underlying transport
  TAO_Transport *transport ();

  /// To handle System Exceptions at the lowest level, a method
  /// returning the request_id_ is needed.
  //@{
  CORBA::ULong request_id (void);
  void request_id (CORBA::ULong req);
  //@}

  /**
   * Set the reference to the object the request should be forwarded
   * to.  This reference will only be used if set prior to calling
   * init_reply().
   */
  void forward_location (CORBA::Object_ptr forward_reference);

  /// Get the forward_location.
  CORBA::Object_ptr forward_location (void);

  /**
   * Since forward location is allowed to be nil then this is a proper
   * method to check if the request is being forwarded.
   */
  bool is_forwarded (void) const;

  /// Get the reply status
  GIOP::ReplyStatusType reply_status (void);

  /// Set the reply status
  void reply_status (GIOP::ReplyStatusType except_type);

  /// Set the requesting principal
  void requesting_principal (const CORBA::OctetSeq & principal);

  /// Return the reference to the tagged profile
  TAO_Tagged_Profile &profile (void);

  void tao_send_reply (void);

  void tao_send_reply_exception (const CORBA::Exception& ex);

  /// Set the boolean member to true.
  void is_dsi (void);

  /// Set the member.
  void dsi_nvlist_align (ptrdiff_t alignment);

  /// Get the operation details for the current request.
  TAO_Operation_Details const * operation_details (void) const;

  /// Set the argument_flag
  void argument_flag (CORBA::Boolean flag);

  /// Get the argument_flag
  CORBA::Boolean argument_flag (void);

  /// Returns @c true if the current request is collocated.
  bool collocated (void) const;

#if TAO_HAS_INTERCEPTORS == 1
  /// Send cached reply. Used in scenarios where the FTORB thinks that
  /// this request is a duplicate
  void send_cached_reply (CORBA::OctetSeq &ocs);

  /// Return a reference to the number of interceptors pushed on to
  /// the current interceptor flow stack.
  /**
   * @note It is a reference since the Portable Interceptor flow stack
   *       code must be able to modify this value and use that value
   *       at a later time without being forced to use TSS.
   */
  size_t & interceptor_count (void);

  /// Return a reference to the "request scope" PICurrent object.
  TAO::PICurrent_Impl *rs_pi_current (void);

  CORBA::Exception *caught_exception (void);

  void caught_exception (CORBA::Exception *exception);

  /// Set the status of the received reply.
  void pi_reply_status (PortableInterceptor::ReplyStatus s);

  /// Get the status of the received reply.
  PortableInterceptor::ReplyStatus pi_reply_status (void);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#if TAO_HAS_ZIOP == 1
  CORBA::Policy_ptr clientCompressionEnablingPolicy ();
  void clientCompressionEnablingPolicy (CORBA::Policy_ptr);
  CORBA::Policy_ptr clientCompressorIdLevelListPolicy ();
  void clientCompressorIdLevelListPolicy (CORBA::Policy_ptr);
#endif /* TAO_HAS_ZIOP == 1 */

private:
  /// Default ctor only used to create a TAO_ServerRequest that is about
  /// to be the target of a clone operation.
  TAO_ServerRequest (void);

  TAO_GIOP_Message_Base *mesg_base_;

  /// Operation name.
  const char* operation_;

  /// Operation length.
  size_t operation_len_;

  /// Do we own the memory associated with operation_?
  bool release_operation_;

  CORBA::Object_var forward_location_;

  bool is_forwarded_;

  /// Incoming stream.
  TAO_InputCDR *incoming_;

  /// Outgoing stream.
  TAO_OutputCDR *outgoing_;

  /// false: oneway (SYNC_NONE or SYNC_WITH_TRANSPORT)
  /// true: twoway, or oneway (SYNC_WITH_SERVER or SYNC_WITH_TARGET)
  CORBA::Boolean response_expected_;

  /**
   * false: Reply would be sent by the object of this class which is the
   *    default.
   * true: Reply would not be prepared by this class and it would be
   *    deferred for somebody.
   */
  CORBA::Boolean deferred_reply_;

  /// true: oneway (SYNC_WITH_SERVER)
  /// false: anything else
  CORBA::Boolean sync_with_server_;

  /// Did we get passed to a CORBA::ServerRequest?
  CORBA::Boolean is_dsi_;

  /// Reply status (will be NO_EXCEPTION in the majority of the cases).
  GIOP::ReplyStatusType reply_status_;

  /// A pointer to the ORB Core for the context where the request was
  /// created.
  TAO_ORB_Core *orb_core_;

  /// Service Context info
  TAO_Service_Context request_service_context_;
  TAO_Service_Context reply_service_context_;

  /// Unique identifier for a request.
  CORBA::ULong request_id_;

  /// The tagged profile that has the addressing information.
  TAO_Tagged_Profile profile_;

  /// Identifies the requester.
  CORBA::OctetSeq_var requesting_principal_;

  /// Used to pad CDR stream if we have used DSI.
  ptrdiff_t dsi_nvlist_align_;

  TAO_Operation_Details const * operation_details_;

  /**
   * An argument flag to indicate whether there is any data that is
   * going to get marshaled along as a reply. The default will be true
   * which indicates that we have some data that needs to be sent back
   * to the client.
   */
  CORBA::Boolean argument_flag_;

#if TAO_HAS_INTERCEPTORS == 1
  /// The number of interceptors pushed on to the current interceptor
  /// flow stack.
  size_t interceptor_count_;

  /// The "Request Scope Current" (RSC) object, as required by
  /// Portable Interceptors.
  TAO::PICurrent_Impl *rs_pi_current_;

  /// Pointer to the caught exception.
  CORBA::Exception * caught_exception_;

  /// Reply status for the current request.
  PortableInterceptor::ReplyStatus pi_reply_status_;
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

  ///// Transport class.
  /// An RAII (resource acquisition is initialization) class instance
  /// for interfacing with TSS storage for the "current" transport.
  TAO::Transport_Selection_Guard transport_;

#if TAO_HAS_ZIOP == 1
  CORBA::Policy_var clientCompressionEnablingPolicy_;
  CORBA::Policy_var clientCompressorIdLevelListPolicy_;
#endif /* TAO_HAS_ZIOP == 1 */
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/TAO_Server_Request.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_SERVER_REQUEST_H */
