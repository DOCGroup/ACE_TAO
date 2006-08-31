// $Id$

//
// handle_event
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
void CUTS_Sync_Event_Handler_T <COMPONENT, EVENTTYPE>::
handle_event (EVENTTYPE * ev)
{
  this->handle_event_i (ev, ACE_Time_Value::zero);
}

//
// thread_count
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
void CUTS_Sync_Event_Handler_T <COMPONENT, EVENTTYPE>::
thread_count (size_t count)
{
  ACE_UNUSED_ARG (count);
}

//
// thread_count
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
size_t CUTS_Sync_Event_Handler_T <COMPONENT, EVENTTYPE>::
thread_count (void) const
{
  return 0;
}

//
// thread_count
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
void CUTS_Sync_Event_Handler_T <COMPONENT, EVENTTYPE>::
priority (int prio)
{
  this->priority_ = prio;
}

//
// thread_count
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
int CUTS_Sync_Event_Handler_T <COMPONENT, EVENTTYPE>::
priority (void) const
{
  return this->priority_;
}
