// -*- C++ -*-

// @@ Mayur the canonical naming convention is to call this file
// "AMH_Response_Handler.h", without the TAO_ prefix.  The
// Server_Request.h class is a deviation from the rules.

/**
 *  @file   TAO_AMH_Response_Handler.h
 *
 *  $Id$
 *
 *  @author Mayur Deshpande (mayur@ics.uci..edu)
 *
 */
#ifndef TAO_AMH_RESPONSE_HANDLER_H
#define TAO_AMH_RESPONSE_HANDLER_H

#include "tao/Service_Context.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Transport;
class TAO_Pluggable_Messaging;
class TAO_Output_CDR;
class TAO_ORB_Core;
class TAO_ServerRequest;

class TAO_Export TAO_AMH_Response_Handler
{
public:
  // @@ Mayur, please document the class and all its methods using the
  // Doxygen style....
  TAO_AMH_Response_Handler (TAO_ServerRequest &server_request);
  virtual ~TAO_AMH_Response_Handler (void);

  // @@ Mayur, it looks like the following two methods are *only* used
  // by derived classes, can we make them protected?
  // @@ Also: you may want to call them _tao_init_reply() and
  // _tao_send_reply(), otherwise they pollute the user namespace.
  // What if the user has:
  //
  // // IDL
  // interface Foo {
  //  void init_reply ();
  // };
  // // implied-IDL
  // local interface AMH_FooResponseHandler {
  //   void init_reply (); // uh-oh...
  // };
  //
  void init_reply (void);
  void send_reply (void);

private:
  // @@ Mayur please document the fields of the class too!
  TAO_Pluggable_Messaging *mesg_base_;
  CORBA::ULong request_id_; // copy
  CORBA::Boolean response_expected_;
  TAO_Transport *transport_;
  /// A pointer to the ORB Core for the context where the request was
  /// created.
  TAO_ORB_Core *orb_core_;

  TAO_Service_Context reply_service_context_; //copy
  CORBA::Boolean argument_flag_;

  //  TAO_GIOP_ReplyStatusType exception_type_;
  /// Exception type (will be NO_EXCEPTION in the majority of the cases).
  CORBA::ULong exception_type_;

  int once_only_;

protected:
  /// The outgoing CDR stream
  /**
   * The IDL-generated ResponseHandler implementations used this field
   * to marshal the response.
   * Making it a field instead of a public accessor makes the code in
   * the generated ResponseHandler implementation a lot more readable.
   */
  TAO_OutputCDR _tao_out;

private:
  // Private and undefined, standard C++ idiom to prohibit copying.
  ACE_UNIMPLEMENTED_FUNC (TAO_AMH_Response_Handler (const TAO_AMH_Response_Handler&))
  ACE_UNIMPLEMENTED_FUNC (TAO_AMH_Response_Handler& operator= (const TAO_AMH_Response_Handler&))
};

#endif /* TAO_AMH_RESPONSE_HANDLER_H */
