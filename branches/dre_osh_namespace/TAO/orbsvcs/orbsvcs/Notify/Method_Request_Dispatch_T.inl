// $Id$

#include "Admin.h"

template <class EVENT, class PROXY, class EVENT_PARAM, class PROXY_PARAM> ACE_INLINE int
TAO_Notify_Method_Request_Dispatch_T<EVENT, PROXY, EVENT_PARAM, PROXY_PARAM>::execute_i (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->proxy_supplier_->has_shutdown ())
    return 0; // If we were shutdown while waiting in the queue, return with no action.

  if (this->filtering_ == 1)
    {
      TAO_Notify_Admin* parent = this->proxy_supplier_->consumer_admin ();

      CORBA::Boolean val =  this->proxy_supplier_->check_filters (this->event_,
                                                                  parent->filter_admin (),
                                                                  parent->filter_operator ()
                                                                  ACE_ENV_ARG_PARAMETER);

      if (TAO_debug_level > 1)
        ACE_DEBUG ((LM_DEBUG, "Proxysupplier %x filter eval result = %d",&this->proxy_supplier_ , val));

      // Filter failed - do nothing.
      if (val == 0)
        return 0;
    }

  ACE_TRY
    {
      TAO_Notify_Consumer* consumer = this->proxy_supplier_->consumer ();

      if (consumer != 0)
        {
          consumer->push (this->event_ ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Notify_Method_Request_Dispatch::: error sending event. \n ");
    }
  ACE_ENDTRY;

  return 0;
}
