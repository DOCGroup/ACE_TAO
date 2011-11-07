// $Id$

#include "tao/RTCORBA/RT_ORB_Loader.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/RT_ORBInitializer.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/SystemException.h"
#include "ace/OS_NS_strings.h"
#include "ace/Arg_Shifter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RT_ORB_Loader::TAO_RT_ORB_Loader (void)
  : initialized_ (false)
{
}

TAO_RT_ORB_Loader::~TAO_RT_ORB_Loader (void)
{
}

int
TAO_RT_ORB_Loader::init (int argc, ACE_TCHAR* argv[])
{
  ACE_TRACE ("TAO_RT_ORB_Loader::init");

  // Only allow initialization once.
  if (this->initialized_)
    return 0;

  this->initialized_ = true;

  ACE_Service_Gestalt *gestalt = ACE_Service_Config::current ();

  ACE_Service_Object * const rt_loader =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (
      gestalt,
      "RT_ORB_Loader",
      true);

  if (rt_loader != 0 && rt_loader != this)
    {
      return rt_loader->init (argc, argv);
    }

  // Set defaults.
  int priority_mapping_type =
    TAO_RT_ORBInitializer::TAO_PRIORITY_MAPPING_DIRECT;
  int network_priority_mapping_type =
    TAO_RT_ORBInitializer::TAO_NETWORK_PRIORITY_MAPPING_LINEAR;
  int ace_sched_policy = ACE_SCHED_OTHER;
  long sched_policy = THR_SCHED_DEFAULT;
  long scope_policy = THR_SCOPE_PROCESS;
  int curarg = 0;
  ACE_Time_Value dynamic_thread_time;
  TAO_RT_ORBInitializer::TAO_RTCORBA_DT_LifeSpan lifespan = TAO_RT_ORBInitializer::TAO_RTCORBA_DT_INFINITIVE;

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
              TAO_RT_ORBInitializer::TAO_PRIORITY_MAPPING_CONTINUOUS;
          else if (ACE_OS::strcasecmp (name,
                                       ACE_TEXT("linear")) == 0)
            priority_mapping_type =
              TAO_RT_ORBInitializer::TAO_PRIORITY_MAPPING_LINEAR;
          else if (ACE_OS::strcasecmp (name,
                                       ACE_TEXT("direct")) == 0)
            priority_mapping_type =
              TAO_RT_ORBInitializer::TAO_PRIORITY_MAPPING_DIRECT;
          else
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("RT_ORB_Loader - unknown argument")
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
                        ACE_TEXT("RT_ORB_Loader - unknown argument")
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
                        ACE_TEXT("RT_ORB_Loader - unknown argument")
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
                TAO_RT_ORBInitializer::TAO_NETWORK_PRIORITY_MAPPING_LINEAR;
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                                   (ACE_TEXT("-RTORBDynamicThreadIdleTimeout"))))
        {
          const ACE_TCHAR *name = current_arg;
          int timeout = ACE_OS::atoi (name);
          dynamic_thread_time = ACE_Time_Value (0, timeout);
          lifespan = TAO_RT_ORBInitializer::TAO_RTCORBA_DT_IDLE;
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                                   (ACE_TEXT("-RTORBDynamicThreadRunTime"))))
        {
          const ACE_TCHAR *name = current_arg;
          int timeout = ACE_OS::atoi (name);
          dynamic_thread_time = ACE_Time_Value (0, timeout);
          lifespan = TAO_RT_ORBInitializer::TAO_RTCORBA_DT_FIXED;
          arg_shifter.consume_arg ();
        }
    else
      {
        arg_shifter.ignore_arg ();
        if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT("RT_ORB_Loader: Unknown option ")
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

      /// Register the RTCORBA ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_RT_ORBInitializer (priority_mapping_type,
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

  return 0;
}


/////////////////////////////////////////////////////////////////////

ACE_FACTORY_DEFINE (TAO_RTCORBA, TAO_RT_ORB_Loader)
ACE_STATIC_SVC_DEFINE (TAO_RT_ORB_Loader,
                       ACE_TEXT ("RT_ORB_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_ORB_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
