// $Id$

#include "Method_Request_Dispatch.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Dispatch.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Method_Request_Dispatch, "$Id$")

#include "tao/debug.h"
#include "ProxySupplier.h"
#include "Consumer.h"
#include "Admin.h"
#include "ConsumerAdmin.h"

TAO_Notify_Method_Request_Dispatch::TAO_Notify_Method_Request_Dispatch (
      const TAO_Notify_Event * event,
      TAO_Notify_ProxySupplier* proxy_supplier,
      bool filtering)
  : TAO_Notify_Method_Request_Event (event)
  , proxy_supplier_ (proxy_supplier)
  , filtering_ (filtering)
{
}

TAO_Notify_Method_Request_Dispatch::~TAO_Notify_Method_Request_Dispatch ()
{
}

TAO_Notify_Method_Request_Dispatch::execute_i (ACE_ENV_SINGLE_ARG_DECL)
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
          consumer->deliver (this ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
        ACE_TEXT ("TAO_Notify_Method_Request_Dispatch::: error sending event.\n ")
        );
    }
  ACE_ENDTRY;

  return 0;
}

///////////////////////////////////////////////////////////////////////////////


/*******************************************************************/

TAO_Notify_Method_Request_Event::TAO_Notify_Method_Request_Event (
  const TAO_Notify_Event * event)
  : event_ (event)
{
}


TAO_Notify_Method_Request_Event::TAO_Notify_Method_Request_Event (
    const TAO_Notify_Method_Request_Event & rhs,
    const TAO_Notify_Event * event)
  : event_ (event)
{
}

TAO_Notify_Method_Request_Event::~TAO_Notify_Method_Request_Event()
{
}

void
TAO_Notify_Method_Request_Event::complete ()
{
  int todo_request_complete;
}


unsigned long
TAO_Notify_Method_Request_Event::sequence ()
{
  int todo_request_sequence;
  return 0;
}

bool
TAO_Notify_Method_Request_Event::should_retry ()
{
  int todo_request_should_retry;
  return false;
}

/**********************************************************/

TAO_Notify_Method_Request_Dispatch_Queueable::TAO_Notify_Method_Request_Dispatch_Queueable (const TAO_Notify_Event_var& event, TAO_Notify_ProxySupplier* proxy_supplier, CORBA::Boolean filtering)
  : TAO_Notify_Method_Request_Queueable (event.get ())
  , TAO_Notify_Method_Request_Dispatch (event.get(), proxy_supplier, filtering)
  , event_var_ (event)
  , proxy_guard_ (proxy_supplier)
{
#if 0
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P|%t) Construct Method_Request_Dispatch @%@\n"),
    this));
#endif
}

TAO_Notify_Method_Request_Dispatch_Queueable::~TAO_Notify_Method_Request_Dispatch_Queueable ()
{
#if 0
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P|%t) Destroy Method_Request_Dispatch @%@\n"),
    this));
#endif
}

int
TAO_Notify_Method_Request_Dispatch_Queueable::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/*********************************************************************************************************/

TAO_Notify_Method_Request_Dispatch_No_Copy::TAO_Notify_Method_Request_Dispatch_No_Copy (const TAO_Notify_Event* event, TAO_Notify_ProxySupplier* proxy_supplier, CORBA::Boolean filtering)
  : TAO_Notify_Method_Request_Dispatch (event, proxy_supplier, filtering)
{
#if 0
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Construct Method_Request_Dispatch_No_Copy @%@\n"),
      this));
#endif

}

TAO_Notify_Method_Request_Dispatch_No_Copy:: ~TAO_Notify_Method_Request_Dispatch_No_Copy ()
{
#if 0
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P|%t) Destroy Method_Request_Dispatch_No_Copy @%@\n"),
    this));
#endif
}

int
TAO_Notify_Method_Request_Dispatch_No_Copy::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

TAO_Notify_Method_Request_Queueable*
TAO_Notify_Method_Request_Dispatch_No_Copy::copy (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify_Method_Request_Queueable* request;

  const TAO_Notify_Event * event_copy = this->event_->queueable_copy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_Notify_Event_Copy_var event_var (event_copy);

  ACE_NEW_THROW_EX (request,
                    TAO_Notify_Method_Request_Dispatch_Queueable (event_var, this->proxy_supplier_, this->filtering_),
                    CORBA::INTERNAL ());

  return request;
}

/*********************************************************************************************************/

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
