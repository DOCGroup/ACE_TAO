/* -*- C++ -*- */
// $Id$

ACE_INLINE
TAO_Cache_IntId::TAO_Cache_IntId (void)
  :handle_ (),
   handler_ (0)
{
}

ACE_INLINE
TAO_Cache_IntId::TAO_Cache_IntId (ACE_HANDLE &handle,
                                  TAO_Connection_Handler *handler)
  :handle_ (handle),
   handler_ (handler)
{
}

ACE_INLINE
TAO_Cache_IntId::TAO_Cache_IntId (const TAO_Cache_IntId &rhs)
{
  this->handle_ = rhs.handle_;
}

ACE_INLINE
TAO_Cache_IntId::~TAO_Cache_IntId (void)
{
}

/*******************************************************/
ACE_INLINE
TAO_Cache_ExtId::TAO_Cache_ExtId (void)
  : connection_property_ (0)
{
}

ACE_INLINE
TAO_Cache_ExtId::TAO_Cache_ExtId (TAO_Base_Connection_Property *prop)
  : connection_property_ (prop->duplicate ())
{

}

ACE_INLINE
TAO_Cache_ExtId::~TAO_Cache_ExtId (void)
{
  delete this->connection_property_;
}


ACE_INLINE void
TAO_Cache_ExtId::operator= (const TAO_Cache_ExtId &rhs)
{
  *this->connection_property_ = *rhs.connection_property_;
}

ACE_INLINE int
TAO_Cache_ExtId::operator== (const TAO_Cache_ExtId &rhs)
{
  return *this->connection_property_ == *rhs.connection_property_;
}

ACE_INLINE int
TAO_Cache_ExtId::operator!= (const TAO_Cache_ExtId &rhs)
{
  return *this->connection_property_ != *rhs.connection_property_;
}

ACE_INLINE u_long
TAO_Cache_ExtId::hash (void) const
{
  return this->connection_property_;
}
