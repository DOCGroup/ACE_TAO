// This may look like C, but it's really -*- C++ -*-

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
 *  @author Copyright 1994-1995 by Sun Microsystems, Inc.
 *  @author Chris Cleeland.
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef TAO_GIOP_SERVER_REQUEST_H
#define TAO_GIOP_SERVER_REQUEST_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "tao/ORB.h"
#include "tao/Tagged_Profile.h"
#include "tao/OctetSeqC.h"
#include "tao/Service_Context.h"

class TAO_Pluggable_Messaging;
class TAO_Transport;

/**
 * @class TAO_ServerRequest
 *
 * @brief Class representing a ServerRequest object.
 * DESCRIPTION
 * Encapsulates CDR, transport and pluggable messaging
 * components on the server side.
 */
class TAO_Export TAO_ServerRequest
{
public:
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
                     const ACE_CString &operation,
                     TAO_OutputCDR &output,
                     TAO_Transport *transport,
                     TAO_ORB_Core *orb_core,
                     int &parse_error);

  /// Destructor.
  virtual ~TAO_ServerRequest (void);

  // = Request attributes.

  /// Return the operation name.
  const char *operation (void) const;

  /// Set the operation name.
  void operation (ACE_CString &operation);

  /// Set the operation name.
  void operation (const char * name,
                  int release);

  /// Return the legnth of the operation.
  unsigned int operation_length (void) const;

  // = TAO extensions.

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

  IOP::ServiceContextList &service_info (void);
  void service_info (IOP::ServiceContextList &service_info);
  // @@ The above two should go away...

  TAO_Service_Context &service_context (void);
  // Return the TAO_Service_Context

  TAO_Transport *transport ();
  // Return the underlying transport

  // To handle System Exceptions at the lowest level,
  // a method returning the request_id_ is needed.
  CORBA::ULong request_id (void);
  void request_id (CORBA::ULong req);

  /// Get the forward_location.
  CORBA::Object_ptr forward_location (void);

  /// Get the exception type.
  CORBA::ULong exception_type (void);

  /// Set the exception type.
  void exception_type (CORBA::ULong except_type);

  /// void requesting_principal (CORBA_Principal_ptr principal);
  /// Set the requesting principal.
  /**** Deprecated in CORBA 2.4 *****/

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
  // A pointer to the ORB Core for the context where the request was
  // created.

  //IOP::ServiceContextList service_info_;
  // The service context for the request (CORBA Reference?).

  TAO_Service_Context service_context_;
  // Service Context info

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
};

#if defined (__ACE_INLINE__)
# include "tao/TAO_Server_Request.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_GIOP_SERVER_REQUEST_H */
