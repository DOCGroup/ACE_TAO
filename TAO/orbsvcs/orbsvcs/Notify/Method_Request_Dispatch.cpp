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

TAO_NS_Method_Request_Dispatch::TAO_NS_Method_Request_Dispatch (const TAO_NS_Event_var& event, TAO_NS_ProxySupplier* proxy_supplier, CORBA::Boolean filtering)
  : TAO_NS_Method_Request_Dispatch_Base (event, proxy_supplier, filtering)
{
  this->init (event);
}

TAO_NS_Method_Request_Dispatch::~TAO_NS_Method_Request_Dispatch ()
{
}

int
TAO_NS_Method_Request_Dispatch::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/*********************************************************************************************************/

TAO_NS_Method_Request_Dispatch_No_Copy::TAO_NS_Method_Request_Dispatch_No_Copy (const TAO_NS_Event* event, TAO_NS_ProxySupplier* proxy_supplier, CORBA::Boolean filtering)
  : TAO_NS_Method_Request_Dispatch_No_Copy_Base (event, proxy_supplier, filtering)
{
}

TAO_NS_Method_Request_Dispatch_No_Copy:: ~TAO_NS_Method_Request_Dispatch_No_Copy ()
{
}

int
TAO_NS_Method_Request_Dispatch_No_Copy::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

TAO_NS_Method_Request*
TAO_NS_Method_Request_Dispatch_No_Copy::copy (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Method_Request* request;

  TAO_NS_Event* event_copy = this->event_->copy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_NS_Event_Copy_var event_var (event_copy);

  ACE_NEW_THROW_EX (request,
                    TAO_NS_Method_Request_Dispatch (event_var, this->proxy_supplier_, this->filtering_),
                    CORBA::INTERNAL ());

  return request;
}

/*********************************************************************************************************/

TAO_NS_Method_Request_Dispatch_No_Copy_Ex::TAO_NS_Method_Request_Dispatch_No_Copy_Ex (const TAO_NS_Event_var& event, TAO_NS_ProxySupplier* proxy_supplier, CORBA::Boolean filtering)
  : TAO_NS_Method_Request_Dispatch_No_Copy_Ex_Base (event, proxy_supplier, filtering)
{
}

TAO_NS_Method_Request_Dispatch_No_Copy_Ex:: ~TAO_NS_Method_Request_Dispatch_No_Copy_Ex ()
{
}

int
TAO_NS_Method_Request_Dispatch_No_Copy_Ex::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

TAO_NS_Method_Request*
TAO_NS_Method_Request_Dispatch_No_Copy_Ex::copy (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Method_Request* request;

  ACE_NEW_THROW_EX (request,
                    TAO_NS_Method_Request_Dispatch (this->event_, this->proxy_supplier_, this->filtering_),
                    CORBA::INTERNAL ());

  return request;
}
