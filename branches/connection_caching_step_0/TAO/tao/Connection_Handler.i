//* -*- C++ -*- */
//$Id$

ACE_INLINE
TAO_Connection_Handler::TAO_Connection_Handler (void)
  : orb_core_ (0),
    tss_resources_ (0)
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
