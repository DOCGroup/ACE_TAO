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
TAO_Connection_Handler::incr_pending_upcalls (void)
{
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->pending_upcall_lock_, -1);

  return ++this->pending_upcalls_;


}

ACE_INLINE int
TAO_Connection_Handler::decr_pending_upcalls (void)
{
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->pending_upcall_lock_, -1);

  return --this->pending_upcalls_;
}

ACE_INLINE int
TAO_Connection_Handler::pending_upcalls (void) const
{
  return this->pending_upcalls_;
}
