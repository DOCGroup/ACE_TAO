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
#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  char *rfactory_args[] = TAO_DEFAULT_RESOURCE_FACTORY_ARGS;
  char *client_args[] = TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS;
  char *server_args[] = TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS;
  return TAO_Internal::open_services
    (sizeof rfactory_args / sizeof rfactory_args[0], rfactory_args,
     sizeof client_args / sizeof client_args[0], client_args,
     sizeof server_args / sizeof server_args[0], server_args);
#else
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, *ACE_Static_Object_Lock::instance (), -1));

  if (TAO_Internal::service_open_count_++ == 0)
#if defined (TAO_USES_STATIC_SERVICE)
    return ACE_Service_Config::open (argc, argv, ACE_DEFAULT_LOGGER_KEY, 0);
#else
    return ACE_Service_Config::open (argc, argv);
#endif /* TAO_USES_STATIC_SERVICE */
  else
    return 0;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */
}

int
TAO_Internal::open_services (int rargc, char *resource_factory_args[],
                             int cargc, char *client_factory_args[],
                             int sargc, char *server_factory_args[])
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, *ACE_Static_Object_Lock::instance (), -1));

  if (TAO_Internal::service_open_count_++ == 0)
    {
#define FAKE_SVC_ENTRY(svcname, svctype, argc, argv) \
  do \
    { \
      ACE_Service_Object *obj = _make_##svctype (0); \
      obj->init (argc, argv); \
      ACE_Service_Repository::instance ()->insert \
        (new ACE_Service_Type (svcname,\
                               new ACE_Service_Object_Type\
                                 (obj, svcname, \
                                    (ACE_Service_Type::DELETE_OBJ \
                                     | ACE_Service_Type::DELETE_THIS)), \
                               0, 1));\
    }\
  while (0) //;

      FAKE_SVC_ENTRY ("Resource_Factory",
                      TAO_Resource_Factory,
                      rargc,
                      resource_factory_args);

      FAKE_SVC_ENTRY ("Client_Strategy_Factory",
                      TAO_Default_Client_Strategy_Factory,
                      cargc,
                      client_factory_args);

      FAKE_SVC_ENTRY ("Server_Strategy_Factory",
                      TAO_Default_Server_Strategy_Factory,
                      sargc,
                      server_factory_args);
    }
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
