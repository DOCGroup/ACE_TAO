//=============================================================================
/**
*  @file   Forwarder.cpp
*
*  $Id$
*
*  @brief  Definition of ImR_DSI_Forwarder
*
*  @author Phil Mesnier <mesnier_p@ociweb.com>
*/
//=============================================================================

#include "orbsvcs/Log_Macros.h"
#include "Forwarder.h"
#include "ImR_Locator_i.h"

#include "tao/ORB.h"
#include "tao/Object_KeyC.h"
#include "tao/ORB_Constants.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/POA_Current_Impl.h"
#include "tao/PortableServer/POA_Current.h"

#include "tao/TAO_Server_Request.h"
#include "tao/DynamicInterface/Request.h"
#include "tao/DynamicInterface/Server_Request.h"
#include "tao/DynamicInterface/AMH_DSI_Response_Handler.h"
#include "tao/Messaging/AMH_Response_Handler.h"

ImR_DSI_Forwarder::ImR_DSI_Forwarder (ImR_Locator_i& imr_impl)
  : locator_ (imr_impl)
  , orb_ (0)
{
}

ImR_DSI_Forwarder::~ImR_DSI_Forwarder (void)
{
}

void
ImR_DSI_Forwarder::init (CORBA::ORB_ptr orb)
{
  ACE_ASSERT (! CORBA::is_nil(orb));
  this->orb_ = orb;
  try
    {
      CORBA::Object_var tmp =
        orb->resolve_initial_references ("POACurrent");

      this->poa_current_var_ =
        PortableServer::Current::_narrow (tmp.in ());
    }
  catch (const CORBA::Exception&)
    {
    }
  ACE_ASSERT (!CORBA::is_nil (this->poa_current_var_.in ()));
}

void
ImR_DSI_Forwarder::_dispatch (TAO_ServerRequest &request,
                              TAO::Portable_Server::Servant_Upcall * /*context */ )
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
      TAO_AMH_DSI_Response_Handler_var rh;
      ACE_NEW (rh, TAO_AMH_DSI_Response_Handler(request));

      rh->init (request, 0);
      // Delegate to user.
      this->invoke (dsi_request, rh.in());
    }
  catch (CORBA::Exception& ex)
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
ImR_DSI_Forwarder::invoke (CORBA::ServerRequest_ptr )
{
  // unused method, only asynch invocations are used
}

char *
ImR_DSI_Forwarder::_primary_interface (const PortableServer::ObjectId&,
                                       PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:Object:1.0");
}

void
ImR_DSI_Forwarder::invoke (CORBA::ServerRequest_ptr request,
                           TAO_AMH_DSI_Response_Handler_ptr resp)
{
  bool is_oneway = !request->_tao_server_request().response_expected()
    || request->_tao_server_request().sync_with_server();

  if (is_oneway)
    {
      CORBA::release (resp);
      return; // nothing else to do?
    }

  PortableServer::POA_var poa = this->poa_current_var_->get_POA();
  PortableServer::ObjectId_var oid = this->poa_current_var_->get_object_id ();

  CORBA::String_var server_name = poa->the_name();

  CORBA::String_var key_str;
  // Unlike POA Current, this implementation cannot be cached.
  TAO::Portable_Server::POA_Current* tao_current =
    dynamic_cast <TAO::Portable_Server::POA_Current*> (this->poa_current_var_.in ());

  ACE_ASSERT(tao_current != 0);
  TAO::Portable_Server::POA_Current_Impl* impl = tao_current->implementation ();
  TAO::ObjectKey::encode_sequence_to_string (key_str.out (), impl->object_key ());

  ImR_DSI_ResponseHandler * rh = 0;
  ACE_NEW (rh, ImR_DSI_ResponseHandler(key_str.in(),
                                    this->locator_.debug() > 0 ?
                                    server_name.in() : "",
                                    this->orb_, resp));
  this->locator_.activate_server_by_name (server_name.in(), false, rh);
}

void
ImR_DSI_Forwarder::invoke_get_interface(CORBA::ServerRequest_ptr )
{
  // no impl
}

void
ImR_DSI_Forwarder::invoke_primary_interface(CORBA::ServerRequest_ptr )
{
  // no impl
}


//--------------------------------------------------------------------

ImR_DSI_ResponseHandler::ImR_DSI_ResponseHandler (const char *key,
                                            const char *server_name,
                                            CORBA::ORB_ptr orb,
                                            TAO_AMH_DSI_Response_Handler_ptr resp)
  :key_str_ (key),
   server_name_ (server_name),
   orb_(CORBA::ORB::_duplicate (orb)),
   resp_ (TAO_AMH_DSI_Response_Handler::_duplicate (resp))
{
}

ImR_DSI_ResponseHandler::~ImR_DSI_ResponseHandler (void)
{
}

void
ImR_DSI_ResponseHandler::send_ior (const char *pior)
{
  ACE_CString ior = pior;

  // Check that the returned ior is the expected partial ior with
  // missing ObjectKey.
  if (ior.find ("corbaloc:") == 0 && ior[ior.length () -1] == '/')
    {
      ior += this->key_str_.in();

      CORBA::Object_var forward_obj =
        this->orb_->string_to_object (ior.c_str ());

      if (!CORBA::is_nil (forward_obj.in ()))
        {
          this->resp_->invoke_location_forward(forward_obj.in(), false);
          delete this;
          return;
        }
      else
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("ImR_DSI_ResponseHandler::send_ior (): Forward_to ")
                      ACE_TEXT ("reference is nil.\n")));
        }
    }
  else
    {
      ORBSVCS_ERROR ((LM_ERROR,
                  ACE_TEXT ("ImR_ResponseHandler::send_ior (): Invalid corbaloc ior.\n")
                  ACE_TEXT ("\t<%s>\n"),
                  ior.c_str()));
    }

  this->invoke_excep_i (new CORBA::OBJECT_NOT_EXIST
                        (CORBA::SystemException::_tao_minor_code
                         ( TAO_IMPLREPO_MINOR_CODE, 0),
                         CORBA::COMPLETED_NO));
}

void
ImR_DSI_ResponseHandler::invoke_excep_i (CORBA::Exception *ex)
{
  TAO_AMH_DSI_Exception_Holder h(ex);
  this->resp_->invoke_excep(&h);
  delete this;
}

void
ImR_DSI_ResponseHandler::send_exception (CORBA::Exception *ex)
{
  //discard the exception, always throw a transient:
  delete ex;

  this->invoke_excep_i (new CORBA::TRANSIENT
                        (CORBA::SystemException::_tao_minor_code
                         (TAO_IMPLREPO_MINOR_CODE, 0),
                         CORBA::COMPLETED_NO));
}
