// $Id$

#include "test_dsi.h"
#include "tao/DynamicInterface/Request.h"
#include "tao/DynamicInterface/Unknown_User_Exception.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "My_DII_Reply_Handler.h"

#if !defined(__ACE_INLINE__)
#include "test_dsi.inl"
#endif /* __ACE_INLINE__ */

void
DSI_Simple_Server::_dispatch (TAO_ServerRequest &request, TAO::Portable_Server::Servant_Upcall *)
{
  // No need to do any of this if the client isn't waiting.
  if (request.response_expected ())
    {
      if (!CORBA::is_nil (request.forward_location ()))
        {
          request.init_reply ();
          request.tao_send_reply ();

          // No need to invoke in this case.
          return;
        }
      else if (request.sync_with_server ())
        {
          // The last line before the call to this function
          // was an ACE_CHECK_RETURN, so if we're here, we
          // know there is no exception so far, and that's all
          // a SYNC_WITH_SERVER client request cares about.
          request.send_no_exception_reply ();
        }
    }

  // Create DSI request object.
  CORBA::ServerRequest *dsi_request = 0;
  ACE_NEW (dsi_request,
           CORBA::ServerRequest (request));

  try
    {
      TAO_AMH_DSI_Response_Handler_ptr rh_ptr;
      ACE_NEW (rh_ptr, TAO_AMH_DSI_Response_Handler(request));

      TAO_AMH_DSI_Response_Handler_var rh(rh_ptr);
      rh->init (request, 0);
      // Delegate to user.
      this->invoke (dsi_request, rh.in());
    }
  catch (const CORBA::Exception& ex)

    {
      // Only if the client is waiting.
      if (request.response_expected () && !request.sync_with_server ())
        {
          request.tao_send_reply_exception (ex);
        }
    }

  CORBA::release (dsi_request);
}

void
DSI_Simple_Server::invoke (CORBA::ServerRequest_ptr)
{
}

void
DSI_Simple_Server::invoke (CORBA::ServerRequest_ptr request,
                           TAO_AMH_DSI_Response_Handler * rph)
{
  CORBA::NVList_ptr opList;
  this->orb_->create_list (0, opList);

  request->arguments (opList);

  CORBA::Request_var target_request;

  this->target_->_create_request (0, // ctx
                                  request->operation (),
                                  opList,
                                  0, // result
                                  0, // exception_list,
                                  0, // context_list,
                                  target_request.inout (),
                                  0);

  target_request->_tao_lazy_evaluation (1);

  // Outgoing request must have the same byte order as the incoming one.
  target_request->_tao_byte_order (request->_tao_incoming_byte_order ());

  try
    {
      // Updates the byte order state, if necessary.
      TAO_DII_Reply_Handler_ptr rh_ptr;
      ACE_NEW (rh_ptr, My_DII_Reply_Handler (rph, this->orb_));
      TAO_DII_Reply_Handler_var rh(rh_ptr);
      target_request->sendc (rh.in());
    }
  catch (const CORBA::UNKNOWN&)
    {
      // Outgoing reply must have the same byte order as the incoming one.
      request->_tao_reply_byte_order (target_request->_tao_byte_order ());

      request->gateway_exception_reply (
          target_request->raw_user_exception ());
      return;
    }

  // Regular replies are handled by the DII AMI/DSI AMH reply handler
  // combination without further involvement of the request objects.
  // The DII AMI reply handler must propagate the incoming reply
  // byte order to the DSI AMH reply handler

  if (ACE_OS::strcmp ("shutdown", request->operation ()) == 0)
    {
      this->orb_->shutdown (0);
    }
}

CORBA::RepositoryId
DSI_Simple_Server::_primary_interface (const PortableServer::ObjectId &,
                                       PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:Simple_Server:1.0");
}

PortableServer::POA_ptr
DSI_Simple_Server::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
