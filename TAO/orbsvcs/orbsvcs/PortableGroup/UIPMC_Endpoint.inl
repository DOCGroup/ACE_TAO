// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE const ACE_INET_Addr &
TAO_UIPMC_Endpoint::object_addr (void) const
{
  return this->object_addr_;
}


ACE_INLINE CORBA::UShort
TAO_UIPMC_Endpoint::port (void) const
{
  return this->port_;
}

ACE_INLINE CORBA::UShort
TAO_UIPMC_Endpoint::port (CORBA::UShort p)
{
  return this->port_ = p;
}


ACE_INLINE ACE_UINT32
TAO_UIPMC_Endpoint::uint_ip_addr (void) const
{
  return (((ACE_UINT32)this->class_d_address_[0] << 24) |
          ((ACE_UINT32)this->class_d_address_[1] << 16) |
          ((ACE_UINT32)this->class_d_address_[2] << 8)  |
          ((ACE_UINT32)this->class_d_address_[3]));
}

ACE_INLINE void
TAO_UIPMC_Endpoint::uint_ip_addr (ACE_UINT32 ip_addr)
{
  this->class_d_address_[0] = (CORBA::Char)(ip_addr >> 24);
  this->class_d_address_[1] = (CORBA::Char)((ip_addr >> 16) & 0xff);
  this->class_d_address_[2] = (CORBA::Char)((ip_addr >> 8) & 0xff);
  this->class_d_address_[3] = (CORBA::Char)(ip_addr & 0xff);
}

ACE_INLINE void
TAO_UIPMC_Endpoint::update_object_addr (void)
{
  this->object_addr_.set (this->port_,
                          this->uint_ip_addr ());
}


ACE_INLINE const char*
TAO_UIPMC_Endpoint::preferred_if () const
{
  return this->preferred_if_.in();
}

TAO_END_VERSIONED_NAMESPACE_DECL
