// $Id$

#include "tao/Connection_Purging_Strategy.h"

#if !defined (__ACE_INLINE__)
# include "tao/Connection_Purging_Strategy.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Connection_Purging_Strategy, "$Id$")

TAO_Connection_Purging_Strategy::~TAO_Connection_Purging_Strategy (void)
{
}


int
TAO_Connection_Purging_Strategy::cache_maximum (void) const
{
  return cache_maximum_;
}


