// $Id$

#include "tao/corba.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Supplier.h"
#include "tao/debug.h"
#include "tao/Messaging/Messaging_TypesC.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Supplier.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/Notify/ProxyConsumer.h"
#include "orbsvcs/Notify/Proxy.h"

#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Supplier::TAO_Notify_Supplier (TAO_Notify_ProxyConsumer* proxy)
  : proxy_ (proxy)
{
}

TAO_Notify_Supplier::~TAO_Notify_Supplier ()
{
}

TAO_Notify_Proxy*
TAO_Notify_Supplier::proxy (void)
{
  return this->proxy_consumer ();
}

void
TAO_Notify_Supplier::dispatch_updates_i (
  const CosNotification::EventTypeSeq& added,
  const CosNotification::EventTypeSeq& removed)
{
  if (!CORBA::is_nil (this->subscribe_.in ()))
    this->subscribe_->subscription_change (added, removed);
}



bool
TAO_Notify_Supplier::is_alive (bool allow_nil_supplier)
{
  bool status = false;
  CORBA::Object_var supplier = this->get_supplier ();
  if (CORBA::is_nil (supplier.in ()))
  {
    // The supplier may not connected or the supplier did
    // not provide a callback. In this case, the liveliness
    // check should return true so it will be validated in 
    // next period. 
    if (allow_nil_supplier)
      return true;
    else
      return status;
  }

  CORBA::PolicyList policy_list;
  try
    {
      bool do_liveliness_check = false;
      ACE_Time_Value now = ACE_OS::gettimeofday ();
      ACE_Time_Value last_ping = proxy_->last_ping();
      if (CORBA::is_nil (this->rtt_obj_.in ()))
      {
        // We need to determine if the supplier on the other end is still
        // alive.  Since we may be in an upcall from the owner of the
        // original supplier, we have to put a timeout on the call in case
        // the client side is not processing ORB requests at this time.  In
        // the event that the timeout exception occurs, we will assume that
        // the original supplier is still around.  If we get any other
        // exception we will say that the original supplier is not
        // available anymore.
        TimeBase::TimeT timeout = 10000000;
        CORBA::Any timeout_any;
        timeout_any <<= timeout;

        policy_list.length (1);
        policy_list[0] = TAO_Notify_PROPERTIES::instance()->orb()->
                          create_policy (
                                Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                                timeout_any);
        rtt_obj_ =
          supplier->_set_policy_overrides (policy_list,
                                          CORBA::ADD_OVERRIDE);

        // Clean up the policy that was allocated in the try/catch
        for (CORBA::ULong i = 0; i < policy_list.length (); i++)
          policy_list[i]->destroy ();

        do_liveliness_check 
          = (last_ping == ACE_Time_Value::zero ? true 
          : now - last_ping >= TAO_Notify_PROPERTIES::instance()->validate_client_delay ());
      }
      else
        do_liveliness_check = 
          now - last_ping >= TAO_Notify_PROPERTIES::instance()->validate_client_interval ();

      if (CORBA::is_nil (rtt_obj_.in ()))
        status = false;
      else if (do_liveliness_check || allow_nil_supplier)
      {
        this->proxy_->last_ping (now);
        status = !rtt_obj_->_non_existent ();
      }
      else
        status = true;
    }
  catch (CORBA::TIMEOUT&)
    {
       status = true;
    }
  catch (CORBA::Exception& ex)
    {
      if (DEBUG_LEVEL > 0)
      {        
        ex._tao_print_exception ("TAO_Notify_Supplier::is_alive: false");
      }
    }

  return status;
}

TAO_END_VERSIONED_NAMESPACE_DECL
