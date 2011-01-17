// -*- C++ -*-
// $Id$

#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "tao/DynamicInterface/Server_Request.h"
#include "tao/ORB_Core.h"
#include "tao/TSS_Resources.h"
#include "tao/IFR_Client_Adapter.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/POA_Current_Impl.h"
#include "tao/PortableServer/Collocated_Arguments_Converter.h"
#include "tao/operation_details.h"

#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_string.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Boolean
TAO_DynamicImplementation::_is_a (const char *logical_type_id)
{
  CORBA::RepositoryId_var id = this->get_id_from_primary_interface ();

  return ACE_OS::strcmp (logical_type_id, id.in ()) == 0;
}

CORBA::Object_ptr
TAO_DynamicImplementation::_this (void)
{
  // The _this() function returns a CORBA::Object_ptr for the target
  // object. Unlike _this() for static skeletons, its return type is
  // not interface-specific because a DSI servant may very well
  // incarnate multiple CORBA objects of different types.
  TAO_Stub *stub = this->_create_stub ();

  // Create a object.
  CORBA::Object_ptr retval = CORBA::Object_ptr ();
  ACE_NEW_RETURN (retval,
                  CORBA::Object (stub,
                                 1,
                                 this),
                  CORBA::Object::_nil ());

  return retval;
}

CORBA::InterfaceDef_ptr
TAO_DynamicImplementation::_get_interface (void)
{
  TAO_IFR_Client_Adapter *adapter =
    ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
        TAO_ORB_Core::ifr_client_adapter_name ());

  if (adapter == 0)
    {
      throw ::CORBA::INTF_REPOS ();
    }

  CORBA::RepositoryId_var id = this->get_id_from_primary_interface ();

  // This doesn't take multiple ORBs into account, but it's being
  // used only to resolve the IFR, so we should be ok.
  return adapter->get_interface (TAO_ORB_Core_instance ()->orb (), id.in ());
}

const char *
TAO_DynamicImplementation::_interface_repository_id (void) const
{
  // This should never be called.
  return 0;
}

void *
TAO_DynamicImplementation::_downcast (const char *)
{
  // Don't know enough to do better.
  return this;
}

TAO_Stub *
TAO_DynamicImplementation::_create_stub (void)
{
  // If DynamicImplementation::_this() is invoked outside of the
  // context of a request invocation on a target object being served
  // by the DSI servant, it raises the PortableServer::WrongPolicy
  // exception. See the CORBA C++ mapping, section 1.38.3.
  TAO::Portable_Server::POA_Current_Impl *poa_current_impl =
    static_cast <TAO::Portable_Server::POA_Current_Impl *>
                     (TAO_TSS_Resources::instance ()->poa_current_impl_);

  if (poa_current_impl == 0
      || this != poa_current_impl->servant ())
    {
      throw PortableServer::POA::WrongPolicy ();
    }

  PortableServer::POA_var poa =
    poa_current_impl->get_POA ();

  CORBA::PolicyList_var client_exposed_policies =
    poa_current_impl->poa ()->client_exposed_policies (
        poa_current_impl->priority ());

  CORBA::RepositoryId_var pinterface =
    this->_primary_interface (poa_current_impl->object_id (), poa.in ());

  return
    poa_current_impl->poa ()->key_to_stub (poa_current_impl->object_key (),
                                           pinterface.in (),
                                           poa_current_impl->priority ());
}

void
TAO_DynamicImplementation::_dispatch (TAO_ServerRequest &request,
                                      void * /* context */)
{
  // No need to do any of this if the client isn't waiting.
  if (request.response_expected ())
    {
      if (request.is_forwarded ())
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
      // Delegate to user.
      this->invoke (dsi_request);

      // Only if the client is waiting.
      if (request.response_expected () && !request.sync_with_server ())
        {
          dsi_request->dsi_marshal ();
        }
    }
  catch (::CORBA::Exception& ex)
    {
      // Only if the client is waiting.
      if (request.response_expected () && !request.sync_with_server ())
        {
          if (request.collocated ()
               && request.operation_details ()->cac () != 0)
            {
              // If we have a cac it will handle the exception and no
              // need to do any further processing
              request.operation_details ()->cac ()->handle_corba_exception (
                request, &ex);
              return;
            }
          else
            request.tao_send_reply_exception (ex);
        }
    }

  ::CORBA::release (dsi_request);
}

CORBA::RepositoryId
TAO_DynamicImplementation::get_id_from_primary_interface (void)
{
  // If this method is called outside of the
  // context of a request invocation on a target object being served
  // by the DSI servant, it raises the PortableServer::WrongPolicy
  // exception. See the CORBA C++ mapping, section 1.38.3.
  TAO::Portable_Server::POA_Current_Impl *poa_current_impl =
    static_cast <TAO::Portable_Server::POA_Current_Impl *>
                     (TAO_TSS_Resources::instance ()->poa_current_impl_);

  if (poa_current_impl == 0
      || this != poa_current_impl->servant ())
    {
      throw PortableServer::POA::WrongPolicy ();
    }

  PortableServer::POA_var poa = poa_current_impl->get_POA ();

  return this->_primary_interface (poa_current_impl->object_id (), poa.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
