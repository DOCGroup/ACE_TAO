// $Id$

#include "RT_ORB_Loader.h"
#include "RT_ORB.h"
#include "RT_Current.h"
#include "RT_ORBInitializer.h"
#include "Linear_Priority_Mapping.h"
#include "Direct_Priority_Mapping.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORBInitializer_Registry.h"

ACE_RCSID (TAO, RT_ORB_Loader, "$Id$")

int
TAO_RT_ORB_Loader::Initializer (void)
{
  ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_RT_ORB_Loader);

  ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_RT_Current_Loader);

  return 0;
}

TAO_RT_ORB_Loader::TAO_RT_ORB_Loader (void)
{
}

TAO_RT_ORB_Loader::~TAO_RT_ORB_Loader (void)
{
}

int
TAO_RT_ORB_Loader::init (int argc,
                         char* argv[])
{
  ACE_TRACE ("TAO_Advanced_Resource_Factory::init");

  int priority_mapping_type = TAO_PRIORITY_MAPPING_DIRECT;
  int sched_policy = ACE_SCHED_OTHER;
  int curarg = 0;

  // Parse any service configurator parameters.
  for (curarg = 0; curarg < argc; curarg++)
    if (ACE_OS::strcasecmp (argv[curarg],
                            "-ORBPriorityMapping") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    "linear") == 0)
              priority_mapping_type = TAO_PRIORITY_MAPPING_LINEAR;
            else if (ACE_OS::strcasecmp (name,
                                         "direct") == 0)
              priority_mapping_type = TAO_PRIORITY_MAPPING_DIRECT;
            else
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("RT_ORB_Loader - unknown argument")
                          ACE_TEXT (" <%s> for -ORBPriorityMapping\n"), name));
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBSchedPolicy") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    "SCHED_OTHER") == 0)
              sched_policy = ACE_SCHED_OTHER;
            else if (ACE_OS::strcasecmp (name,
                                         "SCHED_FIFO") == 0)
              sched_policy = ACE_SCHED_FIFO;
            else if (ACE_OS::strcasecmp (name,
                                         "SCHED_RR") == 0)
              sched_policy = ACE_SCHED_RR;
            else
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("RT_ORB_Loader - unknown argument")
                          ACE_TEXT (" <%s> for -ORBSchedPolicy\n"), name));
          }
      }
    else
      {
        if (TAO_debug_level > 0)
          {
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("RT_ORB_Loader: Unknown option ")
                        ACE_TEXT ("<%s>.\n"),
                        argv[curarg]));
          }
      }

  // Create the initial priority mapping instance.
  TAO_Priority_Mapping *pm;
  switch (priority_mapping_type)
    {
    case TAO_PRIORITY_MAPPING_LINEAR:
      ACE_NEW_RETURN (pm,
                      TAO_Linear_Priority_Mapping (sched_policy),
                      0);
      break;
    default:
    case TAO_PRIORITY_MAPPING_DIRECT:
      ACE_NEW_RETURN (pm,
                      TAO_Direct_Priority_Mapping (sched_policy),
                      0);
      break;
    }


  // Register the ORB initializer.
  ACE_TRY_NEW_ENV
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();
      PortableInterceptor::ORBInitializer_var orb_initializer;

      /// Register the RTCORBA ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_RT_ORBInitializer (pm),
                        CORBA::NO_MEMORY (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;
      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in (),
                                                     ACE_TRY_ENV);
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

CORBA::Object_ptr
TAO_RT_ORB_Loader::create_object (CORBA::ORB_ptr orb,
                                  int,
                                  char *[],
                                  CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  /// Return RT_ORB
  CORBA::Object_ptr rt_orb;

  ACE_NEW_THROW_EX (rt_orb,
                    TAO_RT_ORB (orb),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return rt_orb;
}

/////////////////////////////////////////////////////////////////////

CORBA::Object_ptr
TAO_RT_Current_Loader::create_object (CORBA::ORB_ptr orb,
                                      int,
                                      char *[],
                                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  /// Return RT_Current
  CORBA::Object_ptr current;

  ACE_NEW_THROW_EX (current,
                    TAO_RT_Current (orb->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return current;
}

ACE_FACTORY_DEFINE (TAO, TAO_RT_ORB_Loader)
ACE_STATIC_SVC_DEFINE (TAO_RT_ORB_Loader,
                       ACE_TEXT ("RT_ORB_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_ORB_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_RT_Current_Loader)
ACE_STATIC_SVC_DEFINE (TAO_RT_Current_Loader,
                       ACE_TEXT ("RT_Current_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_Current_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
