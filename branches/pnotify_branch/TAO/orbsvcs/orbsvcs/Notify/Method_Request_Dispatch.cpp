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

TAO_Notify_Method_Request_Dispatch::TAO_Notify_Method_Request_Dispatch (const TAO_Notify_Event_var& event, TAO_Notify_ProxySupplier* proxy_supplier, CORBA::Boolean filtering)
  : TAO_Notify_Method_Request (event.get ())
  , TAO_Notify_Method_Request_Dispatch_Base (event.get(), proxy_supplier, filtering)
  , event_var_ (event)
  , proxy_guard_ (proxy_supplier)
{
#if 0
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P|%t) Construct Method_Request_Dispatch @%@\n"),
    this));
#endif
}

TAO_Notify_Method_Request_Dispatch::~TAO_Notify_Method_Request_Dispatch ()
{
#if 0
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P|%t) Destroy Method_Request_Dispatch @%@\n"),
    this));
#endif
}

int
TAO_Notify_Method_Request_Dispatch::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/*********************************************************************************************************/

TAO_Notify_Method_Request_Dispatch_No_Copy::TAO_Notify_Method_Request_Dispatch_No_Copy (const TAO_Notify_Event* event, TAO_Notify_ProxySupplier* proxy_supplier, CORBA::Boolean filtering)
  : TAO_Notify_Method_Request_Dispatch_Base (event, proxy_supplier, filtering)
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

TAO_Notify_Method_Request*
TAO_Notify_Method_Request_Dispatch_No_Copy::copy (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify_Method_Request* request;

  const TAO_Notify_Event * event_copy = this->event_->copy_on_heap (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_Notify_Event_Copy_var event_var (event_copy);

  ACE_NEW_THROW_EX (request,
                    TAO_Notify_Method_Request_Dispatch (event_var, this->proxy_supplier_, this->filtering_),
                    CORBA::INTERNAL ());

  return request;
}

/*********************************************************************************************************/

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
