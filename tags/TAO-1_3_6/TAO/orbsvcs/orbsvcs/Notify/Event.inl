// $Id$

ACE_INLINE const TAO_Notify_Property_Short&
TAO_Notify_Event::priority (void) const
{
  return this->priority_;
}

ACE_INLINE const TAO_Notify_Property_Time&
TAO_Notify_Event::timeout (void) const
{
  return this->timeout_;
}

/**********************************************************/

ACE_INLINE
TAO_Notify_Event_var::TAO_Notify_Event_var (void)
  : TAO_Notify_Event_var_Base ()
{
}

ACE_INLINE
TAO_Notify_Event_var::TAO_Notify_Event_var (TAO_Notify_Event* event)
  : TAO_Notify_Event_var_Base (event)
{
}

/**********************************************************/

ACE_INLINE
TAO_Notify_Event_Copy_var::TAO_Notify_Event_Copy_var (void)
  : TAO_Notify_Event_var ()
{
}

ACE_INLINE
TAO_Notify_Event_Copy_var::TAO_Notify_Event_Copy_var (TAO_Notify_Event* event)
  : TAO_Notify_Event_var (event)
{
}
