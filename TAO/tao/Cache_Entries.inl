// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ACE_INLINE
  Cache_IntId::Cache_IntId (void)
    : transport_ (0),
      recycle_state_ (ENTRY_UNKNOWN),
      is_connected_ (false)
  {
  }

  ACE_INLINE
  Cache_IntId::Cache_IntId (const Cache_IntId &rhs)
    : transport_ (0),
      recycle_state_ (ENTRY_UNKNOWN),
      is_connected_ (false)
  {
    *this = rhs;
  }

  ACE_INLINE bool
  Cache_IntId::operator== (const Cache_IntId &rhs) const
  {
    return (this->transport_ == rhs.transport_);
  }

  ACE_INLINE bool
  Cache_IntId::operator!= (const Cache_IntId &rhs) const
  {
    return (this->transport_ != rhs.transport_);
  }

  ACE_INLINE TAO_Transport *
  Cache_IntId::transport (void)
  {
    return this->transport_;
  }

  ACE_INLINE const TAO_Transport *
  Cache_IntId::transport (void) const
  {
    return this->transport_;
  }

  ACE_INLINE void
  Cache_IntId::recycle_state (Cache_Entries_State st)
  {
    this->recycle_state_ = st;
  }

  ACE_INLINE Cache_Entries_State
  Cache_IntId::recycle_state (void) const
  {
    return this->recycle_state_;
  }

  ACE_INLINE TAO_Transport *
  Cache_IntId::relinquish_transport (void)
  {
    // Yield ownership of the TAO_Transport object.
    TAO_Transport *val = this->transport_;
    this->transport_ = 0;
    return val;
  }

  /*******************************************************/
  ACE_INLINE
  Cache_ExtId::Cache_ExtId (void)
    : transport_property_ (0),
      is_delete_ (false),
      index_ (0)
  {
  }

  ACE_INLINE
  Cache_ExtId::Cache_ExtId (TAO_Transport_Descriptor_Interface *prop)
    : transport_property_ (prop),
      is_delete_ (false),
      index_ (0)
  {

  }

  ACE_INLINE
  Cache_ExtId::~Cache_ExtId (void)
  {
    if (this->is_delete_)
      delete this->transport_property_;
  }

  ACE_INLINE Cache_ExtId &
  Cache_ExtId::operator= (const Cache_ExtId &rhs)
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

  ACE_INLINE
  Cache_ExtId::Cache_ExtId (const Cache_ExtId &rhs)
    : transport_property_ (0),
      is_delete_ (false),
      index_ (0)
  {
    *this = rhs;
  }

  ACE_INLINE bool
  Cache_ExtId::operator== (const Cache_ExtId &rhs) const
  {
    return (this->transport_property_->is_equivalent (rhs.transport_property_) &&
            this->index_ == rhs.index_);
  }

  ACE_INLINE bool
  Cache_ExtId::operator!= (const Cache_ExtId &rhs) const
  {
    if (this->transport_property_->is_equivalent (rhs.transport_property_) &&
        this->index_ == rhs.index_)
      return false;

    return true;
  }

  ACE_INLINE u_long
  Cache_ExtId::hash (void) const
  {
    return (this->transport_property_->hash () + this->index_);
  }

  ACE_INLINE void
  Cache_ExtId::duplicate (void)
  {
    TAO_Transport_Descriptor_Interface *prop = 0;

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


  ACE_INLINE CORBA::ULong
  Cache_ExtId::index (void) const
  {
    return this->index_;
  }


  ACE_INLINE void
  Cache_ExtId::index (CORBA::ULong index)
  {
    this->index_ = index;
  }

  ACE_INLINE void
  Cache_ExtId::incr_index (void)
  {
    ++this->index_;
  }

  ACE_INLINE TAO_Transport_Descriptor_Interface *
  Cache_ExtId::property (void) const
  {
    return this->transport_property_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
