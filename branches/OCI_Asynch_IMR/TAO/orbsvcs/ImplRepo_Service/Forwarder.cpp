//=============================================================================
/**
*  @file   Forwarder.cpp
*
*  $Id$
*
*  @brief  Definition of ImR_Forwarder
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*  @author Priyanka Gontla <pgontla@doc.ece.uci.edu>
*/
//=============================================================================

#include "Forwarder.h"
#include "ImR_Locator_i.h"

#include "tao/ORB.h"
#include "tao/Object_KeyC.h"
#include "tao/ORB_Constants.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/POA_Current_Impl.h"
#include "tao/PortableServer/POA_Current.h"

#include <tao/TAO_Server_Request.h>
#include <tao/DynamicInterface/Request.h>
#include <tao/DynamicInterface/Server_Request.h>
#include <tao/DynamicInterface/AMH_DSI_Response_Handler.h>
#include <tao/Messaging/AMH_Response_Handler.h>

/**
* This constructor takes in orb and ImR_Locator_i pointers to store for later
* use.  It also grabs a reference to the POACurrent object for use in
* preinvoke.
*/
ImR_Forwarder::ImR_Forwarder (ImR_Locator_i& imr_impl)
  : locator_ (imr_impl)
  , orb_ (0)
{
}

void
ImR_Forwarder::init (CORBA::ORB_ptr orb)
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
      ACE_DEBUG ((LM_DEBUG, "ImR_Forwarder::init() Exception ignored.\n"));
    }
  ACE_ASSERT (!CORBA::is_nil (this->poa_current_var_.in ()));
}

/**
* We figure out the intended recipient from the POA name.  After activating
* the server, we throw a forwarding exception to the correct server.
*
* The big complicated thing here is that we have to create the forwarding
* ior based on what we already have.  So we combine the endpoint received
* from activate_server_i and append the objectid from the request to it.
*/
PortableServer::Servant
ImR_Forwarder::preinvoke (const PortableServer::ObjectId &,
                          PortableServer::POA_ptr poa,
                          const char *,
                          PortableServer::ServantLocator::Cookie &)
{
  ACE_ASSERT (! CORBA::is_nil(poa));
  CORBA::Object_var forward_obj;

  try
    {
      CORBA::String_var server_name = poa->the_name();

      if (locator_.debug() > 1)
        ACE_DEBUG ((LM_DEBUG, "ImR: Activating server <%s>.\n", server_name.in()));

      // The activator stores a partial ior with each server. We can
      // just tack on the current ObjectKey to get a valid ior for
      // the desired server.
      CORBA::String_var pior = locator_.activate_server_by_name (server_name.in (), false);

      ACE_CString ior = pior.in ();

      // Check that the returned ior is the expected partial ior with
      // missing ObjectKey.
      if (ior.find ("corbaloc:") != 0 || ior[ior.length () - 1] != '/')
        {
          ACE_ERROR ((LM_ERROR, "ImR_Forwarder::preinvoke () Invalid corbaloc ior.\n\t<%s>\n", ior.c_str()));
          throw CORBA::OBJECT_NOT_EXIST (
            CORBA::SystemException::_tao_minor_code (
              TAO_IMPLREPO_MINOR_CODE,
              0),
            CORBA::COMPLETED_NO);
        }

      CORBA::String_var key_str;
      // Unlike POA Current, this implementation cannot be cached.
      TAO::Portable_Server::POA_Current* tao_current =
        dynamic_cast <TAO::Portable_Server::POA_Current*> (this->poa_current_var_.in ());

      ACE_ASSERT(tao_current != 0);
      TAO::Portable_Server::POA_Current_Impl* impl = tao_current->implementation ();
      TAO::ObjectKey::encode_sequence_to_string (key_str.out (), impl->object_key ());

      ior += key_str.in();

      if (locator_.debug() > 0)
        ACE_DEBUG ((LM_DEBUG, "ImR: Forwarding invocation on <%s> to <%s>\n", server_name.in(), ior.c_str()));

      forward_obj =
        this->orb_->string_to_object (ior.c_str ());
    }
  catch (const ImplementationRepository::CannotActivate&)
    {
      throw CORBA::TRANSIENT (
        CORBA::SystemException::_tao_minor_code (
          TAO_IMPLREPO_MINOR_CODE,
          0),
        CORBA::COMPLETED_NO);
    }
  catch (const ImplementationRepository::NotFound&)
    {
      throw CORBA::TRANSIENT (
        CORBA::SystemException::_tao_minor_code (
          TAO_IMPLREPO_MINOR_CODE,
          0),
        CORBA::COMPLETED_NO);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Forwarder");
      throw CORBA::TRANSIENT (
        CORBA::SystemException::_tao_minor_code (
          TAO_IMPLREPO_MINOR_CODE,
          0),
        CORBA::COMPLETED_NO);
    }

  if (!CORBA::is_nil (forward_obj.in ()))
    throw PortableServer::ForwardRequest (forward_obj.in ());

  ACE_ERROR ((LM_ERROR, "Error: Forward_to reference is nil.\n"));
  throw CORBA::OBJECT_NOT_EXIST (
    CORBA::SystemException::_tao_minor_code (
      TAO_IMPLREPO_MINOR_CODE,
      0),
    CORBA::COMPLETED_NO);
}

void
ImR_Forwarder::postinvoke (const PortableServer::ObjectId &,
                           PortableServer::POA_ptr,
                           const char *,
                           PortableServer::ServantLocator::Cookie,
                           PortableServer::Servant)
{
}

//-------------------------------------------------------------------------------------

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
      ACE_DEBUG ((LM_DEBUG, "ImR_Forwarder::init() Exception ignored.\n"));
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

  ImR_ReplyHandler * rh = new ImR_ReplyHandler(key_str.in(),
                                               this->locator_.debug() > 0 ?
                                               server_name.in() : "",
                                               this->orb_, resp);
  CORBA::String_var pior = this->locator_.activate_server_by_name (server_name.in(), false);

  rh->send_ior (pior.in());
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


ImR_ReplyHandler::ImR_ReplyHandler (const char *key,
                                    const char *server_name,
                                    CORBA::ORB_ptr orb,
                                    TAO_AMH_DSI_Response_Handler_ptr resp)
  :key_str_ (key),
   server_name_ (server_name),
   orb_(CORBA::ORB::_duplicate (orb)),
   resp_ (resp)
{
}

ImR_ReplyHandler::~ImR_ReplyHandler (void)
{
}

void
ImR_ReplyHandler::send_ior (const char *pior)
{
  ACE_CString ior = pior;

  // Check that the returned ior is the expected partial ior with
  // missing ObjectKey.
  if (ior.find ("corbaloc:") != 0 || ior[ior.length () - 1] != '/')
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ImR_ReplyHandler::send_ior (): Invalid corbaloc ior.\n")
                  ACE_TEXT ("\t<%s>\n"),
                  ior.c_str()));

      CORBA::OBJECT_NOT_EXIST ex (CORBA::SystemException::_tao_minor_code
                                  ( TAO_IMPLREPO_MINOR_CODE, 0),
                                  CORBA::COMPLETED_NO);
      TAO_AMH_DSI_Exception_Holder h(&ex);
      this->resp_->invoke_excep(&h);
      delete this;
      return;
    }

  ior += this->key_str_.in();

  if (this->server_name_[0] != 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ImR_ReplyHandler::send_ior(): Forwarding invocation on <%s> ")
                ACE_TEXT ("to <%s>\n"),
                this->server_name_.in(), ior.c_str()));

  CORBA::Object_var forward_obj;
    this->orb_->string_to_object (ior.c_str ());

  if (!CORBA::is_nil (forward_obj.in ()))
    {
      PortableServer::ForwardRequest fwd (forward_obj.in ());
      TAO_AMH_DSI_Exception_Holder h(&fwd);
      this->resp_->invoke_excep(&h);
      delete this;
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ImR_ReplyHandler::send_ior (): Forward_to ")
                  ACE_TEXT ("reference is nil.\n")));

      CORBA::OBJECT_NOT_EXIST ex (CORBA::SystemException::_tao_minor_code
                                  ( TAO_IMPLREPO_MINOR_CODE, 0),
                                  CORBA::COMPLETED_NO);
      TAO_AMH_DSI_Exception_Holder h(&ex);
      this->resp_->invoke_excep(&h);
      delete this;
    }
}

void
ImR_ReplyHandler::send_exception (void)
{
  CORBA::TRANSIENT ex (CORBA::SystemException::_tao_minor_code
                       ( TAO_IMPLREPO_MINOR_CODE, 0),
                       CORBA::COMPLETED_NO);
  TAO_AMH_DSI_Exception_Holder h(&ex);
  resp_->invoke_excep(&h);
  delete this;
}
