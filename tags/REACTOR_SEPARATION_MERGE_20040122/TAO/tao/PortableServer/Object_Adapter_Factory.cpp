// $Id$

#include "Object_Adapter_Factory.h"
#include "Object_Adapter.h"
#include "PortableServer_ORBInitializer.h"
#include "tao/ORB_Core.h"

TAO_Object_Adapter_Factory::TAO_Object_Adapter_Factory (void)
{
}

TAO_Adapter*
TAO_Object_Adapter_Factory::create (TAO_ORB_Core *orb_core)
{
  return new TAO_Object_Adapter (orb_core->server_factory ()->
                                    active_object_map_creation_parameters (),
                                 *orb_core);
}

int
TAO_Object_Adapter_Factory::init (int /* argc */,
                                  ACE_TCHAR* /* argv */ [])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      /// Register the Messaging ORBInitializer.
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_PortableServer_ORBInitializer,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;

      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P | %t) Caught exception:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

ACE_FACTORY_DEFINE (TAO_PortableServer, TAO_Object_Adapter_Factory)
ACE_STATIC_SVC_DEFINE (TAO_Object_Adapter_Factory,
                       ACE_TEXT ("TAO_Object_Adapter_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Object_Adapter_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

