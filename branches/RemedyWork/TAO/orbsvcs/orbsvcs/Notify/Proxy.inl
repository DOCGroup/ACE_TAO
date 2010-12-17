// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::Boolean
TAO_Notify_Proxy::updates_off (void)
{
  return this->updates_off_;
}

ACE_INLINE CORBA::Boolean
TAO_Notify_Proxy::check_filters (const TAO_Notify_Event* event
                             , TAO_Notify_FilterAdmin& parent_filter_admin
                             , CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator)
{
  // check if it passes the parent filter.
  CORBA::Boolean const parent_val =
    parent_filter_admin.match (event);

  CORBA::Boolean val = false;

  if (filter_operator == CosNotifyChannelAdmin::AND_OP)
    {
      val = parent_val && this->filter_admin_.match (event);
    }
  else
    {
      val = parent_val || this->filter_admin_.match (event);
    }

  return val;
}

TAO_END_VERSIONED_NAMESPACE_DECL
