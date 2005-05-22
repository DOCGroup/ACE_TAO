// -*- C++ -*-
//
// $Id$

#if TAO_HAS_INTERCEPTORS == 1
ACE_INLINE TAO_ClientRequestInterceptor_List::TYPE &
TAO_ClientRequestInterceptor_List::interceptors (void)
{
  return this->interceptors_;
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

