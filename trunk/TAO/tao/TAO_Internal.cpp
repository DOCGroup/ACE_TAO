// $Id$

#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Object_Manager.h"
#include "tao/default_server.h"
#include "tao/default_client.h"
#include "tao/TAO_Internal.h"

ACE_RCSID(tao, TAO_Internal, "$Id$")

#if defined (TAO_USES_STATIC_SERVICE) || defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
ACE_STATIC_SVC_REQUIRE (TAO_Resource_Factory)
ACE_STATIC_SVC_REQUIRE (TAO_Default_Server_Strategy_Factory)
ACE_STATIC_SVC_REQUIRE (TAO_Default_Client_Strategy_Factory)
#endif /* TAO_USES_STATIC_SERVICE */

int TAO_Internal::service_open_count_ = 0;

int
TAO_Internal::open_services (int& argc, char** argv)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, *ACE_Static_Object_Lock::instance (), -1));

  if (TAO_Internal::service_open_count_++ == 0)
#if defined (TAO_USES_STATIC_SERVICE) || defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
    {
      int retv = ACE_Service_Config::open (argc, argv,
                                           ACE_DEFAULT_LOGGER_KEY, 0);
#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
      ACE_Service_Config::process_directive
        (TAO_DEFAULT_RESOURCE_FACTORY_ARGS);
      ACE_Service_Config::process_directive
        (TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS);
      ACE_Service_Config::process_directive
        (TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS);
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */
      return retv;
    }
#else
    return ACE_Service_Config::open (argc, argv);
#endif /* TAO_USES_STATIC_SERVICE */
  else
    return 0;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */
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
