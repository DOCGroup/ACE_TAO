// $Id$

#include "Proxy.h"
#include "Admin.h"

#if ! defined (__ACE_INLINE__)
#include "Proxy.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Proxy, "$Id$")

TAO_NS_Proxy::TAO_NS_Proxy (void)
{
  // Set initial proxy mode to broadcast.
  this->subscribed_types_.insert (TAO_NS_EventType::special ());
}

TAO_NS_Proxy::~TAO_NS_Proxy ()
{
}

CORBA::Boolean
TAO_NS_Proxy::check_filters (TAO_NS_Event_var &event ACE_ENV_ARG_DECL)
{
  // check if it passes the parent filter.
  CORBA::Boolean parent_val =
    this->parent_->filter_admin ().match (event ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Boolean val = 0;

  if (this->parent_->filter_operator () == CosNotifyChannelAdmin::AND_OP)
    {
      val = parent_val && this->filter_admin_.match (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      val = parent_val || this->filter_admin_.match (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  return val;
}
