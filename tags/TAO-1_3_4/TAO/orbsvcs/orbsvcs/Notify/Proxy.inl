// $Id$

ACE_INLINE CORBA::Boolean
TAO_Notify_Proxy::updates_off (void)
{
  return this->updates_off_;
}

ACE_INLINE CORBA::Boolean
TAO_Notify_Proxy::check_filters (const TAO_Notify_Event_var &event
                             , TAO_Notify_FilterAdmin& parent_filter_admin
                             , CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator
                             ACE_ENV_ARG_DECL)
{
  return this->check_filters (event.get (), parent_filter_admin, filter_operator ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE CORBA::Boolean
TAO_Notify_Proxy::check_filters (const TAO_Notify_Event* event
                             , TAO_Notify_FilterAdmin& parent_filter_admin
                             , CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator
                             ACE_ENV_ARG_DECL)
{
  // check if it passes the parent filter.
  CORBA::Boolean parent_val =
    parent_filter_admin.match (event ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Boolean val = 0;

  if (filter_operator == CosNotifyChannelAdmin::AND_OP)
    {
      val = parent_val && this->filter_admin_.match (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  else
    {
      val = parent_val || this->filter_admin_.match (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return val;
}
