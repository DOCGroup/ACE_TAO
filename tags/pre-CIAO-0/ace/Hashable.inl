/* -*- C++ -*- */
// $Id$
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
