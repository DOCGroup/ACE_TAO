// $Id$

#include "orbsvcs/Event/EC_Gateway_IIOP_Factory.h"
#include "orbsvcs/Event/ECG_Defaults.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Time_Utilities.h"

#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "ECG_ConsumerEC_Control.h"
#include "ECG_Reactive_ConsumerEC_Control.h"
#include "EC_Gateway_IIOP.h"

ACE_RCSID(Event, EC_Gateway_IIOP_Factory, "$Id$")

int
TAO_EC_Gateway_IIOP_Factory::init_svcs (void)
{
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_EC_Gateway_IIOP_Factory);
}

TAO_EC_Gateway_IIOP_Factory::TAO_EC_Gateway_IIOP_Factory (void)
  :  orbid_ (TAO_ECG_DEFAULT_IIOP_ORB_ID),
     consumer_ec_control_ (TAO_ECG_DEFAULT_IIOP_CONSUMEREC_CONTROL),
     consumer_ec_control_period_ (TAO_ECG_DEFAULT_IIOP_CONSUMEREC_CONTROL_PERIOD),
     consumer_ec_control_timeout_ (TAO_ECG_DEFAULT_IIOP_CONSUMEREC_CONTROL_TIMEOUT)
{
}

TAO_EC_Gateway_IIOP_Factory::~TAO_EC_Gateway_IIOP_Factory (void)
{
}

int
TAO_EC_Gateway_IIOP_Factory::fini (void)
{
  return 0;
}

int
TAO_EC_Gateway_IIOP_Factory::init (int argc, char* argv[])
{
  int result = 0;

  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const char *arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, ACE_LIB_TEXT("-ECGIIOPConsumerECControl")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("null")) == 0)
                this->consumer_ec_control_ = 0;
              else if (ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("reactive")) == 0)
                this->consumer_ec_control_ = 1;
              else
                this->unsupported_option_value ("-ECGIIOPConsumerECControl", opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_LIB_TEXT("-ECGIIOPConsumerECControlPeriod")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->consumer_ec_control_period_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_LIB_TEXT("-ECGIIOPConsumerECControlTimeout")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              unsigned long timeout = ACE_OS::strtoul(opt, 0, 10);
              this->consumer_ec_control_timeout_.usec(timeout);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_LIB_TEXT("-ECGIIOPUseORBId")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              // Save argument for later use
              this->orbid_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else
        {
          arg_shifter.ignore_arg ();
          ACE_DEBUG ((LM_WARNING,
                             "Ignoring <%s> option "
                             "during initialization.",
                      arg));
          result = -1;
        }
    }

    return result;
}

void
TAO_EC_Gateway_IIOP_Factory::unsupported_option_value (const char * option_name,
                                               const char * option_value)
{
  ACE_ERROR ((LM_ERROR,
              "EC_Gateway_IIOP_Factory -"
              "Unsupported <%s> option value: <%s>. "
              "Ignoring this option - using defaults instead.",
              option_name,
              option_value));
}

TAO_ECG_ConsumerEC_Control*
TAO_EC_Gateway_IIOP_Factory::create_consumerec_control (TAO_EC_Gateway_IIOP* gateway)
{
  if (this->consumer_ec_control_ == 0)
    return new TAO_ECG_ConsumerEC_Control ();
  else if (this->consumer_ec_control_ == 1)
    {
      int argc = 0;
      char **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_.c_str ());
      ACE_Time_Value rate (0, this->consumer_ec_control_period_);
      return new TAO_ECG_Reactive_ConsumerEC_Control (rate, consumer_ec_control_timeout_, gateway, orb.in ());
    }
  return 0;
}

void
TAO_EC_Gateway_IIOP_Factory::destroy_consumerec_control (
  TAO_ECG_ConsumerEC_Control* x)
{
   delete x;
}

ACE_STATIC_SVC_DEFINE (TAO_EC_Gateway_IIOP_Factory,
                       ACE_TEXT ("EC_Gateway_IIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_EC_Gateway_IIOP_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTEvent, TAO_EC_Gateway_IIOP_Factory)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_EC_Gateway_IIOP_Factory>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_EC_Gateway_IIOP_Factory>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

