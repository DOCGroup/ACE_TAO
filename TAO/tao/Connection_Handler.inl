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

ACE_INLINE int
TAO_Connection_Handler::is_closed (void)
{
  return this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED;
}

ACE_INLINE int
TAO_Connection_Handler::is_open (void)
{
  return this->state_ == TAO_LF_Event::LFS_SUCCESS;
}

ACE_INLINE int
TAO_Connection_Handler::is_connecting (void)
{
  return this->state_ == TAO_LF_Event::LFS_CONNECTION_WAIT;
}
