// $Id$

#include "tao/TAO_Internal.h"
#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Object_Manager.h"
#include "tao/default_server.h"
#include "tao/default_client.h"
#include "tao/ORB_Core.h"

ACE_RCSID(tao, TAO_Internal, "$Id$")

#if defined (TAO_USES_STATIC_SERVICE) || defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
ACE_STATIC_SVC_REQUIRE (TAO_Resource_Factory)
ACE_STATIC_SVC_REQUIRE (TAO_Default_Server_Strategy_Factory)
ACE_STATIC_SVC_REQUIRE (TAO_Default_Client_Strategy_Factory)
#endif /* TAO_USES_STATIC_SERVICE */

int TAO_Internal::service_open_count_ = 0;
#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
char *TAO_Internal::resource_factory_args_ = TAO_DEFAULT_RESOURCE_FACTORY_ARGS;
char *TAO_Internal::server_strategy_args_ = TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS;
char *TAO_Internal::client_strategy_args_ = TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */

int
TAO_Internal::open_services (int& argc, char** argv)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, *ACE_Static_Object_Lock::instance (), -1));

  if (TAO_Internal::service_open_count_++ == 0)
    {
      int ignore_static_service = 1;
      int ignore_default_svc_conf = 0;
#if defined (TAO_USES_STATIC_SERVICE) || defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
      ignore_static_service = 0;
#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
      ignore_default_svc_conf = 1;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */
#endif /* TAO_USES_STATIC_SERVICE || TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */
      int retv = ACE_Service_Config::open (argc, argv,
                                           ACE_DEFAULT_LOGGER_KEY,
                                           ignore_static_service,
                                           ignore_default_svc_conf);
# if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
      ACE_Service_Config::process_directive
        (TAO_DEFAULT_RESOURCE_FACTORY_ARGS);
      ACE_Service_Config::process_directive
        (TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS);
      ACE_Service_Config::process_directive
        (TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS);
# endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */
      return retv;
    }
  else
    return 0;
}

TAO_Internal::TAO_Internal (void)
{
}

int
TAO_Internal::close_services (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard,
			    *ACE_Static_Object_Lock::instance (), -1));

  --service_open_count_;

  return 0;
}
