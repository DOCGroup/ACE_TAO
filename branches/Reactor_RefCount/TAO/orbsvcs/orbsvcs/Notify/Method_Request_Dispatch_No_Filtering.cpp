// $Id$

#include "Method_Request_Dispatch_No_Filtering.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Dispatch_No_Filtering.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Method_Request_Dispatch_No_Filtering, "$Id$")

#include "tao/debug.h"
#include "ProxySupplier.h"
#include "Consumer.h"

TAO_NS_Method_Request_Dispatch_No_Filtering::TAO_NS_Method_Request_Dispatch_No_Filtering (const TAO_NS_Event_var& event, TAO_NS_ProxySupplier* proxy_supplier)
  : TAO_NS_Method_Request_Event (event), proxy_supplier_ (proxy_supplier), refcountable_guard_ (*proxy_supplier)
{
}

TAO_NS_Method_Request_Dispatch_No_Filtering::~TAO_NS_Method_Request_Dispatch_No_Filtering ()
{
}

TAO_NS_Method_Request*
TAO_NS_Method_Request_Dispatch_No_Filtering::copy (void)
{
  /// @@use factory
  return new TAO_NS_Method_Request_Dispatch_No_Filtering (this->event_, this->proxy_supplier_);
}

int
TAO_NS_Method_Request_Dispatch_No_Filtering::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->proxy_supplier_->has_shutdown ())
    return 0; // If we were shutdown while waiting in the queue, return with no action.

  ACE_TRY
    {
      this->proxy_supplier_->consumer ()->push (this->event_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "TAO_NS_Method_Request_Dispatch_No_Filtering::: error sending event. ");
      //ACE_RE_THROW;
      }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "TAO_NS_Method_Request_Dispatch_No_Filtering::: error sending event. ");
      //ACE_RE_THROW;
    }
  ACE_ENDTRY;

  return 0;
}
