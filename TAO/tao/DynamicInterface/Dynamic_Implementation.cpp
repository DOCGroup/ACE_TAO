// $Id$

#include "Dynamic_Implementation.h"

ACE_RCSID (DynamicInterface,
           Dynamic_Implementation,
           "$Id$")

#include "Server_Request.h"
#include "tao/ORB_Core.h"
#include "tao/IFR_Client_Adapter.h"
#include "tao/PortableServer/POA.h"
#include "tao/PortableServer/Collocated_Object.h"

#include "ace/Dynamic_Service.h"

CORBA::Boolean
TAO_DynamicImplementation::_is_a (const char *logical_type_id
                                  ACE_ENV_ARG_DECL)
{
  CORBA::RepositoryId_var id =
    this->get_id_from_primary_interface (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return ACE_OS::strcmp (logical_type_id, id.in ()) == 0;
}

CORBA::Object_ptr
TAO_DynamicImplementation::_this (ACE_ENV_SINGLE_ARG_DECL)
{
  // The _this() function returns a CORBA::Object_ptr for the target
  // object. Unlike _this() for static skeletons, its return type is
  // not interface-specific because a DSI servant may very well
  // incarnate multiple CORBA objects of different types.
  TAO_Stub *stub = this->_create_stub (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Create a object.
  TAO_Collocated_Object *retval = 0;
  ACE_NEW_RETURN (retval,
                  TAO_Collocated_Object (stub,
                                         1,
                                         this),
                  CORBA::Object::_nil ());

  return retval;
}

CORBA::InterfaceDef_ptr
TAO_DynamicImplementation::_get_interface (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_IFR_Client_Adapter *adapter =
    ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
        TAO_ORB_Core::ifr_client_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTF_REPOS (),
                        0);
    }

  CORBA::RepositoryId_var id =
    this->get_id_from_primary_interface (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // This doesn't take multiple ORBs into account, but it's being
  // used only to resolve the IFR, so we should be ok.
  return adapter->get_interface (TAO_ORB_Core_instance ()->orb (),
                                 id.in ()
                                 ACE_ENV_ARG_PARAMETER);
}

const char *
TAO_DynamicImplementation::_interface_repository_id (void) const
{
  // This should never be called.
  return 0;
}

void *
TAO_DynamicImplementation::_downcast (const char *repository_id)
{
  ACE_UNUSED_ARG (repository_id);

  // Don't know enough to do better.
  return this;
}

TAO_Stub *
TAO_DynamicImplementation::_create_stub (ACE_ENV_SINGLE_ARG_DECL)
{
  // If DynamicImplementation::_this() is invoked outside of the
  // context of a request invocation on a target object being served
  // by the DSI servant, it raises the PortableServer::WrongPolicy
  // exception. See the CORBA C++ mapping, section 1.38.3.
  TAO_POA_Current_Impl *poa_current_impl =
    ACE_static_cast (TAO_POA_Current_Impl *,
                     TAO_TSS_RESOURCES::instance ()->poa_current_impl_);

  if (poa_current_impl == 0
      || this != poa_current_impl->servant ())
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

  PortableServer::POA_var poa =
    poa_current_impl->get_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::PolicyList_var client_exposed_policies =
    poa_current_impl->poa ()->client_exposed_policies (
        poa_current_impl->priority ()
        ACE_ENV_ARG_PARAMETER
      );
  ACE_CHECK_RETURN (0);

  CORBA::RepositoryId_var pinterface =
    this->_primary_interface (poa_current_impl->object_id (),
                              poa.in ()
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return
    poa_current_impl->poa ()->key_to_stub (poa_current_impl->object_key (),
                                           pinterface.in (),
                                           poa_current_impl->priority ()
                                           ACE_ENV_ARG_PARAMETER);
}

void
TAO_DynamicImplementation::_dispatch (TAO_ServerRequest &request,
                                      void * /* context */
                                      ACE_ENV_ARG_DECL)
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

  ACE_TRY
    {
      // Delegate to user.
      this->invoke (dsi_request
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Only if the client is waiting.
      if (request.response_expected () && !request.sync_with_server ())
        {
          dsi_request->dsi_marshal (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
      // Only if the client is waiting.
      if (request.response_expected () && !request.sync_with_server ())
        {
          request.tao_send_reply_exception (ex);
        }
    }
  ACE_ENDTRY;

  CORBA::release (dsi_request);
}

CORBA::RepositoryId
TAO_DynamicImplementation::get_id_from_primary_interface (
    ACE_ENV_SINGLE_ARG_DECL
  )
{
  // If this method is called outside of the
  // context of a request invocation on a target object being served
  // by the DSI servant, it raises the PortableServer::WrongPolicy
  // exception. See the CORBA C++ mapping, section 1.38.3.
  TAO_POA_Current_Impl *poa_current_impl =
    ACE_static_cast (TAO_POA_Current_Impl *,
                     TAO_TSS_RESOURCES::instance ()->poa_current_impl_);

  if (poa_current_impl == 0
      || this != poa_current_impl->servant ())
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

  PortableServer::POA_var poa =
    poa_current_impl->get_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->_primary_interface (poa_current_impl->object_id (),
                                   poa.in ()
                                   ACE_ENV_ARG_PARAMETER);
}
