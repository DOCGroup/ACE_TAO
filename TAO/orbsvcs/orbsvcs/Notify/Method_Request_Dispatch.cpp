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
  : TAO_Notify_Method_Request_Dispatch_Base (event, proxy_supplier, filtering)
{
  this->init (event);
}

TAO_Notify_Method_Request_Dispatch::~TAO_Notify_Method_Request_Dispatch ()
{
}

int
TAO_Notify_Method_Request_Dispatch::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/*********************************************************************************************************/

TAO_Notify_Method_Request_Dispatch_No_Copy::TAO_Notify_Method_Request_Dispatch_No_Copy (const TAO_Notify_Event* event, TAO_Notify_ProxySupplier* proxy_supplier, CORBA::Boolean filtering)
  : TAO_Notify_Method_Request_Dispatch_No_Copy_Base (event, proxy_supplier, filtering)
{
}

TAO_Notify_Method_Request_Dispatch_No_Copy:: ~TAO_Notify_Method_Request_Dispatch_No_Copy ()
{
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

  TAO_Notify_Event* event_copy = this->event_->copy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_Notify_Event_Copy_var event_var (event_copy);

  ACE_NEW_THROW_EX (request,
                    TAO_Notify_Method_Request_Dispatch (event_var, this->proxy_supplier_, this->filtering_),
                    CORBA::INTERNAL ());

  return request;
}

/*********************************************************************************************************/

TAO_Notify_Method_Request_Dispatch_No_Copy_Ex::TAO_Notify_Method_Request_Dispatch_No_Copy_Ex (const TAO_Notify_Event_var& event, TAO_Notify_ProxySupplier* proxy_supplier, CORBA::Boolean filtering)
  : TAO_Notify_Method_Request_Dispatch_No_Copy_Ex_Base (event, proxy_supplier, filtering)
{
}

TAO_Notify_Method_Request_Dispatch_No_Copy_Ex:: ~TAO_Notify_Method_Request_Dispatch_No_Copy_Ex ()
{
}

int
TAO_Notify_Method_Request_Dispatch_No_Copy_Ex::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

TAO_Notify_Method_Request*
TAO_Notify_Method_Request_Dispatch_No_Copy_Ex::copy (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify_Method_Request* request;

  ACE_NEW_THROW_EX (request,
                    TAO_Notify_Method_Request_Dispatch (this->event_, this->proxy_supplier_, this->filtering_),
                    CORBA::INTERNAL ());

  return request;
}



#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_Method_Request_Dispatch_T<const TAO_Notify_Event_var
, TAO_Notify_ProxySupplier_Guard
, const TAO_Notify_Event_var&
, TAO_Notify_ProxySupplier*>;

template class TAO_Notify_Method_Request_Dispatch_T<const TAO_Notify_Event*
, TAO_Notify_ProxySupplier*
, const TAO_Notify_Event*
, TAO_Notify_ProxySupplier*>;

template class TAO_Notify_Method_Request_Dispatch_T<const TAO_Notify_Event_var&
, TAO_Notify_ProxySupplier*
, const TAO_Notify_Event_var&
, TAO_Notify_ProxySupplier*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_Method_Request_Dispatch_T<const TAO_Notify_Event_var
, TAO_Notify_ProxySupplier_Guard
, const TAO_Notify_Event_var&
, TAO_Notify_ProxySupplier*>

#pragma instantiate TAO_Notify_Method_Request_Dispatch_T<const TAO_Notify_Event*
, TAO_Notify_ProxySupplier*
, const TAO_Notify_Event*
, TAO_Notify_ProxySupplier*>

#pragma instantiate TAO_Notify_Method_Request_Dispatch_T<const TAO_Notify_Event_var&
, TAO_Notify_ProxySupplier*
, const TAO_Notify_Event_var&
, TAO_Notify_ProxySupplier*>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
