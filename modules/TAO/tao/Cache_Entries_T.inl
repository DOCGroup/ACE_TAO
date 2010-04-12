// -*- C++ -*-
//
// $Id$

#include "tao/debug.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template <typename TRANSPORT_TYPE> ACE_INLINE
  Cache_IntId_T<TRANSPORT_TYPE>::Cache_IntId_T (void)
    : transport_ (0),
      recycle_state_ (ENTRY_UNKNOWN),
      is_connected_ (false)
  {
  }

  template <typename TRANSPORT_TYPE> ACE_INLINE
  Cache_IntId_T<TRANSPORT_TYPE>::Cache_IntId_T (const Cache_IntId_T &rhs)
    : transport_ (0),
      recycle_state_ (ENTRY_UNKNOWN),
      is_connected_ (false)
  {
    *this = rhs;
  }

  template <typename TRANSPORT_TYPE> ACE_INLINE bool
  Cache_IntId_T<TRANSPORT_TYPE>::operator== (const Cache_IntId_T &rhs) const
  {
    return (this->transport_ == rhs.transport_);
  }

  template <typename TRANSPORT_TYPE> ACE_INLINE bool
  Cache_IntId_T<TRANSPORT_TYPE>::operator!= (const Cache_IntId_T &rhs) const
  {
    return (this->transport_ != rhs.transport_);
  }

  template <typename TRANSPORT_TYPE> ACE_INLINE
  typename Cache_IntId_T<TRANSPORT_TYPE>::transport_type *
  Cache_IntId_T<TRANSPORT_TYPE>::transport (void)
  {
    return this->transport_;
  }

  template <typename TRANSPORT_TYPE> ACE_INLINE
  const typename Cache_IntId_T<TRANSPORT_TYPE>::transport_type *
  Cache_IntId_T<TRANSPORT_TYPE>::transport (void) const
  {
    return this->transport_;
  }

  template <typename TRANSPORT_TYPE> ACE_INLINE
  bool
  Cache_IntId_T<TRANSPORT_TYPE>::is_connected (void) const
  {
    return this->is_connected_;
  }

  template <typename TRANSPORT_TYPE> ACE_INLINE
  void
  Cache_IntId_T<TRANSPORT_TYPE>::is_connected (bool connected)
  {
    this->is_connected_ = connected;
  }

  template <typename TRANSPORT_TYPE> ACE_INLINE Cache_Entries_State
  Cache_IntId_T<TRANSPORT_TYPE>::recycle_state (void) const
  {
    return this->recycle_state_;
  }

  template <typename TRANSPORT_TYPE> ACE_INLINE
  typename Cache_IntId_T<TRANSPORT_TYPE>::transport_type *
  Cache_IntId_T<TRANSPORT_TYPE>::relinquish_transport (void)
  {
    // Yield ownership of the TAO_Transport object.
    transport_type *val = this->transport_;
    this->transport_ = 0;
    return val;
  }

  template <typename TRANSPORT_TYPE> ACE_INLINE const char *
  Cache_IntId_T<TRANSPORT_TYPE>::state_name (Cache_Entries_State st)
  {
#define TAO_CACHE_INTID_ENTRY(X) case X: return #X
    switch (st)
      {
        TAO_CACHE_INTID_ENTRY (ENTRY_IDLE_AND_PURGABLE);
        TAO_CACHE_INTID_ENTRY (ENTRY_PURGABLE_BUT_NOT_IDLE);
        TAO_CACHE_INTID_ENTRY (ENTRY_BUSY);
        TAO_CACHE_INTID_ENTRY (ENTRY_CLOSED);
        TAO_CACHE_INTID_ENTRY (ENTRY_CONNECTING);
        TAO_CACHE_INTID_ENTRY (ENTRY_UNKNOWN);
      }
    return "***Unknown enum value, update Cache_IntId_T::state_name()";
#undef TAO_CACHE_INTID_ENTRY
  }

  /*******************************************************/
  template <typename TRANSPORT_DESCRIPTOR_TYPE> ACE_INLINE
  Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::Cache_ExtId_T (void)
    : transport_property_ (0),
      is_delete_ (false),
      index_ (0)
  {
  }

  template <typename TRANSPORT_DESCRIPTOR_TYPE> ACE_INLINE
  Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::Cache_ExtId_T (
    typename Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::transport_descriptor_type *prop)
    : transport_property_ (prop),
      is_delete_ (false),
      index_ (0)
  {

  }

  template <typename TRANSPORT_DESCRIPTOR_TYPE> ACE_INLINE
  Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::~Cache_ExtId_T (void)
  {
    if (this->is_delete_)
      delete this->transport_property_;
  }

  template <typename TRANSPORT_DESCRIPTOR_TYPE> ACE_INLINE
  Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE> &
  Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::operator= (const Cache_ExtId_T &rhs)
  {
    if (this != &rhs)
      {
        // Do a deep copy
        this->transport_property_ =
          rhs.transport_property_->duplicate ();

        if (this->transport_property_ == 0)
          {
            this->is_delete_ = false;
            this->index_ = 0;
          }
        else
          {
            this->is_delete_ = true;
            this->index_ = rhs.index_;
          }
      }
    return *this;
  }

  template <typename TRANSPORT_DESCRIPTOR_TYPE> ACE_INLINE
  Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::Cache_ExtId_T (const Cache_ExtId_T &rhs)
    : transport_property_ (0),
      is_delete_ (false),
      index_ (0)
  {
    *this = rhs;
  }

  template <typename TRANSPORT_DESCRIPTOR_TYPE> ACE_INLINE bool
  Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::operator== (const Cache_ExtId_T &rhs) const
  {
    return (this->transport_property_->is_equivalent (rhs.transport_property_) &&
            this->index_ == rhs.index_);
  }

  template <typename TRANSPORT_DESCRIPTOR_TYPE> ACE_INLINE bool
  Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::operator!= (const Cache_ExtId_T &rhs) const
  {
    if (this->transport_property_->is_equivalent (rhs.transport_property_) &&
        this->index_ == rhs.index_)
      return false;

    return true;
  }

  template <typename TRANSPORT_DESCRIPTOR_TYPE> ACE_INLINE u_long
  Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::hash (void) const
  {
    return (this->transport_property_->hash () + this->index_);
  }

  template <typename TRANSPORT_DESCRIPTOR_TYPE> ACE_INLINE void
  Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::duplicate (void)
  {
    transport_descriptor_type *prop = 0;

    // Make a deep copy
    prop = this->transport_property_->duplicate ();

    if (prop == 0)
      return;

    // Release memory if there was some allocated in the first place
    if (this->is_delete_)
      delete this->transport_property_;

    this->is_delete_ = true;
    this->transport_property_ = prop;
  }


  template <typename TRANSPORT_DESCRIPTOR_TYPE> ACE_INLINE CORBA::ULong
  Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::index (void) const
  {
    return this->index_;
  }


  template <typename TRANSPORT_DESCRIPTOR_TYPE> ACE_INLINE void
  Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::index (CORBA::ULong index)
  {
    this->index_ = index;
  }

  template <typename TRANSPORT_DESCRIPTOR_TYPE> ACE_INLINE void
  Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::incr_index (void)
  {
    ++this->index_;
  }

  template <typename TRANSPORT_DESCRIPTOR_TYPE> ACE_INLINE
  typename Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::transport_descriptor_type *
  Cache_ExtId_T<TRANSPORT_DESCRIPTOR_TYPE>::property (void) const
  {
    return this->transport_property_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
