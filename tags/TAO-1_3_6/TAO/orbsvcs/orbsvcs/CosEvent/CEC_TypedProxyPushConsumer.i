// $Id$

ACE_INLINE CORBA::Boolean
TAO_CEC_TypedProxyPushConsumer::is_connected_i (void) const
{
  return this->connected_;
}

// ****************************************************************

ACE_INLINE int
TAO_CEC_TypedProxyPushConsumer_Guard::locked (void) const
{
  return this->locked_;
}
