// $Id$

#include "RT_ORB_Loader.h"
#include "RT_ORBInitializer.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORBInitializer_Registry.h"

ACE_RCSID (TAO, RT_ORB_Loader, "$Id$")

TAO_RT_ORB_Loader::TAO_RT_ORB_Loader (void)
{
}

TAO_RT_ORB_Loader::~TAO_RT_ORB_Loader (void)
{
}

int
TAO_RT_ORB_Loader::init (int argc,
                         ACE_TCHAR* argv[])
{
  ACE_TRACE ("TAO_RT_ORB_Loader::init");

  static int initialized = 0;

  // Only allow initialization once.
  if (initialized)
    return 0;

  initialized = 1;

  // Set defaults.
  int priority_mapping_type = TAO_RT_ORBInitializer::TAO_PRIORITY_MAPPING_DIRECT;
  long sched_policy = THR_SCHED_DEFAULT;
  long scope_policy = THR_SCOPE_PROCESS;
  int curarg = 0;

  // Parse any service configurator parameters.
  for (curarg = 0; curarg < argc; curarg++)
    if (ACE_OS::strcasecmp (argv[curarg],
                            ACE_LIB_TEXT("-ORBPriorityMapping")) == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_LIB_TEXT("continuous")) == 0)
              priority_mapping_type = TAO_RT_ORBInitializer::TAO_PRIORITY_MAPPING_CONTINUOUS;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("linear")) == 0)
              priority_mapping_type = TAO_RT_ORBInitializer::TAO_PRIORITY_MAPPING_LINEAR;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("direct")) == 0)
              priority_mapping_type = TAO_RT_ORBInitializer::TAO_PRIORITY_MAPPING_DIRECT;
            else
              ACE_DEBUG ((LM_DEBUG,
                          ACE_LIB_TEXT("RT_ORB_Loader - unknown argument")
                          ACE_LIB_TEXT(" <%s> for -ORBPriorityMapping\n"),
                          name));
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBSchedPolicy")) == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_LIB_TEXT("SCHED_OTHER")) == 0)
              sched_policy = THR_SCHED_DEFAULT;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("SCHED_FIFO")) == 0)
              sched_policy = THR_SCHED_FIFO;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("SCHED_RR")) == 0)
              sched_policy = THR_SCHED_RR;
            else
              ACE_DEBUG ((LM_DEBUG,
                          ACE_LIB_TEXT("RT_ORB_Loader - unknown argument")
                          ACE_LIB_TEXT(" <%s> for -ORBSchedPolicy\n"),
                          name));
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBScopePolicy")) == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_LIB_TEXT("SYSTEM")) == 0)
              scope_policy = THR_SCOPE_SYSTEM;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("PROCESS")) == 0)
              scope_policy = THR_SCOPE_PROCESS;
            else
              ACE_DEBUG ((LM_DEBUG,
                          ACE_LIB_TEXT("RT_ORB_Loader - unknown argument")
                          ACE_LIB_TEXT(" <%s> for -ORBScopePolicy\n"),
                          name));
          }
      }
    else
      {
        if (TAO_debug_level > 0)
          {
            ACE_DEBUG ((LM_ERROR,
                        ACE_LIB_TEXT("RT_ORB_Loader: Unknown option ")
                        ACE_LIB_TEXT("<%s>.\n"),
                        argv[curarg]));
          }
      }

  // Register the ORB initializer.
  ACE_TRY_NEW_ENV
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();
      PortableInterceptor::ORBInitializer_var orb_initializer;

      /// Register the RTCORBA ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_RT_ORBInitializer (priority_mapping_type,
                                               sched_policy,
                                               scope_policy),
                        CORBA::NO_MEMORY (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;
      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught while initializing the RTORB:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

/////////////////////////////////////////////////////////////////////

ACE_FACTORY_DEFINE (TAO, TAO_RT_ORB_Loader)
ACE_STATIC_SVC_DEFINE (TAO_RT_ORB_Loader,
                       ACE_TEXT ("RT_ORB_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_ORB_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
