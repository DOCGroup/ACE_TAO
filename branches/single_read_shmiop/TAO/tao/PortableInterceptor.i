// -*- C++ -*-
//
// $Id$

ACE_INLINE
TAO_ClientRequestInterceptor_Adapter::TAO_ClientRequestInterceptor_Adapter
  (TAO_ClientRequestInterceptor_List::TYPE &interceptors)
    : interceptors_ (interceptors),
      len_ (0),
      stack_size_ (0)
{
  this->len_ = interceptors.size ();
}

// -------------------------------------------------------------------

ACE_INLINE
TAO_ServerRequestInterceptor_Adapter::TAO_ServerRequestInterceptor_Adapter
  (TAO_ServerRequestInterceptor_List::TYPE &interceptors,
   size_t &stack_size)
    : interceptors_ (interceptors),
      len_ (0),
      stack_size_ (stack_size)
{
  this->len_ = interceptors.size ();
}
