// $Id$

#include "tao/TAO_Internal.h"
#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Object_Manager.h"
#include "tao/default_server.h"
#include "tao/default_client.h"
#include "tao/default_resource.h"

ACE_RCSID(tao, TAO_Internal, "$Id$")

int TAO_Internal::service_open_count_ = 0;
#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
char *TAO_Internal::resource_factory_args_ = TAO_DEFAULT_RESOURCE_FACTORY_ARGS;
char *TAO_Internal::server_strategy_args_ = TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS;
char *TAO_Internal::client_strategy_args_ = TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS;
#else
char *TAO_Internal::resource_factory_args_ = 0;
char *TAO_Internal::server_strategy_args_ = 0;
char *TAO_Internal::client_strategy_args_ = 0;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */

void
TAO_Internal::default_svc_conf_entries (const char *resource_factory_args,
                                        const char *server_strategy_args,
                                        const char *client_strategy_args)
{
  TAO_Internal::resource_factory_args_ = resource_factory_args;
  TAO_Internal::server_strategy_args_ = server_strategy_args;
  TAO_Internal::client_strategy_args_ = client_strategy_args;
}

int
TAO_Internal::open_services (int& argc, char** argv, int ignore_default_svc_conf)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, *ACE_Static_Object_Lock::instance (), -1));
#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
  ignore_default_svc_conf = 1;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */

  if (TAO_Internal::service_open_count_++ == 0)
    {
      ACE_Service_Config::static_svcs ()->
        insert (&ace_svc_desc_TAO_Default_Resource_Factory);
      ACE_Service_Config::static_svcs ()->
        insert (&ace_svc_desc_TAO_Default_Client_Strategy_Factory);
      ACE_Service_Config::static_svcs ()->
        insert (&ace_svc_desc_TAO_Default_Server_Strategy_Factory);

      int retv = ACE_Service_Config::open (argc, argv,
                                           ACE_DEFAULT_LOGGER_KEY,
                                           0, // Don't ignore static services.
                                           ignore_default_svc_conf);

      if (TAO_Internal::resource_factory_args_ != 0)
        ACE_Service_Config::process_directive (TAO_Internal::resource_factory_args_);
      if (TAO_Internal::client_strategy_args_ != 0)
        ACE_Service_Config::process_directive (TAO_Internal::client_strategy_args_);
      if (TAO_Internal::server_strategy_args_ != 0)
        ACE_Service_Config::process_directive (TAO_Internal::server_strategy_args_);

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
