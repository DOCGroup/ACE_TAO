// $Id$

#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/Event/EC_Reactive_ConsumerControl.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"
#include "orbsvcs/Event/EC_ProxyConsumer.h" // @@ MSVC 6 bug

#include "tao/Messaging/Messaging.h"
#include "tao/ORB_Core.h"

#include "ace/Reactor.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Reactive_ConsumerControl.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Reactive_ConsumerControl::
     TAO_EC_Reactive_ConsumerControl (const ACE_Time_Value &rate,
                                      const ACE_Time_Value &timeout,
                                      TAO_EC_Event_Channel_Base *ec,
                                      CORBA::ORB_ptr orb)
  : rate_ (rate),
    timeout_ (timeout),
    adapter_ (this),
    event_channel_ (ec),
    orb_ (CORBA::ORB::_duplicate (orb))
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
    , timer_id_ (-1)
#endif /* TAO_HAS_CORBA_MESSAGING != 0*/
{
  this->reactor_ =
    this->orb_->orb_core ()->reactor ();
}

TAO_EC_Reactive_ConsumerControl::~TAO_EC_Reactive_ConsumerControl (void)
{
}

void
TAO_EC_Reactive_ConsumerControl::query_consumers ()
{
  TAO_EC_Ping_Consumer worker (this);
  this->event_channel_->for_each_consumer (&worker);
}

void
TAO_EC_Reactive_ConsumerControl::handle_timeout (
      const ACE_Time_Value &,
      const void *)
{
  // NOTE, setting RELATIVE_RT_TIMEOUT_POLICY for the duration of
  // query_consumers () below has greater impact than desired.  For
  // example, while we are pinging consumers here, a nested upcall,
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
      this->query_consumers ();

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
TAO_EC_Reactive_ConsumerControl::activate (void)
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
               Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE, any);

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
TAO_EC_Reactive_ConsumerControl::shutdown (void)
{
  int r = 0;

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  r = this->reactor_->cancel_timer (timer_id_);
#endif /* TAO_HAS_CORBA_MESSAGING */
  this->adapter_.reactor (0);
  return r;
}

void
TAO_EC_Reactive_ConsumerControl::consumer_not_exist (
      TAO_EC_ProxyPushSupplier *proxy)
{
  try
    {
      //ACE_DEBUG ((LM_DEBUG,
      //            "EC_Reactive_ConsumerControl(%P|%t) - "
      //            "Consumer %x does not exists\n", long(proxy)));
      proxy->disconnect_push_supplier ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Reactive_ConsumerControl::consumer_not_exist");
      // Ignore all exceptions..
    }
}

void
TAO_EC_Reactive_ConsumerControl::system_exception (
      TAO_EC_ProxyPushSupplier *proxy,
      CORBA::SystemException & /* exception */)
{
  try
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
      proxy->disconnect_push_supplier ();
    }
  catch (const CORBA::Exception&)
    {
      // Ignore all exceptions..
    }
}

// ****************************************************************

TAO_EC_ConsumerControl_Adapter::TAO_EC_ConsumerControl_Adapter (
      TAO_EC_Reactive_ConsumerControl *adaptee)
  :  adaptee_ (adaptee)
{
}

int
TAO_EC_ConsumerControl_Adapter::handle_timeout (
      const ACE_Time_Value &tv,
      const void *arg)
{
  this->adaptee_->handle_timeout (tv, arg);
  return 0;
}

// ****************************************************************

void
TAO_EC_Ping_Consumer::work (TAO_EC_ProxyPushSupplier *supplier)
{
  try
    {
      CORBA::Boolean disconnected;
      CORBA::Boolean non_existent =
        supplier->consumer_non_existent (disconnected);
      if (non_existent && !disconnected)
        {
          this->control_->consumer_not_exist (supplier);
        }
    }
  catch (const CORBA::OBJECT_NOT_EXIST&)
    {
      this->control_->consumer_not_exist (supplier);
    }
  catch (const CORBA::TRANSIENT&)
    {
      // This is TAO's minor code for a failed connection, we may
      // want to be more lenient in the future..
      // if (transient.minor () == 0x54410085)
      this->control_->consumer_not_exist (supplier);
    }
  catch (const CORBA::Exception&)
    {
      // Ignore all exceptions
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
