// $Id$

#include "Config_Factory.h"
#include "ECConfig.h"

#include "ace/Arg_Shifter.h"
#include "ace/OS_NS_strings.h"
#include "orbsvcs/Sched/Reconfig_Scheduler.h"

#if ! defined (__ACE_INLINE__)
#include "Config_Factory.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Configurator, Default_Config_Factory, "$Id$")

namespace ConfigFactory {

typedef TAO_Reconfig_Scheduler<TAO_MUF_FAIR_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX> MUF_SCHED_TYPE;
typedef TAO_Reconfig_Scheduler<TAO_RMS_FAIR_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX> RMS_SCHED_TYPE;
typedef TAO_Reconfig_Scheduler<TAO_EDF_FAIR_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX> EDF_SCHED_TYPE;

Config_Factory::~Config_Factory (void)
{
}

Default_Config_Factory::~Default_Config_Factory (void)
{
}

int
Default_Config_Factory::init_svcs (void)
{
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_Default_Config_Factory);
}

int
Default_Config_Factory::init (int argc, ACE_TCHAR* argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, ACE_LIB_TEXT("-ECConfig")) == 0)
        {
          arg_shifter.consume_arg ();
          test_config_ = 0;

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if ((ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("null")) == 0)
                  || (ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("rms")) == 0))
                {
                  this->sched_type_ = RMS; //default
                  ACE_DEBUG ((LM_DEBUG,
                              "Default_Config_Factory: Scheduling type is RMS\n"));
                }
              else if (ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("muf")) == 0)
                {
                  this->sched_type_ = MUF;
                  ACE_DEBUG ((LM_DEBUG,
                              "Default_Config_Factory: Scheduling type is MUF\n"));
                }
              else if (ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("edf")) == 0)
                {
                  this->sched_type_ = EDF;
                  ACE_DEBUG ((LM_DEBUG,
                              "Default_Config_Factory: Scheduling type is EDF\n"));
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "Default_Config_Factory - "
                              "unsupported scheduling type <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }
      else
        {
          arg_shifter.consume_arg ();
          ACE_DEBUG ((LM_DEBUG,
                      "Default_Config_Factory - "
                      "ignoring option <%s>\n",
                      arg));
        }
    }
  return 0;
}

int
Default_Config_Factory::fini (void)
{
  return 0;
}

TestConfig::Test_Config*
Default_Config_Factory::create_testconfig ()
{
  if (this->test_config_ == 0)
    {
      switch (this->sched_type_) {
        case EDF:
          ACE_DEBUG ((LM_DEBUG,
                      "Default_Config_Factory: Returning EDF Test_Config\n"));
          return new TestConfig::ECConfig<EDF_SCHED_TYPE>();
          break;
        case MUF:
          ACE_DEBUG ((LM_DEBUG,
                      "Default_Config_Factory: Returning MUF Test_Config\n"));
          return new TestConfig::ECConfig<MUF_SCHED_TYPE>();
          break;
        case RMS:
          ACE_DEBUG ((LM_DEBUG,
                      "Default_Config_Factory: Returning RMS Test_Config\n"));
          return new TestConfig::ECConfig<RMS_SCHED_TYPE>();
          break;
        default:
          ACE_ERROR ((LM_ERROR,
                      "Default_Config_Factory - "
                      "unknown scheduling type <%d>\n",
                      this->sched_type_));
          return 0;
      };
    } //else...
  return 0;
}

void
Default_Config_Factory::destroy_testconfig (TestConfig::Test_Config *x)
{
  delete x;
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (Default_Config_Factory,
                       ACE_TEXT ("Config_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Default_Config_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (Test_Driver, Default_Config_Factory)

// ****************************************************************

} //namespace ConfigFactory

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
