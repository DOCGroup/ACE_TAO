#include "ace/Hashable.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

unsigned long
ACE_Hashable::hash () const
{
  // In doing the check below, we take chance of paying a performance
  // price when the hash value is zero.  But, that will (hopefully)
  // happen far less often than a non-zero value, so this caching
  // strategy should pay off, esp. if hash computation is expensive
  // relative to the simple comparison.

  if (this->hash_value_ == 0)
    this->hash_value_ = this->hash_i ();

  return this->hash_value_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
