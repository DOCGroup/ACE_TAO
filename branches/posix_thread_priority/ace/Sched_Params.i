/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE
//
// = FILENAME
//    Scheduling_Params.i
//
// = CREATION DATE
//    28 January 1997
//
// = AUTHOR
//    David Levine
//
// ============================================================================

ACE_INLINE
ACE_Scheduling_Params::ACE_Scheduling_Params (const ACE_Thread_Priority &priority,
                                              const int scope,
                                              const ACE_Time_Value &quantum)
  : priority_ (priority),
    scope_ (scope),
    quantum_ (quantum)
{
}

ACE_INLINE ACE_Scheduling_Params::~ACE_Scheduling_Params (void)
{
}

ACE_INLINE const ACE_Thread_Priority &
ACE_Scheduling_Params::priority (void) const
{
  return this->priority_;
}

ACE_INLINE int
ACE_Scheduling_Params::scope (void) const
{
  return this->scope_;
}

ACE_INLINE void
ACE_Scheduling_Params::set_scope (const int scope)
{
  this->scope_ = scope;
}

ACE_INLINE const ACE_Time_Value &
ACE_Scheduling_Params::quantum (void) const
{
  return this->quantum_;
}

ACE_INLINE void
ACE_Scheduling_Params::set_quantum (const ACE_Time_Value &quantum)
{
  this->quantum_ = quantum;
}


// EOF
