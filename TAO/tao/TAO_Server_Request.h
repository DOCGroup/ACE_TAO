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


#ifndef TAO_GIOP_SERVER_REQUEST_H
#define TAO_GIOP_SERVER_REQUEST_H
#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"

#include "Tagged_Profile.h"
#include "OctetSeqC.h"
#include "Service_Context.h"
#include "Object.h"

#if TAO_HAS_INTERCEPTORS == 1
#include "PICurrent.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

class TAO_Pluggable_Messaging;
class TAO_Transport;
class TAO_AMH_Response_Handler;

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

  TAO_ServerRequest (TAO_Pluggable_Messaging *mesg_base,
                     TAO_InputCDR &input,
                     TAO_OutputCDR &output,
                     TAO_Transport *transport,
                     TAO_ORB_Core *orb_core);

  TAO_ServerRequest (TAO_Pluggable_Messaging *mesg_base,
                     CORBA::ULong request_id,
                     CORBA::Boolean response_expected,
                     CORBA::Boolean deferred_flag,
                     TAO_ObjectKey &object_key,
                     const char *operation,
                     TAO_OutputCDR &output,
                     TAO_Transport *transport,
                     TAO_ORB_Core *orb_core,
                     int &parse_error);

  /// Destructor.
  virtual ~TAO_ServerRequest (void);

  /**
   * @name Request attributes.
   */
  //@{
  /// Return the operation name.
  const char *operation (void) const;

  /// Set the operation name.
  void operation (const char *operation,
                  size_t length,
                  int release);

  /// Return the legnth of the operation.
  unsigned int operation_length (void) const;
  //@}

  /// Return the underlying ORB.
  CORBA::ORB_ptr orb (void);

  /// Return the ORB core pointer member.
  TAO_ORB_Core *orb_core (void);

  /// Start a Reply message.
  void init_reply (void);

  /// Retrieve the incoming stream.
  TAO_InputCDR &incoming (void);

  /// Retrieve the outgoing stream.
  TAO_OutputCDR &outgoing (void);

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

  // CORBA::Principal_ptr principal (void) const;

  TAO_ObjectKey &object_key (void);

  /**
   * @todo This two methods should go away.
   */
  //@{
  IOP::ServiceContextList &request_service_info (void);
  IOP::ServiceContextList &reply_service_info (void);
  //@}

  /// Return the TAO_Service_Context
  TAO_Service_Context &request_service_context (void);
  TAO_Service_Context &reply_service_context (void);

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

  /// Get the exception type.
  CORBA::ULong exception_type (void);

  /// Set the exception type.
  void exception_type (CORBA::ULong except_type);

  /// Set the requesting principal.
  /**
   * @note Deprecated in CORBA 2.4
   */
  // void requesting_principal (CORBA_Principal_ptr principal);

  /// Set the requesting principal
  void requesting_principal (const CORBA_OctetSeq & principal);

  /// Return the reference to the tagged profile
  TAO_Tagged_Profile &profile (void);

  void tao_send_reply (void);

  void tao_send_reply_exception (CORBA::Exception&);

  /// Set the boolean member to 1.
  void is_dsi (void);

  /// Set the member.
  void dsi_nvlist_align (ptr_arith_t alignment);

  /// Get/Set operations for the argument_flag
  void argument_flag (CORBA::Boolean flag);
  CORBA::Boolean argument_flag (void);

#if TAO_HAS_INTERCEPTORS == 1
  /// Return a reference to the number of interceptors pushed on to
  /// the current interceptor flow stack.
  /**
   * @note It is a reference since the Portable Interceptor flow stack
   *       code  must be able to modify this value and use that value
   *       at a later time without being forced to use TSS.
   */
  size_t &interceptor_count (void);

  /// Return a reference to the "request scope" PICurrent object.
  TAO_PICurrent_Impl &rs_pi_current (void);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

private:

  TAO_Pluggable_Messaging *mesg_base_;

  /// Operation name.
  ACE_CString operation_;

  CORBA::Object_var forward_location_;

  /// Incoming stream.
  TAO_InputCDR *incoming_;

  /// Outgoing stream.
  TAO_OutputCDR *outgoing_;

  /// Transport class.
  TAO_Transport *transport_;

  /// 0: oneway (SYNC_NONE or SYNC_WITH_TRANSPORT)
  /// 1: twoway, or oneway (SYNC_WITH_SERVER or SYNC_WITH_TARGET)
  CORBA::Boolean response_expected_;

  /**
   * 0: Reply would be sent by the object of this class which is the
   *    default.
   * 1: Reply would not be prepared by this class and it would be
   *    deferred for somebody.
   */
  CORBA::Boolean deferred_reply_;

  /// 1: oneway (SYNC_WITH_SERVER)
  /// 0: anything else
  CORBA::Boolean sync_with_server_;

  //  TAO_GIOP_ReplyStatusType exception_type_;
  /// Exception type (will be NO_EXCEPTION in the majority of the cases).
  CORBA::ULong exception_type_;

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
  CORBA_OctetSeq_var requesting_principal_;

  /// Did we get passed to a CORBA::ServerRequest?
  CORBA::Boolean is_dsi_;

  /// Used to pad CDR stream if we have used DSI.
  ptr_arith_t dsi_nvlist_align_;

  /**
   * An argument flag to indicate whether there is any data that is
   * going to get marshalled along as a reply. The default will be 1
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
  TAO_PICurrent_Impl rs_pi_current_;
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
};

#if defined (__ACE_INLINE__)
# include "TAO_Server_Request.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_GIOP_SERVER_REQUEST_H */
