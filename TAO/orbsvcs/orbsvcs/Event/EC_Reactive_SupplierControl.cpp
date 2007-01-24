// $Id$

#include "orbsvcs/Event/EC_SupplierAdmin.h"
#include "orbsvcs/Event/EC_Reactive_SupplierControl.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/Event/EC_ProxyConsumer.h"
#include "orbsvcs/Event/EC_ProxySupplier.h" // @@ MSVC 6 bug

#include "tao/Messaging/Messaging.h"
#include "tao/ORB_Core.h"

#include "ace/Reactor.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Reactive_SupplierControl.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Reactive_SupplierControl, "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Reactive_SupplierControl::
     TAO_EC_Reactive_SupplierControl (const ACE_Time_Value &rate,
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

TAO_EC_Reactive_SupplierControl::~TAO_EC_Reactive_SupplierControl (void)
{
}

void
TAO_EC_Reactive_SupplierControl::query_suppliers (
      ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_EC_Ping_Supplier worker (this);
  this->event_channel_->for_each_supplier (&worker
                                           ACE_ENV_ARG_PARAMETER);
}

void
TAO_EC_Reactive_SupplierControl::handle_timeout (
      const ACE_Time_Value &,
      const void *)
{
  ACE_TRY_NEW_ENV
    {
      // Query the state of the Current object *before* we initiate
      // the iteration...
      CORBA::PolicyTypeSeq types;
      CORBA::PolicyList_var policies =
        this->policy_current_->get_policy_overrides (types
                                                      ACE_ENV_ARG_PARAMETER);

      // Change the timeout
      this->policy_current_->set_policy_overrides (this->policy_list_,
                                                   CORBA::ADD_OVERRIDE
                                                    ACE_ENV_ARG_PARAMETER);

      // Query the state of the suppliers...
      this->query_suppliers ();

      this->policy_current_->set_policy_overrides (policies.in (),
                                                   CORBA::SET_OVERRIDE
                                                    ACE_ENV_ARG_PARAMETER);
      for (CORBA::ULong i = 0; i != policies->length (); ++i)
        {
          policies[i]->destroy ();
        }
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions
    }
  ACE_ENDTRY;
}

int
TAO_EC_Reactive_SupplierControl::activate (void)
{
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  ACE_TRY_NEW_ENV
    {
      // Get the PolicyCurrent object
      CORBA::Object_var tmp =
        this->orb_->resolve_initial_references ("PolicyCurrent"
                                                 ACE_ENV_ARG_PARAMETER);

      this->policy_current_ =
        CORBA::PolicyCurrent::_narrow (tmp.in ()
                                        ACE_ENV_ARG_PARAMETER);

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
TAO_EC_Reactive_SupplierControl::shutdown (void)
{
  int r = 0;

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  r = this->reactor_->cancel_timer (timer_id_);
#endif /* TAO_HAS_CORBA_MESSAGING */
  this->adapter_.reactor (0);
  return r;
}

void
TAO_EC_Reactive_SupplierControl::supplier_not_exist (
      TAO_EC_ProxyPushConsumer *proxy
      ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      //ACE_DEBUG ((LM_DEBUG,
      //            "EC_Reactive_SupplierControl(%P|%t) - "
      //            "Consumer %x does not exists\n", long(proxy)));
      proxy->disconnect_push_consumer ();
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions..
    }
  ACE_ENDTRY;
}

void
TAO_EC_Reactive_SupplierControl::system_exception (
      TAO_EC_ProxyPushConsumer *proxy,
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
      proxy->disconnect_push_consumer ();
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions..
    }
  ACE_ENDTRY;
}

// ****************************************************************

TAO_EC_SupplierControl_Adapter::TAO_EC_SupplierControl_Adapter (
      TAO_EC_Reactive_SupplierControl *adaptee)
  :  adaptee_ (adaptee)
{
}

int
TAO_EC_SupplierControl_Adapter::handle_timeout (
      const ACE_Time_Value &tv,
      const void *arg)
{
  this->adaptee_->handle_timeout (tv, arg);
  return 0;
}

// ****************************************************************

void
TAO_EC_Ping_Supplier::work (TAO_EC_ProxyPushConsumer *consumer
                            ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      CORBA::Boolean disconnected;
      CORBA::Boolean non_existent =
        consumer->supplier_non_existent (disconnected
                                          ACE_ENV_ARG_PARAMETER);
      if (non_existent && !disconnected)
        {
          this->control_->supplier_not_exist (consumer ACE_ENV_ARG_PARAMETER);
        }
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
      this->control_->supplier_not_exist (consumer ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCH (CORBA::TRANSIENT, transient)
    {
      // This is TAO's minor code for a failed connection, we may
      // want to be more lenient in the future..
      // if (transient.minor () == 0x54410085)
      this->control_->supplier_not_exist (consumer ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      // Ignore all exceptions
    }
  ACE_ENDTRY;
}

TAO_END_VERSIONED_NAMESPACE_DECL
