// $Id$

#include "Dynamic_Implementation.h"

ACE_RCSID(DynamicInterface, Dynamic_Implementation, "$Id$")

#if (TAO_HAS_MINIMUM_CORBA == 0)

#include "Server_Request.h"
#include "tao/ORB_Core.h"
#include "tao/PortableServer/POA.h"
#include "tao/PortableServer/Collocated_Object.h"

CORBA::Object_ptr
TAO_DynamicImplementation::_this (CORBA::Environment &ACE_TRY_ENV)
{
  // The _this() function returns a CORBA::Object_ptr for the target
  // object. Unlike _this() for static skeletons, its return type is
  // not interface-specific because a DSI servant may very well
  // incarnate multiple CORBA objects of different types.
  TAO_Stub *stub = this->_create_stub (ACE_TRY_ENV);
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
TAO_DynamicImplementation::_create_stub (CORBA::Environment &ACE_TRY_ENV)
{
  // If DynamicImplementation::_this() is invoked outside of the
  // context of a request invocation on a target object being served
  // by the DSI servant, it raises the PortableServer::WrongPolicy
  // exception.
  TAO_POA_Current_Impl *poa_current_impl =
    ACE_static_cast(TAO_POA_Current_Impl *,
                    TAO_TSS_RESOURCES::instance ()->poa_current_impl_);

  if (poa_current_impl == 0 
      || this != poa_current_impl->servant ())
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

  PortableServer::POA_var poa = poa_current_impl->get_POA (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA::RepositoryId interface = 
    this->_primary_interface (poa_current_impl->object_id (),
                              poa.in (),
                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA::PolicyList_var client_exposed_policies =
    poa_current_impl->poa ()->client_exposed_policies (
                                  poa_current_impl->priority (),
                                  ACE_TRY_ENV
                                );
  ACE_CHECK_RETURN (0);

  // @@ PPOA
  // @@ return orb->create_stub_object (
  // @@ client_exposed_policies._retn (),
  // @@ poa_current_impl->poa (),
  // @@ ACE_TRY_ENV);
  return poa_current_impl->poa ()->key_to_stub (
                                       poa_current_impl->object_key (),
                                       interface,
                                       poa_current_impl->priority (),
                                       ACE_TRY_ENV
                                     );
}

void
TAO_DynamicImplementation::_dispatch (TAO_ServerRequest &request,
                                      void *context,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (context);

  // Create DSI request object.
  CORBA::ServerRequest *dsi_request = 0;
  ACE_NEW (dsi_request,
           CORBA::ServerRequest (request));

  // Delegate to user.
  this->invoke (dsi_request, 
                ACE_TRY_ENV);
  ACE_CHECK;

  if (request.response_expected ())
    {
      request.init_reply ();

      dsi_request->dsi_marshal (ACE_TRY_ENV);
      ACE_CHECK;
    }

   ACE_TRY
     {
       if ((!request.sync_with_server () && request.response_expected ()))
         {
           request.tao_send_reply ();
           ACE_TRY_CHECK;
         }
     }
   ACE_CATCH(CORBA::Exception,ex)
     {
       request.tao_send_reply_exception(ex);
     }
   ACE_ENDTRY;

   CORBA::release (dsi_request);
}

#endif /* TAO_HAS_MINIMUM_CORBA */
