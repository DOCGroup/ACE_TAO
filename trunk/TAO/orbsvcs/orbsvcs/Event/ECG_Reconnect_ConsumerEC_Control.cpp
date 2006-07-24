// $Id$

#include "orbsvcs/Event/EC_Gateway_IIOP.h"
#include "orbsvcs/Event/ECG_Reconnect_ConsumerEC_Control.h"
#include "tao/Messaging/Messaging.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

ACE_RCSID(Event, ECG_Reconnect_ConsumerEventChannelControl, "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ECG_Reconnect_ConsumerEC_Control::
     TAO_ECG_Reconnect_ConsumerEC_Control (const ACE_Time_Value &rate,
                                           const ACE_Time_Value &timeout,
                                           TAO_EC_Gateway_IIOP* gateway,
                                           CORBA::ORB_ptr orb)
  : rate_ (rate),
    timeout_ (timeout),
    adapter_ (this),
    gateway_ (gateway),
    orb_ (CORBA::ORB::_duplicate (orb)),
    is_consumer_ec_connected_ (1)
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
    , timer_id_ (-1)
#endif /* TAO_HAS_CORBA_MESSAGING != 0*/
{
  this->reactor_ =
    this->orb_->orb_core ()->reactor ();
}

TAO_ECG_Reconnect_ConsumerEC_Control::~TAO_ECG_Reconnect_ConsumerEC_Control (void)
{
}

void
TAO_ECG_Reconnect_ConsumerEC_Control::try_reconnect (
      ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
    {
      CORBA::Boolean disconnected;
      CORBA::Boolean non_existent =
      gateway_->consumer_ec_non_existent (disconnected
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (!non_existent)
        {
          this->reconnect(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions
    }
  ACE_ENDTRY;
}

void
TAO_ECG_Reconnect_ConsumerEC_Control::reconnect (
      ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
    {
      is_consumer_ec_connected_ = 1;

      gateway_->reconnect_consumer_ec(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions
    }
  ACE_ENDTRY;
}

void
TAO_ECG_Reconnect_ConsumerEC_Control::query_eventchannel (
      ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
    {
      if (is_consumer_ec_connected_ == 1)
        {
          CORBA::Boolean disconnected;
          CORBA::Boolean non_existent =
            gateway_->consumer_ec_non_existent (disconnected
                                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          if (non_existent && !disconnected)
            {
              this->event_channel_not_exist (gateway_ ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
      else
        {
           this->try_reconnect(ACE_ENV_SINGLE_ARG_PARAMETER);
           ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
      this->event_channel_not_exist (gateway_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::TRANSIENT, transient)
    {
      // This is TAO's minor code for a failed connection, we may
      // want to be more lenient in the future..
      // if (transient.minor () == 0x54410085)
      this->event_channel_not_exist (gateway_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions
    }
  ACE_ENDTRY;
}

void
TAO_ECG_Reconnect_ConsumerEC_Control::handle_timeout (
      const ACE_Time_Value &,
      const void *)
{
  // NOTE, setting RELATIVE_RT_TIMEOUT_POLICY for the duration of
  // query_eventchannel () below has greater impact than desired.  For
  // example, while we are pinging consumers here, a nested upcall,
  // which requires making remote calls may come into the ORB.  Those
  // remote calls will be carried out with with
  // RELATIVE_RT_TIMEOUT_POLICY set here in effect.
  // @@ TODO: should use Guard to set and reset policies.
  ACE_TRY_NEW_ENV
    {
      // Query the state of the Current object *before* we initiate
      // the iteration...
      CORBA::PolicyTypeSeq types;
      CORBA::PolicyList_var policies =
        this->policy_current_->get_policy_overrides (types
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Change the timeout
      this->policy_current_->set_policy_overrides (this->policy_list_,
                                                   CORBA::ADD_OVERRIDE
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Query the state of the consumers...
      this->query_eventchannel (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->policy_current_->set_policy_overrides (policies.in (),
                                                   CORBA::SET_OVERRIDE
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      for (CORBA::ULong i = 0; i != policies->length (); ++i)
        {
          policies[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions
    }
  ACE_ENDTRY;
}

int
TAO_ECG_Reconnect_ConsumerEC_Control::activate (void)
{
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  ACE_TRY_NEW_ENV
    {
      // Get the PolicyCurrent object
      CORBA::Object_var tmp =
        this->orb_->resolve_initial_references ("PolicyCurrent"
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->policy_current_ =
        CORBA::PolicyCurrent::_narrow (tmp.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Timeout for polling state (default = 10 msec)
      TimeBase::TimeT timeout = timeout_.usec() * 10;
      CORBA::Any any;
      any <<= timeout;

      this->policy_list_.length (1);
      this->policy_list_[0] =
        this->orb_->create_policy (
               Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
               any
               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Only schedule the timer, when the rate is not zero
      if (this->rate_ != ACE_Time_Value::zero)
      {
        // Schedule the timer after these policies has been set, because the
        // handle_timeout uses these policies, if done in front, the channel
        // can crash when the timeout expires before initiazation is ready.
        timer_id_ = this->reactor_->schedule_timer (&this->adapter_,
                                                    0,
                                                    this->rate_,
                                                    this->rate_);
        if (timer_id_ == -1)
          return -1;
      }
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;
#endif /* TAO_HAS_CORBA_MESSAGING */

  return 0;
}

int
TAO_ECG_Reconnect_ConsumerEC_Control::shutdown (void)
{
  int r = 0;

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  r = this->reactor_->cancel_timer (timer_id_);
#endif /* TAO_HAS_CORBA_MESSAGING */

  this->adapter_.reactor (0);
  return r;
}

void
TAO_ECG_Reconnect_ConsumerEC_Control::event_channel_not_exist (
      TAO_EC_Gateway_IIOP* gateway
      ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      //ACE_DEBUG ((LM_DEBUG,
      //            "ECG_Reconnect_ConsumerControl(%P|%t) - "
      //            "channel %x does not exists\n"));
      is_consumer_ec_connected_ = 0;

      gateway->suspend_supplier_ec (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      gateway->cleanup_consumer_proxies (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO_EC_Reconnect_ConsumerControl::event_channel_not_exist");
      // Ignore all exceptions..
    }
  ACE_ENDTRY;
}

void
TAO_ECG_Reconnect_ConsumerEC_Control::system_exception (
      TAO_EC_Gateway_IIOP* gateway,
      CORBA::SystemException & /* exception */
      ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      // The current implementation is very strict, and kicks out a
      // client on the first system exception. We may
      // want to be more lenient in the future, for example,
      // this is TAO's minor code for a failed connection.
      //
      // if (CORBA::TRANSIENT::_narrow (&exception) != 0
      //     && exception->minor () == 0x54410085)
      //   return;

      // Anything else is serious, including timeouts...
      //ACE_DEBUG ((LM_DEBUG,
      //            "ECG_Reconnect_ConsumerControl(%P|%t) - "
      //            "channel %x does not exists system except\n"));
      is_consumer_ec_connected_ = 0;

      gateway->suspend_supplier_ec (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      gateway->cleanup_consumer_proxies (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions..
    }
  ACE_ENDTRY;
}

// ****************************************************************

TAO_ECG_Reconnect_ConsumerEC_Control_Adapter::TAO_ECG_Reconnect_ConsumerEC_Control_Adapter (
      TAO_ECG_Reconnect_ConsumerEC_Control *adaptee)
  :  adaptee_ (adaptee)
{
}

int
TAO_ECG_Reconnect_ConsumerEC_Control_Adapter::handle_timeout (
      const ACE_Time_Value &tv,
      const void *arg)
{
  this->adaptee_->handle_timeout (tv, arg);
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
