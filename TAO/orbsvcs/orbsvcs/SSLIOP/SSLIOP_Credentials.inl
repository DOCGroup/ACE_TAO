// -*- C++ -*-
//
// $Id$

ACE_INLINE X509 *
TAO_SSLIOP_Credentials::x509 (void)
{
  return TAO_SSLIOP_X509::_duplicate (this->x509_.in ());
}

#ifndef NO_RSA
ACE_INLINE RSA *
TAO_SSLIOP_Credentials::rsa (void)
{
  return TAO_SSLIOP_RSA::_duplicate (this->rsa_.in ());
}
#endif  /* !NO_RSA */

// #ifndef NO_DSA
// ACE_INLINE DSA *
// TAO_SSLIOP_Credentials::dsa (void)
// {
//   return TAO_SSLIOP_DSA::_duplicate (this->dsa_.in ());
// }
// #endif  /* !NO_DSA */
