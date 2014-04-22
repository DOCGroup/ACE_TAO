// $Id$

#include "orbsvcs/LoadBalancing/LB_ClientComponent.h"
#include "orbsvcs/LoadBalancing/LB_ClientORBInitializer.h"

#include "tao/ORB_Constants.h"
#include "tao/ORBInitializer_Registry.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_LB_ClientComponent::init (int /* argc */, ACE_TCHAR * /* argv */[])
{
  return this->register_orb_initializer ();
}

int
TAO_LB_ClientComponent::fini (void)
{
  return 0;
}

int
TAO_LB_ClientComponent::register_orb_initializer (void)
{
  try
    {
      // Register the LB_ClientComponent ORB initializer.
      PortableInterceptor::ORBInitializer_ptr tmp;
      ACE_NEW_THROW_EX (tmp,
                        TAO_LB_ClientORBInitializer,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      PortableInterceptor::ORBInitializer_var initializer = tmp;

      PortableInterceptor::register_orb_initializer (initializer.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unable to register LB_ClientComponent ORB ""initializer.");
      return -1;
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_LB_ClientComponent,
                       ACE_TEXT ("LB_ClientComponent"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_LB_ClientComponent),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_LoadBalancing, TAO_LB_ClientComponent)
