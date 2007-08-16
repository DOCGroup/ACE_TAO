// -*- C++ -*-
//
// $Id$


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE const SSLIOP::SSL &
TAO::SSLIOP::Acceptor::ssl_component (void) const
{
  return this->ssl_component_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
