/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE
//
// = FILENAME
//    Thread_Priority.i
//
// = CREATION DATE
//    28 January 1997
//
// = AUTHOR
//    David Levine
//
// ============================================================================

ACE_INLINE
ACE_Thread_Priority::ACE_Thread_Priority (Priority_Class priority_class,
					  Thread_Priority default_thread_priority) 
  : priority_class_ (priority_class),
    default_thread_priority_ (default_thread_priority)
{
  // normalize () sets os_priority_class_ and os_default_thread_priority_
  (void) normalize ();
}

ACE_INLINE
ACE_Thread_Priority::~ACE_Thread_Priority (void)
{
}

ACE_INLINE long
ACE_Thread_Priority::priority_class (const Priority_Class priority_class)
{
  this->priority_class_ = priority_class;
  return this->normalize ();
}

ACE_INLINE long ACE_Thread_Priority::default_thread_priority (const Thread_Priority default_thread_priority)
{
  this->default_thread_priority_ = default_thread_priority;
  return this->normalize ();
}

ACE_INLINE ACE_Thread_Priority::Priority_Class
ACE_Thread_Priority::priority_class (void) const
{
  return this->priority_class_;
}

ACE_INLINE ACE_Thread_Priority::Thread_Priority
ACE_Thread_Priority::default_thread_priority (void) const
{
  return this->default_thread_priority_;
}

ACE_INLINE ACE_id_t
ACE_Thread_Priority::os_priority_class (void) const
{
  return this->os_priority_class_;
}

ACE_INLINE ACE_pri_t
ACE_Thread_Priority::os_default_thread_priority (void) const
{
  return this->os_default_thread_priority_;
}
