// $Id$

#include "EC_Reactive_SupplierControl.h"
#include "EC_Event_Channel.h"
#include "EC_SupplierAdmin.h"
#include "EC_ProxyConsumer.h"

#if ! defined (__ACE_INdLINE__)
#include "EC_Reactive_SupplierControl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Reactive_SupplierControl, "$Id$")

TAO_EC_Reactive_SupplierControl::
     TAO_EC_Reactive_SupplierControl (const ACE_Time_Value &rate,
                                      TAO_EC_Event_Channel *ec,
                                      CORBA::ORB_ptr orb)
  : rate_ (rate),
    adapter_ (this),
    event_channel_ (ec),
    orb_ (CORBA::ORB::_duplicate (orb))
{
  this->reactor_ =
    this->orb_->orb_core ()->reactor ();
}

TAO_EC_Reactive_SupplierControl::~TAO_EC_Reactive_SupplierControl (void)
{
}

void
TAO_EC_Reactive_SupplierControl::query_suppliers (
      CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_SupplierAdmin *supplier_admin =
    this->event_channel_->supplier_admin ();
      
  TAO_EC_SupplierAdmin::Busy_Lock &lock =
    supplier_admin->busy_lock ();

  ACE_GUARD (TAO_EC_SupplierAdmin::Busy_Lock, ace_mon, lock);
  
  TAO_EC_SupplierAdmin::ConsumerSetIterator end =
    supplier_admin->end ();
  for (TAO_EC_SupplierAdmin::ConsumerSetIterator i =
         supplier_admin->begin ();
       i != end;
       ++i)
    {
      TAO_EC_ProxyPushConsumer *proxy = *i;
      ACE_TRY
        {
          CORBA::Boolean disconnected;
          CORBA::Boolean non_existent =
            proxy->supplier_non_existent (disconnected,
                                          ACE_TRY_ENV);
          ACE_TRY_CHECK;
          if (non_existent && !disconnected)
            {
              this->supplier_not_exist (proxy, ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
        }
      ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
        {
          this->supplier_not_exist (proxy, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CORBA::TRANSIENT, transient)
        {
          // This is TAO's minor code for a failed connection, we may
          // want to be more lenient in the future..
          // if (transient.minor () == 0x54410085)
          this->supplier_not_exist (proxy, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // Ignore all exceptions
        }
      ACE_ENDTRY;
    }
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
        this->policy_current_->get_policy_overrides (types,
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Change the timeout
      this->policy_current_->set_policy_overrides (this->policy_list_,
                                                   CORBA::ADD_OVERRIDE,
                                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Query the state of the suppliers...
      this->query_suppliers (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->policy_current_->set_policy_overrides (policies.in (),
                                                   CORBA::SET_OVERRIDE,
                                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;
      for (CORBA::ULong i = 0; i != policies->length (); ++i)
        {
          policies[i]->destroy (ACE_TRY_ENV);
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
TAO_EC_Reactive_SupplierControl::activate (void)
{
  long id = this->reactor_->schedule_timer (&this->adapter_,
                                            0,
                                            this->rate_,
                                            this->rate_);
  if (id == -1)
    return -1;
  
  ACE_TRY_NEW_ENV
    {
      // Get the PolicyCurrent object
      CORBA::Object_var tmp =
        this->orb_->resolve_initial_references ("PolicyCurrent",
                                                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->policy_current_ =
        CORBA::PolicyCurrent::_narrow (tmp.in (),
                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Pre-compute the policy list to the set the right timeout
      // value...
      // @@ TODO It is currently hard-coded to 10 milliseconds
      TimeBase::TimeT timeout = 10 * 10000;
      CORBA::Any any;
      any <<= timeout;

      this->policy_list_.length (1);
      this->policy_list_[0] =
        this->orb_->create_policy (
               Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
               any,
               ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
TAO_EC_Reactive_SupplierControl::shutdown (void)
{
  return this->reactor_->remove_handler (&this->adapter_,
                                         ACE_Event_Handler::DONT_CALL);
}

void
TAO_EC_Reactive_SupplierControl::supplier_not_exist (
      TAO_EC_ProxyPushConsumer *proxy,
      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      proxy->disconnect_push_consumer (ACE_TRY_ENV);
      ACE_TRY_CHECK;
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
      CORBA::SystemException & /* exception */,
      CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      // This is TAO's minor code for a failed connection, we may
      // want to be more lenient in the future..
      // if (CORBA::TRANSIENT::_narrow (&exception) != 0
      //     && exception->minor () == 0x54410085)
      //   return;

      // Anything else is serious, including timeouts...
      proxy->disconnect_push_consumer (ACE_TRY_ENV);
      ACE_TRY_CHECK;
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
