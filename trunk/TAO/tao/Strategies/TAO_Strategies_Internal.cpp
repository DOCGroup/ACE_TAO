// $Id$

#include "TAO_Strategies_Internal.h"
#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Object_Manager.h"

#include "advanced_resource.h"
#include "UIOP_Factory.h"
#include "SHMIOP_Factory.h"

#include "tao/Object_Loader.h"

ACE_RCSID(Strategies, TAO_Strategies_Internal, "$Id$")

int TAO_Strategies_Internal::service_open_count_ = 0;
#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
const char *TAO_Strategies_Internal::resource_factory_args_ = TAO_ADVANCED_RESOURCE_FACTORY_ARGS;
#else
const char *TAO_Strategies_Internal::resource_factory_args_ = 0;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */

void
TAO_Strategies_Internal::default_svc_conf_entries (
                           const char *resource_factory_args)
{
  TAO_Strategies_Internal::resource_factory_args_ = resource_factory_args;
}

int
TAO_Strategies_Internal::open_services (int &argc,
                                        char **argv,
                                        int ignore_default_svc_conf_file,
                                        int skip_service_config_open)
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX, guard, *ACE_Static_Object_Lock::instance (), -1));
#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
  ignore_default_svc_conf_file = 1;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */

  if (TAO_Strategies_Internal::service_open_count_++ == 0)
    {
      ACE_Service_Config::static_svcs ()->
        insert (&ace_svc_desc_TAO_Advanced_Resource_Factory);

      // Configure the UIOP and SHMIOP factories. You do *NOT*
      // need modify this code to add your own protocol, instead
      // simply add the following to your svc.conf file:
      //
      // dynamic PN_Factory Service_Object * LIB:_make_PN_Protocol_Factory() ""
      // static Resource_Factory "-ORBProtocolFactory PN_Factory"
      //
      // where PN is the name of your protocol and LIB is the base
      // name of the shared library that implements the protocol.
#if TAO_HAS_UIOP == 1
      ACE_Service_Config::static_svcs ()->
        insert (&ace_svc_desc_TAO_UIOP_Protocol_Factory);
#endif /* TAO_HAS_UIOP == 1 */
#if TAO_HAS_SHMIOP == 1
      ACE_Service_Config::static_svcs ()->
        insert (&ace_svc_desc_TAO_SHMIOP_Protocol_Factory);
#endif /* TAO_HAS_UIOP == 1 */
      int result = 0;

      if (skip_service_config_open == 0)
        result = ACE_Service_Config::open (argc, argv,
                                           ACE_DEFAULT_LOGGER_KEY,
                                           0, // Don't ignore static services.
                                           ignore_default_svc_conf_file);

      if (TAO_Strategies_Internal::resource_factory_args_ != 0)
        ACE_Service_Config::process_directive (TAO_Strategies_Internal::resource_factory_args_);
      return result;
    }
  else
    return 0;
}

TAO_Strategies_Internal::TAO_Strategies_Internal (void)
{
}

int
TAO_Strategies_Internal::close_services (void)
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX, guard,
                            *ACE_Static_Object_Lock::instance (), -1));
  --service_open_count_;
  return 0;
}
