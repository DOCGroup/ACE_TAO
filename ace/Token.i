/* -*- C++ -*- */
// $Id$

// Token.i

ACE_INLINE int
ACE_Token::remove (void)
{
  ACE_TRACE ("ACE_Token::remove");
  // Don't have an implementation for this yet...
  ACE_NOTSUP_RETURN (-1);
}

ACE_INLINE int 
ACE_Token::tryacquire (void)
{
  ACE_TRACE ("ACE_Token::tryacquire");
  return this->shared_acquire 
    (0, 0, (ACE_Time_Value *) &ACE_Time_Value::zero);
}

ACE_INLINE int 
ACE_Token::waiters (void)
{
  ACE_TRACE ("ACE_Token::waiters");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

  int ret = this->waiters_;
  return ret;
}

ACE_INLINE ACE_thread_t 
ACE_Token::current_owner (void)
{
  ACE_TRACE ("ACE_Token::current_owner");
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, this->owner_);

  return this->owner_;
}

