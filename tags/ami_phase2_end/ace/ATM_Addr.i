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

ACE_INLINE unsigned char
ACE_ATM_Addr::get_selector (void) const
{
  ACE_TRACE ("ACE_ATM_Addr::get_selector");
#if defined (ACE_HAS_FORE_ATM_XTI)
  return atm_addr_.sap.t_atm_sap_addr.address[ATMNSAP_ADDR_LEN - 1];
#else
  return 0;
#endif /* ACE_HAS_FORE_ATM_XTI */
}

ACE_INLINE void
ACE_ATM_Addr::set_selector (unsigned char selector)
{
  ACE_TRACE ("ACE_ATM_Addr::set_selector");
#if defined (ACE_HAS_FORE_ATM_XTI)
  atm_addr_.sap.t_atm_sap_addr.address[ATMNSAP_ADDR_LEN - 1] = selector;
#else
  ACE_UNUSED_ARG (selector);
#endif /* ACE_HAS_FORE_ATM_XTI */
}
