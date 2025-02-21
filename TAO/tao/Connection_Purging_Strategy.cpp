#include "tao/Connection_Purging_Strategy.h"

#if !defined (__ACE_INLINE__)
# include "tao/Connection_Purging_Strategy.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Connection_Purging_Strategy::~TAO_Connection_Purging_Strategy ()
{
}

int
TAO_Connection_Purging_Strategy::cache_maximum () const
{
  return cache_maximum_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
