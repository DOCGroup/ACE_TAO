// $Id$

ACE_INLINE const TAO_NS_Property_Short&
TAO_NS_Event::priority (void) const
{
  return this->priority_;
}

ACE_INLINE const TAO_NS_Property_Time&
TAO_NS_Event::timeout (void) const
{
  return this->timeout_;
}

/**********************************************************/

ACE_INLINE
TAO_NS_Event_var::TAO_NS_Event_var (void)
  : TAO_NS_Event_var_Base ()
{
}

ACE_INLINE
TAO_NS_Event_var::TAO_NS_Event_var (TAO_NS_Event* event)
  : TAO_NS_Event_var_Base (event)
{
}

/**********************************************************/

ACE_INLINE
TAO_NS_Event_Copy_var::TAO_NS_Event_Copy_var (void)
  : TAO_NS_Event_var ()
{
}

ACE_INLINE
TAO_NS_Event_Copy_var::TAO_NS_Event_Copy_var (TAO_NS_Event* event)
  : TAO_NS_Event_var (event)
{
}
