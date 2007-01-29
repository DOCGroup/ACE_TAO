//$Id$

#include "tao/Cache_Entries.h"
#include "tao/Transport.h"


#if !defined (__ACE_INLINE__)
# include "tao/Cache_Entries.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao,
          Cache_Entries,
          "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Cache_IntId::Cache_IntId (TAO_Transport *transport)
    : transport_ (transport)
    , recycle_state_ (ENTRY_UNKNOWN)
  {
    transport->add_reference ();
  }

  Cache_IntId::~Cache_IntId (void)
  {
    if (this->transport_)
      this->transport_->remove_reference ();
  }

  Cache_IntId&
  Cache_IntId::operator= (const Cache_IntId &rhs)
  {
    if (this != &rhs)
      {
        this->recycle_state_ = rhs.recycle_state_;

        TAO_Transport *old_transport = this->transport_;
        this->transport_ = rhs.transport_;
        if (this->transport_)
          this->transport_->add_reference ();
        if (old_transport)
          old_transport->remove_reference ();
      }

    return *this;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
