// $Id$

#include "orbsvcs/Event/EC_Gateway_IIOP_Factory.h"
#include "orbsvcs/Event/ECG_Defaults.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Time_Utilities.h"

#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"

#include "orbsvcs/Event/ECG_ConsumerEC_Control.h"
#include "orbsvcs/Event/ECG_Reactive_ConsumerEC_Control.h"
#include "orbsvcs/Event/ECG_Reconnect_ConsumerEC_Control.h"
#include "orbsvcs/Event/EC_Gateway_IIOP.h"
#include "ace/OS_NS_strings.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Gateway_IIOP_Factory.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
     consumer_ec_control_timeout_ (TAO_ECG_DEFAULT_IIOP_CONSUMEREC_CONTROL_TIMEOUT),
     use_ttl_ (TAO_ECG_DEFAULT_IIOP_USE_TTL),
     use_consumer_proxy_map_ (TAO_ECG_DEFAULT_IIOP_USE_CONSUMER_PROXY_MAP)
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
TAO_EC_Gateway_IIOP_Factory::init (int argc, ACE_TCHAR* argv[])
{
  int result = 0;

  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECGIIOPConsumerECControl")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                this->consumer_ec_control_ = 0;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("reactive")) == 0)
                this->consumer_ec_control_ = 1;
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("reconnect")) == 0)
                this->consumer_ec_control_ = 2;
              else
                this->unsupported_option_value (ACE_TEXT("-ECGIIOPConsumerECControl"), opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECGIIOPConsumerECControlPeriod")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->consumer_ec_control_period_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECGIIOPConsumerECControlTimeout")) == 0)
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

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECGIIOPUseORBId")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              // Save argument for later use
              this->orbid_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECGIIOPUseTTL")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->use_ttl_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECGIIOPUseConsumerProxyMap")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->use_consumer_proxy_map_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }

      else
        {
          arg_shifter.ignore_arg ();
          ACE_DEBUG ((LM_WARNING,
                             "Ignoring <%s> option "
                             "during initialization.\n",
                      arg));
          result = -1;
        }
    }

    return result;
}

void
TAO_EC_Gateway_IIOP_Factory::unsupported_option_value (const ACE_TCHAR * option_name,
                                                       const ACE_TCHAR * option_value)
{
  ACE_ERROR ((LM_ERROR,
              "EC_Gateway_IIOP_Factory -"
              "Unsupported <%s> option value: <%s>. "
              "Ignoring this option - using defaults instead.\n",
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
      ACE_TCHAR **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_.c_str ());
      ACE_Time_Value rate (0, this->consumer_ec_control_period_);
      return new TAO_ECG_Reactive_ConsumerEC_Control (rate, consumer_ec_control_timeout_, gateway, orb.in ());
    }
  else if (this->consumer_ec_control_ == 2)
    {
      int argc = 0;
      ACE_TCHAR **argv = 0;
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, this->orbid_.c_str ());
      ACE_Time_Value rate (0, this->consumer_ec_control_period_);
      return new TAO_ECG_Reconnect_ConsumerEC_Control (rate, consumer_ec_control_timeout_, gateway, orb.in ());
    }
  return 0;
}

void
TAO_EC_Gateway_IIOP_Factory::destroy_consumerec_control (
  TAO_ECG_ConsumerEC_Control* x)
{
   delete x;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_EC_Gateway_IIOP_Factory,
                       ACE_TEXT ("EC_Gateway_IIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_EC_Gateway_IIOP_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTEvent_Serv, TAO_EC_Gateway_IIOP_Factory)
