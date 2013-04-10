// $Id$

#include "tao/CSD_ThreadPool/CSD_TP_Strategy_Factory.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"
#include "tao/debug.h"
#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_strings.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::CSD::TP_Strategy_Factory::TP_Strategy_Factory()
{
}


TAO::CSD::TP_Strategy_Factory::~TP_Strategy_Factory()
{
}


int
TAO::CSD::TP_Strategy_Factory::init (int argc,
                                     ACE_TCHAR* argv[])
{
  ACE_TRACE ("TAO::CSD::TP_Strategy_Factory::init");

  static int initialized = 0;

  // Only allow initialization once.
  if (initialized)
    return 0;

  initialized = 1;
  TAO_CSD_Strategy_Repository *repo =
    ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance
      (ACE_TEXT ("TAO_CSD_Strategy_Repository"));

  if (repo == 0)
    {
      TAO_CSD_ThreadPool::init ();
      repo = ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance
        (ACE_TEXT ("TAO_CSD_Strategy_Repository"));
    }

  if (repo == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     ACE_TEXT ("TAO (%P|%t) - TP_Strategy_Factory - ")
                     ACE_TEXT ("cannot initialize strategy repo\n")));
        }
      return -1;
    }

  repo->init(0,0);

  // Parse any service configurator parameters.
  for (int curarg = 0; curarg < argc; curarg++)
    {
      if (ACE_OS::strcasecmp (argv[curarg],
                              ACE_TEXT("-CSDtp")) == 0)
        {
          ACE_CString poa_name;
          unsigned long num_threads = 1;
          bool serialize_servants = true;

          curarg++;
          if (curarg >= argc)
            {
              return -1;
            }

          // Parse the parameter
          ACE_TCHAR *sep = ACE_OS::strchr (argv[curarg],':');
          if (sep == 0)
            {
              poa_name = ACE_TEXT_ALWAYS_CHAR (argv[curarg]);
            }
          else
            {
              *sep = 0;
              poa_name = ACE_TEXT_ALWAYS_CHAR (argv[curarg]);
              num_threads = ACE_OS::strtol (sep + 1, &sep, 10);
              if (*sep != 0 && *sep != ':')
                {
                  return -1;
                }
              if (*sep == ':')
                {
                  if (ACE_OS::strcasecmp (
                    sep + 1, ACE_TEXT_CHAR_TO_TCHAR ("OFF")) == 0)
                    {
                      serialize_servants = false;
                    }
                }
            }

          // Create the ThreadPool strategy for each named poa.
          TP_Strategy* strategy = 0;
          ACE_NEW_RETURN (strategy,
                          TP_Strategy (num_threads, serialize_servants),
                          -1);
          CSD_Framework::Strategy_var objref = strategy;
          repo->add_strategy (poa_name, strategy);

        }
      else
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT("CSD_ORB_Loader: Unknown option ")
                          ACE_TEXT("<%s>.\n"),
                          argv[curarg]));
            }
          return -1;
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_NAMESPACE_DEFINE(TAO_CSD_TP,
                  TAO_CSD_TP_Strategy_Factory,
                  TAO::CSD::TP_Strategy_Factory)

ACE_STATIC_SVC_DEFINE(TAO_CSD_TP_Strategy_Factory,
                      ACE_TEXT("TAO_CSD_TP_Strategy_Factory"),
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME(TAO_CSD_TP_Strategy_Factory),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                      0)
