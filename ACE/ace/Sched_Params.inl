/**
 *  @file    Sched_Params.inl
 *
 *  @author David Levine
 */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
ACE_Sched_Params::ACE_Sched_Params (
      Policy const policy,
      ACE_Sched_Priority const priority,
      const int scope,
      ACE_Time_Value const &quantum)
  : policy_ (policy),
    priority_ (priority),
    scope_ (scope),
    quantum_ (quantum)
{
}

ACE_INLINE ACE_Sched_Params::~ACE_Sched_Params () = default;

ACE_INLINE ACE_Sched_Params::Policy
ACE_Sched_Params::policy () const
{
  return this->policy_;
}

ACE_INLINE void
ACE_Sched_Params::policy (ACE_Sched_Params::Policy const policy)
{
  this->policy_ = policy;
}

ACE_INLINE ACE_Sched_Priority
ACE_Sched_Params::priority () const
{
  return this->priority_;
}

ACE_INLINE void
ACE_Sched_Params::priority (ACE_Sched_Priority const priority)
{
  this->priority_ = priority;
}

ACE_INLINE int
ACE_Sched_Params::scope () const
{
  return this->scope_;
}

ACE_INLINE void
ACE_Sched_Params::scope (const int scope)
{
  this->scope_ = scope;
}

ACE_INLINE ACE_Time_Value const &
ACE_Sched_Params::quantum () const
{
  return this->quantum_;
}

ACE_INLINE void
ACE_Sched_Params::quantum (ACE_Time_Value const &quant)
{
  this->quantum_ = quant;
}

ACE_INLINE ACE_Sched_Params::Policy const &
ACE_Sched_Priority_Iterator::policy () const
{
  return this->policy_;
}

ACE_INLINE int
ACE_Sched_Priority_Iterator::scope () const
{
  return this->scope_;
}

ACE_INLINE
ACE_Sched_Priority_Iterator::ACE_Sched_Priority_Iterator (ACE_Sched_Params::Policy const &policy,
                                                          int scope)
 : policy_ (policy),
   scope_ (scope),
   priority_ (0),
   done_ (0)
{
  priority_ = ACE_Sched_Params::priority_min (this->policy (), this->scope ());
}

ACE_INLINE
ACE_Sched_Priority_Iterator::~ACE_Sched_Priority_Iterator () = default;

ACE_INLINE int
ACE_Sched_Priority_Iterator::more () const
{
  return !this->done_;
}

ACE_INLINE int
ACE_Sched_Priority_Iterator::priority () const
{
  return this->priority_;
}

ACE_INLINE void
ACE_Sched_Priority_Iterator::next ()
{
  if (this->done_)
    {
      return;
    }

  int old_priority = this->priority_;
  priority_ = ACE_Sched_Params::next_priority (this->policy (),
                                               this->priority (),
                                               this->scope ());
  this->done_ = old_priority == priority_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
