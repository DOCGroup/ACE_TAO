/* -*- C++ -*- */
// $Id$

ACE_INLINE
TAO_Cache_IntId::TAO_Cache_IntId (void)
  : handler_ (0)
{
}

ACE_INLINE
TAO_Cache_IntId::TAO_Cache_IntId (TAO_Connection_Handler *handler)
  : handler_ (handler)
{
}

ACE_INLINE
TAO_Cache_IntId::TAO_Cache_IntId (const TAO_Cache_IntId &rhs)
{
  this->handler_ = rhs.handler_;
}

ACE_INLINE
TAO_Cache_IntId::~TAO_Cache_IntId (void)
{
}

ACE_INLINE void
TAO_Cache_IntId::operator= (const TAO_Cache_IntId &rhs)
{
  this->handler_ = rhs.handler_;
}

ACE_INLINE int
TAO_Cache_IntId::operator== (const TAO_Cache_IntId &rhs) const
{
  return (this->handler_ == rhs.handler_);
}

ACE_INLINE int
TAO_Cache_IntId::operator!= (const TAO_Cache_IntId &rhs) const
{
  return (this->handler_ != rhs.handler_);
}

ACE_INLINE TAO_Connection_Handler *
TAO_Cache_IntId::handler (void)
{
  return this->handler_;
}

ACE_INLINE const TAO_Connection_Handler *
TAO_Cache_IntId::handler (void) const
{
  return this->handler_;
}

/*******************************************************/
ACE_INLINE
TAO_Cache_ExtId::TAO_Cache_ExtId (void)
  : connection_property_ (0),
    is_delete_ (0),
    index_ (0),
    recycle_state_ (ACE_RECYCLABLE_UNKNOWN)
{
}

ACE_INLINE
TAO_Cache_ExtId::TAO_Cache_ExtId (TAO_Base_Connection_Property *prop)
  : connection_property_ (prop),
    is_delete_ (0),
    index_ (0),
    recycle_state_ (ACE_RECYCLABLE_UNKNOWN)
{

}

ACE_INLINE
TAO_Cache_ExtId::~TAO_Cache_ExtId (void)
{
  if (this->is_delete_)
    delete this->connection_property_;
}

ACE_INLINE
TAO_Cache_ExtId::TAO_Cache_ExtId (const TAO_Cache_ExtId &rhs)
{
  // Do a deep copy
  this->connection_property_ =
    rhs.connection_property_->duplicate ();
  this->is_delete_ = 1;
  this->index_ = rhs.index_;
  this->recycle_state_ = rhs.recycle_state_;
}

ACE_INLINE void
TAO_Cache_ExtId::operator= (const TAO_Cache_ExtId &rhs)
{
  // Do a deep copy
  this->connection_property_ =
    rhs.connection_property_->duplicate ();
  this->is_delete_ = 1;
  this->index_ = rhs.index_;
  this->recycle_state_ = rhs.recycle_state_;
}

ACE_INLINE int
TAO_Cache_ExtId::operator== (const TAO_Cache_ExtId &rhs) const
{
  return (*this->connection_property_ == *rhs.connection_property_ &&
            this->index_ == rhs.index_);
}

ACE_INLINE int
TAO_Cache_ExtId::operator!= (const TAO_Cache_ExtId &rhs) const
{
  return (*this->connection_property_ != *rhs.connection_property_ ||
          this->index_ != rhs.index_);
}

ACE_INLINE u_long
TAO_Cache_ExtId::hash (void) const
{
  return (this->connection_property_->hash () + this->index_);
}

ACE_INLINE void
TAO_Cache_ExtId::duplicate (void)
{
  TAO_Base_Connection_Property *prop = 0;

  // Make a deep copy
  prop = this->connection_property_->duplicate ();


  // Release memory if there was some allocated in the first place
 if (this->is_delete_)
   delete this->connection_property_;

  this->is_delete_ = 1;
  this->connection_property_ = prop;
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

ACE_INLINE TAO_Base_Connection_Property *
TAO_Cache_ExtId::property (void) const
{
  return this->connection_property_;
}

ACE_INLINE void
TAO_Cache_ExtId::recycle_state (ACE_Recyclable_State st)
{
  this->recycle_state_ = st;
}

ACE_INLINE ACE_Recyclable_State
TAO_Cache_ExtId::recycle_state (void)
{
  return this->recycle_state_;
}
