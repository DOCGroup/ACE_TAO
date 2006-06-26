//$Id$
#define ACE_BUILD_SVC_DLL
#include "Smart_Test_Factory.h"
#if defined (ACE_OPENVMS)
// need this for ACE_Static_Svc_Registrar
# include "ace/Service_Config.h"
#endif

Smart_Test_Factory::Smart_Test_Factory (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Smart_Test_Factory\n"));
}

Test_ptr
Smart_Test_Factory::create_proxy (Test_ptr proxy
                                  ACE_ENV_ARG_DECL_NOT_USED)
 {
   ACE_DEBUG ((LM_DEBUG,
               "create_smart_proxy\n"));

   if (CORBA::is_nil (proxy) == 0)
     ACE_NEW_RETURN (proxy, Smart_Test_Proxy (proxy), 0);

   return proxy;

 }

// The following Factory is used by the <ACE_Service_Config> to
// dynamically initialize the state of the Smart_Proxy_Factory
ACE_SVC_FACTORY_DEFINE (Smart_Test_Factory)
