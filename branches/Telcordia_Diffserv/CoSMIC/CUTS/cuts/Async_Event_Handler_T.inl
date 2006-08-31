// $Id$

//
// spawn_i
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
int CUTS_Async_Event_Handler_T <COMPONENT, EVENTTYPE>::
spawn_i (size_t count)
{
  // We need to spawn <thread_count_> number of threads
  // with specified <priority_>.
  return this->thr_manager_.spawn_n (count,
                                     &THIS::event_loop,
                                     this,
                                     THR_NEW_LWP | THR_JOINABLE |
                                     THR_INHERIT_SCHED,
                                     this->priority_,
                                     GRP_ACTIVE);
}

//
// thread_count
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
size_t CUTS_Async_Event_Handler_T <COMPONENT, EVENTTYPE>::
thread_count (void) const
{
  return this->thread_map_.current_size ();
}


//
// priority
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_INLINE
int CUTS_Async_Event_Handler_T <COMPONENT, EVENTTYPE>::
priority (void) const
{
  return this->priority_;
}
