/* -*- C++ -*- */
// $Id$

// Addr.i

// Return the address of the address.

ACE_INLINE void *
ACE_Addr::get_addr (void) const
{
  return 0;
}

ACE_INLINE void 
ACE_Addr::set_addr (void *, int)
{
}

ACE_INLINE int 
ACE_Addr::operator == (const ACE_Addr &sap) const
{
  return sap.addr_type_ == 0;
}

ACE_INLINE int
ACE_Addr::operator != (const ACE_Addr &sap) const
{
  return sap.addr_type_ != 0;
}

// Return the size of the address. 

ACE_INLINE int
ACE_Addr::get_size (void) const
{
  return this->addr_size_;
}

// Sets the size of the address. 

ACE_INLINE void
ACE_Addr::set_size (int size)
{
  this->addr_size_ = size;
}

// Return the type of the address. 

ACE_INLINE int
ACE_Addr::get_type (void) const
{
  return this->addr_type_;
}

// Set the type of the address. 

ACE_INLINE void
ACE_Addr::set_type (int type)
{
  this->addr_type_ = type;
}
