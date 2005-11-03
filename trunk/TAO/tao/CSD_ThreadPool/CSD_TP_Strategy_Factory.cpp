// $Id$

#include "CSD_TP_Strategy_Factory.h"
#include "CSD_TP_Strategy.h"
#include "CSD_ThreadPool.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"
#include "tao/debug.h"
#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_strings.h"

ACE_RCSID (CSD_ThreadPool,
           TP_Strategy_Factory,
           "$Id$")

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
    ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance ("TAO_CSD_Strategy_Repository");

  if (repo != 0)
    repo->init(0,0);

  ACE_CString poa_name;
  unsigned long num_threads = 1;

  // Parse any service configurator parameters.
  for (int curarg = 0; curarg < argc; curarg++)
    if (ACE_OS::strcasecmp (argv[curarg],
                            ACE_TEXT("-CSDtp")) == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            // Parse the parameter
            ACE_CString arg ((const char *)argv[curarg]);
            ssize_t pos = arg.find (':');
            // If the number of threads is not specified then use the default value
            // which is 1.
            if (pos == ACE_CString::npos)
              {
                poa_name = arg;
              }
            else
              {
                poa_name = arg.substr (0, pos);
                ACE_CString num_thread_str = arg.substr (pos + 1, arg.length () - pos);
                num_threads = ACE_OS::strtoul (num_thread_str.c_str (), 0, 10);
              }

            // Create the ThreadPool strategy for each named poa.
            TP_Strategy* strategy = 0;
            ACE_NEW_RETURN (strategy, TP_Strategy (num_threads), -1);
            CSD_Framework::Strategy_var objref = strategy;

            TAO_CSD_Strategy_Repository *repo =
              ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance
              ("TAO_CSD_Strategy_Repository");

            if (repo == 0)
              {
                 TAO_CSD_ThreadPool::init ();
                 repo = ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance (
                            "TAO_CSD_Strategy_Repository"
                          );
              }


            repo->add_strategy (poa_name, strategy);
          }
      }
    else
      {
        if (TAO_debug_level > 0)
          {
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT("CSD_ORB_Loader: Unknown option ")
                        ACE_TEXT("<%s>.\n"),
                        argv[curarg]));
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
