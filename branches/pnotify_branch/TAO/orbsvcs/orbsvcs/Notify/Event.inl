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

ACE_INLINE const TAO_Notify_Property_Boolean&
TAO_Notify_Event::reliable (void) const
{
  return this->reliable_;
}

/**********************************************************/

ACE_INLINE
TAO_Notify_Event_var::TAO_Notify_Event_var (void)
  : TAO_Notify_Event_var_Base ()
{
}

ACE_INLINE
TAO_Notify_Event_var::TAO_Notify_Event_var (const TAO_Notify_Event* event)
  : TAO_Notify_Event_var_Base (const_cast <TAO_Notify_Event *>(event))
{
}

/**********************************************************/

ACE_INLINE
TAO_Notify_Event_Copy_var::TAO_Notify_Event_Copy_var (void)
  : TAO_Notify_Event_var ()
{
}

ACE_INLINE
TAO_Notify_Event_Copy_var::TAO_Notify_Event_Copy_var (const TAO_Notify_Event* event)
  : TAO_Notify_Event_var (event)
{
}

ACE_INLINE
const TAO_Notify_Event *
TAO_Notify_Event::copy_on_heap (ACE_ENV_SINGLE_ARG_DECL) const
{
  if (this->event_on_heap_ == 0)
  {
    TAO_Notify_Event * copied =
      this->copy (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
    const_cast <TAO_Notify_Event *> (this)->event_on_heap_ = copied;
    copied->event_on_heap_ = copied;
  }
  return this->event_on_heap_;
}

