// -*- C++ -*-
//
// $Id$

TAO_ClientRequestInterceptor_List::TYPE &
TAO_ClientRequestInterceptor_List::interceptors (void)
{
  return this->interceptors_;
}

// ------------------

TAO_ServerRequestInterceptor_List::TYPE &
TAO_ServerRequestInterceptor_List::interceptors (void)
{
  return this->interceptors_;
}
