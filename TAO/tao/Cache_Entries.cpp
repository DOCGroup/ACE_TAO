//$Id$

#include "tao/Cache_Entries.h"


#if !defined (__ACE_INLINE__)
# include "tao/Cache_Entries.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Cache_Entries, "$Id$")


void
TAO_Cache_IntId::operator= (const TAO_Cache_IntId &rhs)
{
  this->handler_ = rhs.handler_;
}

int
TAO_Cache_IntId::operator== (const TAO_Cache_IntId &rhs) const
{
  return (this->handler_ == rhs.handler_);
}

int
TAO_Cache_IntId::operator!= (const TAO_Cache_IntId &rhs) const
{
  return (this->handler_ != rhs.handler_);
}
