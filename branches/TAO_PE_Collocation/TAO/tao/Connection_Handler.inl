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

#if !defined (TAO_HAS_COLLOCATION)
ACE_INLINE TAO_Transport *
TAO_Connection_Handler::transport (void)
{
  return this->transport_;
}
#endif

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

ACE_INLINE int
TAO_Connection_Handler::set_dscp_codepoint (CORBA::Boolean)
{
  return 0;
}

ACE_INLINE void
TAO_Connection_Handler::set_op_signature (TAO_Skeleton &skeleton,
                                          TAO_Servant_Base *&servant)
{
  this->op_signature_ = skeleton;
  this->servant_ = servant;
}

ACE_INLINE void
TAO_Connection_Handler::get_op_signature (TAO_Skeleton &operation_ptr,
                                          TAO_Servant_Base *&servant)
{
  operation_ptr = this->op_signature_;
  servant = this->servant_;
}
