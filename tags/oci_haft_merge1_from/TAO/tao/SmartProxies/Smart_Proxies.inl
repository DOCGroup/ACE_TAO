/* -*- C++ -*- */
// $Id$

ACE_INLINE
TAO_Smart_Proxy_Base::TAO_Smart_Proxy_Base (void)
{
}

ACE_INLINE
TAO_Smart_Proxy_Base::TAO_Smart_Proxy_Base (CORBA::Object_ptr proxy)
  : base_proxy_(proxy)
{
}

ACE_INLINE
TAO_Smart_Proxy_Base::~TAO_Smart_Proxy_Base (void)
{
}
