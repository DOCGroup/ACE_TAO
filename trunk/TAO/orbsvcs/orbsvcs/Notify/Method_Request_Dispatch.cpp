// $Id$

#include "Method_Request_Dispatch.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Dispatch.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Method_Request_Dispatch, "$Id$")

#include "tao/debug.h"
#include "ProxySupplier.h"
#include "Consumer.h"
#include "Admin.h"
#include "ConsumerAdmin.h"

TAO_NS_Method_Request_Dispatch::TAO_NS_Method_Request_Dispatch (const TAO_NS_Event_var& event, TAO_NS_ProxySupplier* proxy_supplier)
  : TAO_NS_Method_Request_Event (event), proxy_supplier_ (proxy_supplier), refcountable_guard_ (*proxy_supplier)
{
}

TAO_NS_Method_Request_Dispatch::~TAO_NS_Method_Request_Dispatch ()
{
}

TAO_NS_Method_Request*
TAO_NS_Method_Request_Dispatch::copy (void)
{
  /// @@use factory
  return new TAO_NS_Method_Request_Dispatch (this->event_, this->proxy_supplier_);
}

int
TAO_NS_Method_Request_Dispatch::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->proxy_supplier_->has_shutdown ())
    return 0; // If we were shutdown while waiting in the queue, return with no action.

  TAO_NS_Admin* parent = this->proxy_supplier_->consumer_admin ();

  CORBA::Boolean val =  this->proxy_supplier_->check_filters (this->event_,
                                                              parent->filter_admin (),
                                                              parent->filter_operator ()
                                                              ACE_ENV_ARG_PARAMETER);

  if (TAO_debug_level > 1)
    ACE_DEBUG ((LM_DEBUG, "Proxysupplier %x filter eval result = %d",this->proxy_supplier_ , val));

  // Filter failed - do nothing.
  if (val == 0)
    return 0;

  ACE_TRY
    {
      TAO_NS_Consumer* consumer = this->proxy_supplier_->consumer ();

      if (consumer != 0)
        {
          consumer->push (this->event_ ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_NS_Method_Request_Dispatch::: error sending event. \n ");
    }
  ACE_ENDTRY;

  return 0;
}
