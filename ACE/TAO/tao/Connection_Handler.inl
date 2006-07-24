// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_ORB_Core *
TAO_Connection_Handler::orb_core (void)
{
  return this->orb_core_;
}

ACE_INLINE TAO_Transport *
TAO_Connection_Handler::transport (void)
{
  return this->transport_;
}

ACE_INLINE bool
TAO_Connection_Handler::is_closed (void) const
{
  return (this->state_ == TAO_LF_Event::LFS_CONNECTION_CLOSED ||
          this->state_ == TAO_LF_Event::LFS_TIMEOUT);
}

ACE_INLINE bool
TAO_Connection_Handler::is_open (void) const
{
  return this->state_ == TAO_LF_Event::LFS_SUCCESS;
}

ACE_INLINE bool
TAO_Connection_Handler::is_connecting (void) const
{
  return this->state_ == TAO_LF_Event::LFS_CONNECTION_WAIT;
}

//@@ CONNECTION_HANDLER_SPL_METHODS_ADD_HOOK

TAO_END_VERSIONED_NAMESPACE_DECL
