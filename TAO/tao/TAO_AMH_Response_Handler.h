// -*- C++ -*-
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

//#include "tao/CDR.h"
//#include "tao/ORB_Core.h"
//#include "tao/ORB.h"
//#include "tao/Pluggable_Messaging_Utils.h"
//#include "tao/GIOP_Utils.h"
#include "tao/Service_Context.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Transport;
class TAO_Pluggable_Messaging;
class TAO_Output_CDR;
class TAO_ORB_Core;

class TAO_AMH_Response_Handler
{
public:
TAO_AMH_Response_Handler ();
void init_reply (void);
void send_reply (void);
void mesg_base (TAO_Pluggable_Messaging *mesg_base);
void request_id (CORBA::ULong request_id);
void response_expected (CORBA::Boolean response_expected);
void transport (TAO_Transport *transport);
//void reply_service_info (IOP::ServiceContext &service_context);
virtual ~TAO_AMH_Response_Handler (void);

TAO_OutputCDR *tao_out_;


private:
TAO_Pluggable_Messaging *mesg_base_;
CORBA::ULong request_id_; // copy
CORBA::Boolean response_expected_;
TAO_Transport *transport_;
TAO_Service_Context reply_service_context_; //copy
CORBA::Boolean argument_flag_;

/// A pointer to the ORB Core for the context where the request was
/// created.
TAO_ORB_Core *orb_core_;

//  TAO_GIOP_ReplyStatusType exception_type_;
/// Exception type (will be NO_EXCEPTION in the majority of the cases).
CORBA::ULong exception_type_;

int once_only_;
};

#endif /* TAO_AMH_RESPONSE_HANDLER_H */
