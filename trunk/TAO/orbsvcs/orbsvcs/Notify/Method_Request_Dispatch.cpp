// $Id$

#include "Method_Request_Dispatch.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Dispatch.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Method_Request_Dispatch, "$Id$")

#include "tao/debug.h"
#include "ProxySupplier.h"
#include "Consumer.h"
#include "Proxy.h"
#include "Admin.h"
#include "EventChannel.h"
#include "EventChannelFactory.h"
#include "Notify_Service.h"

TAO_NS_Method_Request_Dispatch::TAO_NS_Method_Request_Dispatch (TAO_NS_Event_var& event, TAO_NS_ProxySupplier* proxy_supplier)
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

  CORBA::Boolean val =  this->proxy_supplier_->check_filters (this->event_ ACE_ENV_ARG_PARAMETER);

  if (TAO_debug_level > 1)
    ACE_DEBUG ((LM_DEBUG, "Proxysupplier %x filter eval result = %d",this->proxy_supplier_ , val));

  // Filter failed - do nothing.
  if (val == 0)
    return 0;

  ACE_TRY
    {
      this->proxy_supplier_->consumer ()->push (this->event_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "TAO_NS_Method_Request_Dispatch::: error sending event. ");
      //ACE_RE_THROW;
      }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "TAO_NS_Method_Request_Dispatch::: error sending event. ");
      //ACE_RE_THROW;
    }
  ACE_ENDTRY;

  return 0;
}
