// -*- C++ -*-
//
// $Id$

ACE_INLINE
TAO_ServerRequestInterceptor_Adapter::TAO_ServerRequestInterceptor_Adapter
  (TAO_ServerRequestInterceptor_List::TYPE &interceptors,
   size_t &stack_size)
    : interceptors_ (interceptors),
      len_ (0),
      stack_size_ (stack_size),
      location_forwarded_ (0)
{
  this->len_ = interceptors.size ();
}

ACE_INLINE CORBA::Boolean
TAO_ServerRequestInterceptor_Adapter::location_forwarded (void) const
{
  return this->location_forwarded_;
}
