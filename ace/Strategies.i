/* -*- C++ -*- */
// $Id$

// Strategies.i

ACE_INLINE
ACE_Reactor_Notification_Strategy::~ACE_Reactor_Notification_Strategy (void)
{
}

ACE_INLINE
ACE_Connection_Recycling_Strategy::ACE_Connection_Recycling_Strategy (void)
{
}

ACE_INLINE
ACE_Recyclable::ACE_Recyclable (ACE_Recyclable::State initial_state)
  : state_ (initial_state)
{
}

ACE_INLINE
ACE_Recyclable::~ACE_Recyclable (void)
{
}

ACE_INLINE ACE_Recyclable::State 
ACE_Recyclable::state (void) const
{
  return this->state_;
}

ACE_INLINE void 
ACE_Recyclable::state (ACE_Recyclable::State new_state)
{
  this->state_ = new_state;
}

ACE_INLINE 
ACE_Hashable::ACE_Hashable (void)
  : hash_value_ (0)
{
}

ACE_INLINE 
ACE_Hashable::~ACE_Hashable (void)
{
}

ACE_INLINE u_long
ACE_Hashable::hash (void) const
{
  // In doing the check below, we take chance of paying a performance
  // price when the hash value is zero.  But, that will (hopefully)
  // happen far less often than a non-zero value, so this caching
  // strategy should pay off, esp. if hash computation is expensive
  // relative to the simple comparison.

  if (this->hash_value_ == 0)
    ((ACE_Hashable *) this)->hash_value_ = this->hash_i ();

  return this->hash_value_;
}

ACE_INLINE 
ACE_Refcountable::ACE_Refcountable (int refcount)
  : refcount_ (refcount)
{
}

ACE_INLINE 
ACE_Refcountable::~ACE_Refcountable (void)
{
}

ACE_INLINE int
ACE_Refcountable::increment (void)
{
  return ++this->refcount_;
}

ACE_INLINE int 
ACE_Refcountable::decrement (void)
{
  return --this->refcount_;
}

ACE_INLINE int 
ACE_Refcountable::refcount (void) const
{
  return this->refcount_;
}

