#include "LB_ClientComponent.h"
#include "LB_ClientORBInitializer.h"


ACE_RCSID (LoadBalancing,
           LB_ClientComponent,
           "$Id$")


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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Register the LB_ClientComponent ORB initializer.
      PortableInterceptor::ORBInitializer_ptr tmp;
      ACE_NEW_THROW_EX (tmp,
                        TAO_LB_ClientORBInitializer,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;

      PortableInterceptor::ORBInitializer_var initializer = tmp;

      PortableInterceptor::register_orb_initializer (initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unable to register LB_ClientComponent ORB "
                           "initializer.");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}


ACE_STATIC_SVC_DEFINE (TAO_LB_ClientComponent,
                       ACE_TEXT ("LB_ClientComponent"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_LB_ClientComponent),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_LoadBalancing, TAO_LB_ClientComponent)
