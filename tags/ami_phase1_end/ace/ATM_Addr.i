/* -*- C++ -*- */
// $Id$

// ATM_Addr.i

// Default dtor.
ACE_INLINE
ACE_ATM_Addr::~ACE_ATM_Addr (void)
{
}

// Return the address.

ACE_INLINE void *
ACE_ATM_Addr::get_addr (void) const
{
  ACE_TRACE ("ACE_ATM_Addr::get_addr");
  return (void *) &this->atm_addr_;
}
