// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::Boolean
TAO_CEC_TypedProxyPushConsumer::is_connected_i () const
{
  return this->connected_;
}

// ****************************************************************

ACE_INLINE int
TAO_CEC_TypedProxyPushConsumer_Guard::locked () const
{
  return this->locked_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
