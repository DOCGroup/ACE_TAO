// -*- C++ -*-
//
//$Id$

ACE_INLINE
TAO_Connection_Handler::TAO_Connection_Handler (void)
  : orb_core_ (0),
    transport_ (0),
    tss_resources_ (0),
    pending_upcalls_ (1),
    pending_upcall_lock_ (0)
{
}

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

ACE_INLINE int
TAO_Connection_Handler::is_connect_complete (void) const
{
  return this->successful () ||
    this->error_detected ();
}

ACE_INLINE int
TAO_Connection_Handler::is_connect_successful (void) const
{
  return (this->error_detected () == 0);
}

ACE_INLINE int
TAO_Connection_Handler::pending_upcalls (void) const
{
  return this->pending_upcalls_;
}
