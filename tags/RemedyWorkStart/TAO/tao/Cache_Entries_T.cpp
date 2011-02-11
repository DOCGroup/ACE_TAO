// -*- C++ -*-
// $Id$

# include "tao/Cache_Entries_T.h"

#if !defined (__ACE_INLINE__)
# include "tao/Cache_Entries_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template <typename TRANSPORT_TYPE>
  Cache_IntId_T<TRANSPORT_TYPE>::Cache_IntId_T (TRANSPORT_TYPE *transport)
    : transport_ (transport)
    , recycle_state_ (ENTRY_UNKNOWN)
    , is_connected_ (false)
  {
    this->is_connected_ = transport->is_connected();
    transport->add_reference ();
    if (TAO_debug_level > 9)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Cache_IntId_T::Cache_IntId_T, ")
                  ACE_TEXT ("this=%@ Transport[%d] is%Cconnected\n"),
                  this, transport->id (), (is_connected_ ? " " : " not ")));
  }

  template <typename TRANSPORT_TYPE>
  Cache_IntId_T<TRANSPORT_TYPE>::~Cache_IntId_T (void)
  {
    if (this->transport_)
      this->transport_->remove_reference ();
  }

  template <typename TRANSPORT_TYPE>
  Cache_IntId_T<TRANSPORT_TYPE>&
  Cache_IntId_T<TRANSPORT_TYPE>::operator= (
    const Cache_IntId_T<TRANSPORT_TYPE> &rhs)
  {
    if (this != &rhs)
      {
        this->recycle_state_ = rhs.recycle_state_;
        this->is_connected_ = rhs.is_connected_;
        transport_type *old_transport = this->transport_;
        this->transport_ = rhs.transport_;
        if (this->transport_)
          this->transport_->add_reference ();
        if (old_transport)
          old_transport->remove_reference ();
      }

    return *this;
  }

  template <typename TRANSPORT_TYPE>
  void
  Cache_IntId_T<TRANSPORT_TYPE>::recycle_state (Cache_Entries_State st)
  {
    if (TAO_debug_level > 9)
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Cache_IntId_T::")
                  ACE_TEXT ("recycle_state, %C->%C Transport[%d] IntId=%@\n"),
                  state_name (recycle_state_), state_name (st),
                  transport_ ? transport_->id() : 0, this));
    this->recycle_state_ = st;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
