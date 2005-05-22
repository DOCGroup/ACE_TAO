// $Id$

#include "tao/ServerRequestInterceptor_List.h"

#if !defined (__ACE_INLINE__)
# include "tao/ServerRequestInterceptor_List.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           ServerRequestInterceptor_List,
           "$Id$")

#if TAO_HAS_INTERCEPTORS == 1


TAO_ServerRequestInterceptor_List::TAO_ServerRequestInterceptor_List (void)
  : interceptors_ ()
{
}

TAO_ServerRequestInterceptor_List::~TAO_ServerRequestInterceptor_List (void)
{
}

size_t
TAO_ServerRequestInterceptor_List::length (void)
{
  return this->interceptors_.size ();
}

void
TAO_ServerRequestInterceptor_List::length (size_t len)
{
  this->interceptors_.size (len);
}

PortableInterceptor::Interceptor_ptr
TAO_ServerRequestInterceptor_List::interceptor (size_t index)
{
  return this->interceptors_[index].in ();
}

void
TAO_ServerRequestInterceptor_List::add_interceptor (
  PortableInterceptor::ServerRequestInterceptor_ptr interceptor
  ACE_ENV_ARG_DECL)
{
  const size_t index = this->add_interceptor_i (interceptor
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->interceptors_[index] =
    PortableInterceptor::ServerRequestInterceptor::_duplicate (interceptor);
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

