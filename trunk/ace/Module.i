/* -*- C++ -*- */
// $Id$

// Module.i

template <ACE_SYNCH_1> ACE_INLINE void *
ACE_Module<ACE_SYNCH_2>::arg (void) const
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::arg");
  return this->arg_;
}

template <ACE_SYNCH_1> ACE_INLINE void
ACE_Module<ACE_SYNCH_2>::arg (void *a)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::arg");
  this->arg_ = a;
}

template <ACE_SYNCH_1> ACE_INLINE const char *
ACE_Module<ACE_SYNCH_2>::name (void) const
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::name");
  return this->name_;
}

template <ACE_SYNCH_1> ACE_INLINE void
ACE_Module<ACE_SYNCH_2>::name (const char *n)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::name");
  ACE_OS::strncpy (this->name_, n, MAXNAMLEN);
}

template <ACE_SYNCH_1> ACE_INLINE ACE_Task<ACE_SYNCH_2> *
ACE_Module<ACE_SYNCH_2>::writer (void)
{ 
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::writer");
  return this->q_pair_[1];
}

template <ACE_SYNCH_1> ACE_INLINE ACE_Task<ACE_SYNCH_2> *
ACE_Module<ACE_SYNCH_2>::reader (void)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::reader");
  return this->q_pair_[0];
}

template <ACE_SYNCH_1> ACE_INLINE ACE_Module<ACE_SYNCH_2> *
ACE_Module<ACE_SYNCH_2>::next (void)
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::next");
  return this->next_;
}

template <ACE_SYNCH_1> ACE_INLINE void
ACE_Module<ACE_SYNCH_2>::next (ACE_Module<ACE_SYNCH_2> *m) 
{
  ACE_TRACE ("ACE_Module<ACE_SYNCH_2>::next");
  this->next_ = m;
}


