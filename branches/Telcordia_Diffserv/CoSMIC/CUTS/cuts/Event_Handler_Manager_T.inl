// $Id$

//
// activate
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
void CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::activate (void)
{
  this->impl_->activate ();
}

//
// deactivate
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
void
CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::
deactivate (void)
{
  this->impl_->deactivate ();
}

//
// handle_event
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
void
CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::
handle_event (EVENTTYPE * ev)
{
  this->impl_->handle_event (ev);
}

//
// port_agent
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
CUTS_Port_Agent &
CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::
port_agent (void)
{
  return this->config_.port_agent_;
}

//
// bind
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
void
CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::
bind (Component_Type * component,
      Event_Method method)
{
  this->config_.component_ = component;
  this->config_.method_ = method;
}

//
// unbind
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
void
CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::
unbind (void)
{
  this->config_.component_ = 0;
  this->config_.method_ = 0;
}

//
// mode
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
CUTS_Event_Handler::Event_Mode
CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::
mode (void) const
{
  return this->mode_;
}

//
// priority
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
int
CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::
priority (void) const
{
  return this->impl_->priority ();
}

//
// priority
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
void
CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::
priority (int prio)
{
  this->impl_->priority (prio);
}

//
// thread_count
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
size_t
CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::
thread_count (void) const
{
  return this->impl_->thread_count ();
}

//
// thread_count
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
void
CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::
thread_count (size_t count)
{
  this->impl_->thread_count (count);
}

//
// name
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
const char *
CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::name (void) const
{
  return this->config_.port_agent_.name ();
}

//
// name
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
void
CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::
name (const char * name)
{
  return this->config_.port_agent_.name (name);
}
