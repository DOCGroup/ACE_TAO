// -*- C++ -*-
//
//$Id$

ACE_INLINE TAO_ORB_Core *
TAO_Connection_Handler::orb_core (void)
{
  return this->orb_core_;
}

ACE_INLINE TAO_ORB_Core_TSS_Resources *
TAO_Connection_Handler::tss_resources (void)
{
  return this->tss_resources_;
}

ACE_INLINE TAO_Transport *
TAO_Connection_Handler::transport (void)
{
  return this->transport_;
}

#if 0
// NOTE: Not used
ACE_INLINE int
TAO_Connection_Handler::is_connect_complete (void) const
{
  return this->successful () || this->error_detected ();
}

ACE_INLINE int
TAO_Connection_Handler::is_connect_successful (void) const
{
  return (this->error_detected () == 0);
}
#endif /*if 0*/

ACE_INLINE int
TAO_Connection_Handler::is_finalized (void)
{
  return this->is_state_final ();
}
