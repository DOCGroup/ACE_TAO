// $Id$

#include "tao/Dynamic_TP/Dynamic_TP_POA_Loader.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0



#include "tao/debug.h"
//Review these
#include "tao/CSD_ThreadPool/CSD_TP_Strategy_Factory.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"
#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"


//Review these

#include "tao/ORB_Constants.h"
//#include "tao/ORBInitializer_Registry.h"
#include "tao/SystemException.h"
#include "ace/OS_NS_strings.h"
#include "ace/Arg_Shifter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Dynamic_TP_POA_Loader::TAO_Dynamic_TP_POA_Loader (void)
  : initialized_ (false)
{
}

TAO_Dynamic_TP_POA_Loader::~TAO_Dynamic_TP_POA_Loader (void)
{
}

int
TAO_Dynamic_TP_POA_Loader::init (int argc, ACE_TCHAR* argv[])
{
  ACE_TRACE ("TAO_Dynamic_TP_POA_Loader::init");

  // Only allow initialization once.
  if (this->initialized_)
    return 0;

  this->initialized_ = true;

  ACE_Service_Gestalt *gestalt = ACE_Service_Config::current ();

  ACE_Service_Object * const dynamic_tp_poa_loader =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (
      gestalt,
      "Dynamic_TP_POA_Loader",
      true);

  if (dynamic_tp_poa_loader != 0 && dynamic_tp_poa_loader != this)
    {
      return dynamic_tp_poa_loader->init (argc, argv);
    }

  // Set defaults.
 /* int priority_mapping_type =
    TAO_RT_POAInitializer::TAO_PRIORITY_MAPPING_DIRECT;
  int network_priority_mapping_type =
    TAO_RT_POAInitializer::TAO_NETWORK_PRIORITY_MAPPING_LINEAR;
  int ace_sched_policy = ACE_SCHED_OTHER;
  long sched_policy = THR_SCHED_DEFAULT;
  long scope_policy = THR_SCOPE_PROCESS;
  int curarg = 0;
  ACE_Time_Value dynamic_thread_time;
  TAO_RT_POAInitializer::TAO_DynamicTP_DT_LifeSpan lifespan = TAO_RT_POAInitializer::TAO_DynamicTP_DT_INFINITIVE;

  ACE_Arg_Shifter arg_shifter (argc, argv);
  
  // Parse any service configurator parameters.
  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *current_arg = 0;
      if (0 != (current_arg = arg_shifter.get_the_parameter
                  (ACE_TEXT("-ORBPriorityMapping"))))
        {
          const ACE_TCHAR *name = current_arg;
          if (ACE_OS::strcasecmp (name,
                                  ACE_TEXT("continuous")) == 0)
            priority_mapping_type =
              TAO_RT_POAInitializer::TAO_PRIORITY_MAPPING_CONTINUOUS;
          else if (ACE_OS::strcasecmp (name,
                                       ACE_TEXT("linear")) == 0)
            priority_mapping_type =
              TAO_RT_POAInitializer::TAO_PRIORITY_MAPPING_LINEAR;
          else if (ACE_OS::strcasecmp (name,
                                       ACE_TEXT("direct")) == 0)
            priority_mapping_type =
              TAO_RT_POAInitializer::TAO_PRIORITY_MAPPING_DIRECT;
          else
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("Dynamic_TP_POA_Loader - unknown argument")
                        ACE_TEXT(" <%s> for -ORBPriorityMapping\n"),
                        name));
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                                 (ACE_TEXT("-ORBSchedPolicy"))))
        {
          const ACE_TCHAR *name = current_arg;
          if (ACE_OS::strcasecmp (name,
                                  ACE_TEXT("SCHED_OTHER")) == 0)
            {
              ace_sched_policy = ACE_SCHED_OTHER;
              sched_policy = THR_SCHED_DEFAULT;
            }
          else if (ACE_OS::strcasecmp (name,
                                       ACE_TEXT("SCHED_FIFO")) == 0)
            {
              ace_sched_policy = ACE_SCHED_FIFO;
              sched_policy = THR_SCHED_FIFO;
            }
          else if (ACE_OS::strcasecmp (name,
                                       ACE_TEXT("SCHED_RR")) == 0)
            {
              ace_sched_policy = ACE_SCHED_RR;
              sched_policy = THR_SCHED_RR;
            }
          else
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("Dynamic_TP_POA_Loader - unknown argument")
                        ACE_TEXT(" <%s> for -ORBSchedPolicy\n"),
                        name));
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                                 (ACE_TEXT("-ORBScopePolicy"))))
        {
          const ACE_TCHAR *name = current_arg;
          if (ACE_OS::strcasecmp (name,
                                  ACE_TEXT("SYSTEM")) == 0)
            scope_policy = THR_SCOPE_SYSTEM;
          else if (ACE_OS::strcasecmp (name,
                                       ACE_TEXT("PROCESS")) == 0)
            scope_policy = THR_SCOPE_PROCESS;
          else
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("Dynamic_TP_POA_Loader - unknown argument")
                        ACE_TEXT(" <%s> for -ORBScopePolicy\n"),
                        name));
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                                 (ACE_TEXT("-RTORBNetworkPriorityMapping"))))
        {
          const ACE_TCHAR *name = current_arg;
          if (ACE_OS::strcasecmp (name,
                                  ACE_TEXT("linear")) == 0)
              network_priority_mapping_type =
                TAO_RT_POAInitializer::TAO_NETWORK_PRIORITY_MAPPING_LINEAR;
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                                   (ACE_TEXT("-RTORBDynamicThreadIdleTimeout"))))
        {
          const ACE_TCHAR *name = current_arg;
          int timeout = ACE_OS::atoi (name);
          dynamic_thread_time = ACE_Time_Value (0, timeout);
          lifespan = TAO_RT_POAInitializer::TAO_DynamicTP_DT_IDLE;
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                                   (ACE_TEXT("-RTORBDynamicThreadRunTime"))))
        {
          const ACE_TCHAR *name = current_arg;
          int timeout = ACE_OS::atoi (name);
          dynamic_thread_time = ACE_Time_Value (0, timeout);
          lifespan = TAO_RT_POAInitializer::TAO_DynamicTP_DT_FIXED;
          arg_shifter.consume_arg ();
        }
    else
      {
        arg_shifter.ignore_arg ();
        if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT("Dynamic_TP_POA_Loader: Unknown option ")
                      ACE_TEXT("<%s>.\n"),
                      argv[curarg]));
        }
      }
    }

  // Register the ORB initializer.
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      /// Register the DynamicTP ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_RT_POAInitializer (priority_mapping_type,
                                               network_priority_mapping_type,
                                               ace_sched_policy,
                                               sched_policy,
                                               scope_policy,
                                               lifespan,
                                               dynamic_thread_time),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      PortableInterceptor::ORBInitializer_var orb_initializer;
      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught while "
        "initializing the RTORB");
      return 1;
    }

  return 0; */

   TAO_CSD_Strategy_Repository *repo =
    ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance ("TAO_CSD_Strategy_Repository");

  if (repo != 0)
    repo->init(0,0);

  // Parse any service configurator parameters.
  for (int curarg = 0; curarg < argc; curarg++)
    if (ACE_OS::strcasecmp (argv[curarg],
                            ACE_TEXT("-CSDtp")) == 0)
      {
        ACE_CString poa_name;
        unsigned long num_threads = 1;
        bool serialize_servants = true;

        curarg++;
        if (curarg < argc)
          {
            // Parse the parameter
            ACE_CString arg ((const char *)argv[curarg]);
            ACE_CString::size_type pos = arg.find (':');

            if (pos == ACE_CString::npos)
              {
                poa_name = arg;
              }
            else
              {
                poa_name = arg.substr (0, pos);

                ACE_CString arg_remainder =
                                   arg.substr (pos + 1, arg.length () - pos);

                ACE_CString num_thread_str;

                pos = arg_remainder.find (':');

                if (pos == ACE_CString::npos)
                  {
                    num_thread_str = arg_remainder;
                  }
                else
                  {
                    num_thread_str = arg_remainder.substr (0, pos);

                    ACE_CString off_str =
                                     arg_remainder.substr (pos + 1, arg.length () - pos);

                    // Case-insensitive string comparison.
                    if (ACE_OS::strcasecmp (off_str.c_str(), "OFF") == 0)
                      {
                        serialize_servants = false;
                      }
                  }

                num_threads = ACE_OS::strtoul (num_thread_str.c_str (), 0, 10);

                if (num_threads == 0)
                  {
                    // Minimum of 1 thread required.
                    num_threads = 1;
                  }
              }

            // Create the ThreadPool strategy for each named poa.
			
            TAO::CSD::TP_Strategy* strategy = 0;   // Need a new dynamic thread pool strategy here
            ACE_NEW_RETURN (strategy,
                            TAO::CSD::TP_Strategy (num_threads, serialize_servants),
                            -1);


            //TAO_CSD_Strategy_Repository *repo =
            //  ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance
            //  ("TAO_CSD_Strategy_Repository");

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
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT("CSD_ORB_Loader: Unknown option ")
                        ACE_TEXT("<%s>.\n"),
                        argv[curarg]));
          }
      }


  return 0;
}


/////////////////////////////////////////////////////////////////////

ACE_FACTORY_DEFINE (TAO_Dynamic_TP, TAO_Dynamic_TP_POA_Loader)
ACE_STATIC_SVC_DEFINE (TAO_Dynamic_TP_POA_Loader,
                       ACE_TEXT ("Dynamic_TP_POA_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Dynamic_TP_POA_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)



TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
