/* -*- C++ -*- */
// $Id$

ACE_INLINE
TAO_Cache_IntId::TAO_Cache_IntId (void)
  : transport_ (0),
    recycle_state_ (ACE_RECYCLABLE_UNKNOWN)
{
}

ACE_INLINE
TAO_Cache_IntId::TAO_Cache_IntId (const TAO_Cache_IntId &rhs)
  : transport_ (0),
    recycle_state_ (ACE_RECYCLABLE_UNKNOWN)
{
  *this = rhs;
}

ACE_INLINE int
TAO_Cache_IntId::operator== (const TAO_Cache_IntId &rhs) const
{
  return (this->transport_ == rhs.transport_);
}

ACE_INLINE int
TAO_Cache_IntId::operator!= (const TAO_Cache_IntId &rhs) const
{
  return (this->transport_ != rhs.transport_);
}

ACE_INLINE TAO_Transport *
TAO_Cache_IntId::transport (void)
{
  return this->transport_;
}

ACE_INLINE const TAO_Transport *
TAO_Cache_IntId::transport (void) const
{
  return this->transport_;
}

ACE_INLINE void
TAO_Cache_IntId::recycle_state (ACE_Recyclable_State st)
{
  this->recycle_state_ = st;
}

ACE_INLINE ACE_Recyclable_State
TAO_Cache_IntId::recycle_state (void)
{
  return this->recycle_state_;
}

ACE_INLINE TAO_Transport *
TAO_Cache_IntId::relinquish_transport (void)
{
  // Yield ownership of the TAO_Transport object.
  TAO_Transport *val = this->transport_;
  this->transport_ = 0;
  return val;
}

/*******************************************************/
ACE_INLINE
TAO_Cache_ExtId::TAO_Cache_ExtId (void)
  : transport_property_ (0),
    is_delete_ (0),
    index_ (0)
{
}

ACE_INLINE
TAO_Cache_ExtId::TAO_Cache_ExtId (TAO_Transport_Descriptor_Interface *prop)
  : transport_property_ (prop),
    is_delete_ (0),
    index_ (0)
{

}

ACE_INLINE
TAO_Cache_ExtId::~TAO_Cache_ExtId (void)
{
  if (this->is_delete_)
    delete this->transport_property_;
}

ACE_INLINE TAO_Cache_ExtId &
TAO_Cache_ExtId::operator= (const TAO_Cache_ExtId &rhs)
{
  if (this != &rhs)
    {
      // Do a deep copy
      this->transport_property_ =
        rhs.transport_property_->duplicate ();

      if (this->transport_property_ == 0)
        {
          this->is_delete_ = 0;
          this->index_ = 0;
        }
    else
      {
        this->is_delete_ = 1;
        this->index_ = rhs.index_;
      }
    }
  return *this;
}

ACE_INLINE
TAO_Cache_ExtId::TAO_Cache_ExtId (const TAO_Cache_ExtId &rhs)
  : transport_property_ (0),
    is_delete_ (0),
    index_ (0)
{
  *this = rhs;
}

ACE_INLINE int
TAO_Cache_ExtId::operator== (const TAO_Cache_ExtId &rhs) const
{
  return (this->transport_property_->is_equivalent (rhs.transport_property_) &&
            this->index_ == rhs.index_);
}

ACE_INLINE int
TAO_Cache_ExtId::operator!= (const TAO_Cache_ExtId &rhs) const
{
  if (this->transport_property_->is_equivalent (rhs.transport_property_) &&
      this->index_ == rhs.index_)
  return 0;

  return 1;
}

ACE_INLINE u_long
TAO_Cache_ExtId::hash (void) const
{
  return (this->transport_property_->hash () + this->index_);
}

ACE_INLINE void
TAO_Cache_ExtId::duplicate (void)
{
  TAO_Transport_Descriptor_Interface *prop = 0;

  // Make a deep copy
  prop = this->transport_property_->duplicate ();

  if (prop == 0)
    return;

  // Release memory if there was some allocated in the first place
 if (this->is_delete_)
   delete this->transport_property_;

  this->is_delete_ = 1;
  this->transport_property_ = prop;
}


ACE_INLINE CORBA::ULong
TAO_Cache_ExtId::index (void)
{
  return this->index_;
}

ACE_INLINE CORBA::ULong
TAO_Cache_ExtId::index (void) const
{
  return this->index_;
}


ACE_INLINE void
TAO_Cache_ExtId::index (CORBA::ULong index)
{
  this->index_ = index;
}

ACE_INLINE void
TAO_Cache_ExtId::incr_index (void)
{
  ++this->index_;
}

ACE_INLINE TAO_Transport_Descriptor_Interface *
TAO_Cache_ExtId::property (void) const
{
  return this->transport_property_;
}
