//$Id$

#include "tao/Cache_Entries.h"
#include "tao/Transport.h"


#if !defined (__ACE_INLINE__)
# include "tao/Cache_Entries.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Cache_Entries, "$Id$")

TAO_Cache_IntId::TAO_Cache_IntId (TAO_Transport *transport)
  : transport_ (TAO_Transport::_duplicate (transport)),
    recycle_state_ (ACE_RECYCLABLE_UNKNOWN)
{
}

TAO_Cache_IntId::~TAO_Cache_IntId (void)
{
  TAO_Transport::release (this->transport_);
}

TAO_Cache_IntId&
TAO_Cache_IntId::operator= (const TAO_Cache_IntId &rhs)
{
  if (this != &rhs) {
    this->recycle_state_ = rhs.recycle_state_;

    TAO_Transport* old_transport = this->transport_;
    this->transport_ = TAO_Transport::_duplicate (rhs.transport_);
    TAO_Transport::release (old_transport);
  }
  return *this;
}

