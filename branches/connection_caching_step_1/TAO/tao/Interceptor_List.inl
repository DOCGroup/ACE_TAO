// -*- C++ -*-
//
// $Id$

ACE_INLINE TAO_ClientRequestInterceptor_List::TYPE &
TAO_ClientRequestInterceptor_List::interceptors (void)
{
  return this->interceptors_;
}

// ------------------

ACE_INLINE TAO_ServerRequestInterceptor_List::TYPE &
TAO_ServerRequestInterceptor_List::interceptors (void)
{
  return this->interceptors_;
}
