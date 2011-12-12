// $Id$

#include "orbsvcs/Event/EC_Gateway_IIOP.h"
#include "orbsvcs/Event/ECG_Reactive_ConsumerEC_Control.h"
#include "tao/Messaging/Messaging.h"
#include "tao/ORB_Core.h"

#include "ace/Reactor.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ECG_Reactive_ConsumerEC_Control::
     TAO_ECG_Reactive_ConsumerEC_Control (const ACE_Time_Value &rate,
                                          const ACE_Time_Value &timeout,
                                          TAO_EC_Gateway_IIOP* gateway,
                                          CORBA::ORB_ptr orb)
  : rate_ (rate),
    timeout_ (timeout),
    adapter_ (this),
    gateway_ (gateway),
    orb_ (CORBA::ORB::_duplicate (orb))
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
    , timer_id_ (-1)
#endif /* TAO_HAS_CORBA_MESSAGING != 0*/
{
  this->reactor_ =
    this->orb_->orb_core ()->reactor ();
}

TAO_ECG_Reactive_ConsumerEC_Control::~TAO_ECG_Reactive_ConsumerEC_Control (void)
{
}

void
TAO_ECG_Reactive_ConsumerEC_Control::query_eventchannel ()
{
  try
    {
      CORBA::Boolean disconnected;
      CORBA::Boolean non_existent =
        gateway_->consumer_ec_non_existent (disconnected);
      if (non_existent && !disconnected)
        {
          this->event_channel_not_exist (gateway_);
        }
    }
  catch (const CORBA::OBJECT_NOT_EXIST&)
    {
      this->event_channel_not_exist (gateway_);
    }
  catch (const CORBA::TRANSIENT&)
    {
      // This is TAO's minor code for a failed connection, we may
      // want to be more lenient in the future..
      // if (transient.minor () == 0x54410085)
      this->event_channel_not_exist (gateway_);
    }
  catch (const CORBA::Exception&)
    {
      // Ignore all exceptions
    }
}

void
TAO_ECG_Reactive_ConsumerEC_Control::handle_timeout (
      const ACE_Time_Value &,
      const void *)
{
  // NOTE, setting RELATIVE_RT_TIMEOUT_POLICY for the duration of
  // query_eventchannel () below has greater impact than desired.  For
  // example, while we are pinging ec here, a nested upcall,
  // which requires making remote calls may come into the ORB.  Those
  // remote calls will be carried out with with
  // RELATIVE_RT_TIMEOUT_POLICY set here in effect.

  // @@ TODO: should use Guard to set and reset policies.
  try
    {
      // Query the state of the Current object *before* we initiate
      // the iteration...
      CORBA::PolicyTypeSeq types;
      CORBA::PolicyList_var policies =
        this->policy_current_->get_policy_overrides (types);

      // Change the timeout
      this->policy_current_->set_policy_overrides (this->policy_list_,
                                                   CORBA::ADD_OVERRIDE);

      // Query the state of the consumers...
      this->query_eventchannel ();

      this->policy_current_->set_policy_overrides (policies.in (),
                                                   CORBA::SET_OVERRIDE);
      for (CORBA::ULong i = 0; i != policies->length (); ++i)
        {
          policies[i]->destroy ();
        }
    }
  catch (const CORBA::Exception&)
    {
      // Ignore all exceptions
    }
}

int
TAO_ECG_Reactive_ConsumerEC_Control::activate (void)
{
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  try
    {
      // Get the PolicyCurrent object
      CORBA::Object_var tmp =
        this->orb_->resolve_initial_references ("PolicyCurrent");

      this->policy_current_ =
        CORBA::PolicyCurrent::_narrow (tmp.in ());

      // Timeout for polling state (default = 10 msec)
      TimeBase::TimeT timeout = timeout_.usec() * 10;
      CORBA::Any any;
      any <<= timeout;

      this->policy_list_.length (1);
      this->policy_list_[0] =
        this->orb_->create_policy (
               Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
               any);

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
  catch (const CORBA::Exception&)
    {
      return -1;
    }
#endif /* TAO_HAS_CORBA_MESSAGING */

  return 0;
}

int
TAO_ECG_Reactive_ConsumerEC_Control::shutdown (void)
{
  int r = 0;

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  r = this->reactor_->cancel_timer (timer_id_);
#endif /* TAO_HAS_CORBA_MESSAGING */

  this->adapter_.reactor (0);
  return r;
}

void
TAO_ECG_Reactive_ConsumerEC_Control::event_channel_not_exist (
      TAO_EC_Gateway_IIOP* gateway)
{
  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  "EC_Reactive_ConsumerControl(%P|%t) - "
                  "channel %x does not exists\n"));
      gateway->cleanup_consumer_ec ();

      gateway->cleanup_consumer_proxies ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_EC_Reactive_ConsumerControl::event_channel_not_exist");
      // Ignore all exceptions..
    }
}

void
TAO_ECG_Reactive_ConsumerEC_Control::system_exception (
      TAO_EC_Gateway_IIOP* gateway,
      CORBA::SystemException & /* exception */)
{
  try
    {
      gateway->cleanup_consumer_ec ();

      gateway->cleanup_consumer_proxies ();

    }
  catch (const CORBA::Exception&)
    {
      // Ignore all exceptions..
    }
}

// ****************************************************************

TAO_ECG_Reactive_ConsumerEC_Control_Adapter::TAO_ECG_Reactive_ConsumerEC_Control_Adapter (
      TAO_ECG_Reactive_ConsumerEC_Control *adaptee)
  :  adaptee_ (adaptee)
{
}

int
TAO_ECG_Reactive_ConsumerEC_Control_Adapter::handle_timeout (
      const ACE_Time_Value &tv,
      const void *arg)
{
  this->adaptee_->handle_timeout (tv, arg);
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
