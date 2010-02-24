// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Smart_Proxy_Base::TAO_Smart_Proxy_Base (void)
{
}

ACE_INLINE
TAO_Smart_Proxy_Base::TAO_Smart_Proxy_Base (CORBA::Object_ptr proxy)
  : base_proxy_(proxy)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
