// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    TAO_Server_Request.h
//
// = DESCRIPTION
//    Header file for CORBA's Server Skeleton Interface's
//    "Server Request" type.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc. and Chris Cleeland.
//     Modifications by Aniruddha Gokhale based on CORBAv2.2 Feb 98
// ============================================================================

#ifndef TAO_GIOP_SERVER_REQUEST_H
#define TAO_GIOP_SERVER_REQUEST_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "tao/ORB.h"
//#include "tao/Principal.h"
//#include "tao/GIOP_Message_Base.h"
#include "tao/Tagged_Profile.h"
#include "tao/OctetSeqC.h"

class TAO_Pluggable_Messaging;
class TAO_Transport;

class TAO_Export TAO_ServerRequest
{
  // = TITLE
  //    Class representing a ServerRequest object.
  //
  //   DESCRIPTION
  //    Encapsulates CDR, transport and pluggable messaging
  //    components on the server side.
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

  virtual ~TAO_ServerRequest (void);
  // Destructor.

  // = Request attributes.

  const char *operation (void) const;
  // Return the operation name.

  void operation (ACE_CString &operation);
  // Set the operation name.

  void operation (const char * name,
                  int release);
  // Set the operation name.

  unsigned int operation_length (void) const;
  // Return the legnth of the operation.

  // = TAO extensions.

  CORBA::ORB_ptr orb (void);
  // Return the underlying ORB.

  TAO_ORB_Core *orb_core (void);
  // Return the ORB core pointer member.

  void init_reply (void);
  // Start a Reply message.

  TAO_InputCDR &incoming (void);
  // Retrieve the incoming stream.

  TAO_OutputCDR &outgoing (void);
  // Retrieve the outgoing stream.

  CORBA::Boolean response_expected (void) const;
  // Is the response expected?

  CORBA::Boolean deferred_reply (void) const;
  // Should the reply be deferred?

  void response_expected (CORBA::Boolean response);
  // Set the response expected flag.

  CORBA::Boolean sync_with_server (void) const;
  // Should we return before dispatching the servant?

  void sync_with_server (CORBA::Boolean sync_flag);
  // Set the sync_with_server flag.

  void send_no_exception_reply (void);
  // Used with reliable oneway requests.

  // CORBA::Principal_ptr principal (void) const;

  TAO_ObjectKey &object_key (void);

  IOP::ServiceContextList &service_info (void);
  void service_info (IOP::ServiceContextList &service_info);

  // To handle System Exceptions at the lowest level,
  // a method returning the request_id_ is needed.
  CORBA::ULong request_id (void);
  void request_id (CORBA::ULong req);

  CORBA::Object_ptr forward_location (void);
  // Get the forward_location.

  CORBA::ULong exception_type (void);
  // Get the exception type.

  void exception_type (CORBA::ULong except_type);
  // Set the exception type.

  /**** Deprecated in CORBA 2.4 *****/
  // void requesting_principal (CORBA_Principal_ptr principal);
  // Set the requesting principal.

  void requesting_principal (const CORBA_OctetSeq & principal);
  // Set the requesting principal

  TAO_Tagged_Profile &profile (void);
  // Return the reference to the tagged profile

  void tao_send_reply (void);

  void tao_send_reply_exception (CORBA::Exception&);

  void is_dsi (void);
  // Set the boolean member to 1.

  void dsi_nvlist_align (ptr_arith_t alignment);
  // Set the member.

  void argument_flag (CORBA::Boolean flag);
  CORBA::Boolean argument_flag (void);
  // Get/Set operations for the argument_flag

private:

  TAO_Pluggable_Messaging *mesg_base_;

  ACE_CString operation_;
  // Operation name.

  CORBA::Object_var forward_location_;

  TAO_InputCDR *incoming_;
  // Incoming stream.

  TAO_OutputCDR *outgoing_;
  // Outgoing stream.

  TAO_Transport *transport_;
  // Transport class.

  CORBA::Boolean response_expected_;
  // 0: oneway (SYNC_NONE or SYNC_WITH_TRANSPORT)
  // 1: twoway, or oneway (SYNC_WITH_SERVER or SYNC_WITH_TARGET)

  CORBA::Boolean deferred_reply_;
  // 0: Reply would be sent by the object of this class which is the
  //    default.
  // 1: Reply would not be prepared by this class and it would be
  //    deferred for somebody.

  CORBA::Boolean sync_with_server_;
  // 1: oneway (SYNC_WITH_SERVER)
  // 0: anything else

  //  TAO_GIOP_ReplyStatusType exception_type_;
  CORBA::ULong exception_type_;
  // Exception type (will be NO_EXCEPTION in the majority of the cases).

  TAO_ORB_Core *orb_core_;
  // A pointer to the ORB Core for the context where the request was
  // created.

  IOP::ServiceContextList service_info_;
  // The service context for the request (CORBA Reference?).

  CORBA::ULong request_id_;
  // Unique identifier for a request.

  TAO_Tagged_Profile profile_;
  // The tagged profile that has the addressing information.

  CORBA_OctetSeq_var requesting_principal_;
  // Identifies the requester.

  CORBA::Boolean is_dsi_;
  // Did we get passed to a CORBA::ServerRequest?

  ptr_arith_t dsi_nvlist_align_;
  // Used to pad CDR stream if we have used DSI.

  CORBA::Boolean argument_flag_;
  // An argument flag to indicate whether there is any data that is
  // going to get marshalled along as a reply. The default will be 1
  // which indicates that we have some data that needs to be sent back
  // to the client.
};

#if defined (__ACE_INLINE__)
# include "tao/TAO_Server_Request.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_GIOP_SERVER_REQUEST_H */
