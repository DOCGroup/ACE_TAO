/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE
//
// = FILENAME
//    Sched_Params.i
//
// = CREATION DATE
//    28 January 1997
//
// = AUTHOR
//    David Levine
//
// ============================================================================

ACE_INLINE
ACE_Sched_Params::ACE_Sched_Params (
      const Policy policy,
      const ACE_Sched_Priority priority,
      const int scope,
      const ACE_Time_Value &quantum)
  : policy_ (policy),
    priority_ (priority),
    scope_ (scope),
    quantum_ (quantum)
{
}

ACE_INLINE ACE_Sched_Params::~ACE_Sched_Params (void)
{
}

ACE_INLINE ACE_Sched_Params::Policy
ACE_Sched_Params::policy (void) const
{
  return this->policy_;
}

ACE_INLINE void
ACE_Sched_Params::policy (const ACE_Sched_Params::Policy policy)
{
  this->policy_ = policy;
}

ACE_INLINE ACE_Sched_Priority
ACE_Sched_Params::priority (void) const
{
  return this->priority_;
}

ACE_INLINE void
ACE_Sched_Params::priority (const ACE_Sched_Priority priority)
{
  this->priority_ = priority;
}

ACE_INLINE int
ACE_Sched_Params::scope (void) const
{
  return this->scope_;
}

ACE_INLINE void
ACE_Sched_Params::scope (const int scope)
{
  this->scope_ = scope;
}

ACE_INLINE const ACE_Time_Value &
ACE_Sched_Params::quantum (void) const
{
  return this->quantum_;
}

ACE_INLINE void
ACE_Sched_Params::quantum (const ACE_Time_Value &quant)
{
  this->quantum_ = quant;
}


// EOF
