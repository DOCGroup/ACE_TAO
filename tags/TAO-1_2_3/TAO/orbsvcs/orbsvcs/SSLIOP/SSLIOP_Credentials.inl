// -*- C++ -*-
//
// $Id$

ACE_INLINE X509 *
TAO_SSLIOP_Credentials::x509 (void)
{
  return TAO_SSLIOP_X509::_duplicate (this->x509_.in ());
}

ACE_INLINE EVP_PKEY *
TAO_SSLIOP_Credentials::evp (void)
{
  return TAO_SSLIOP_EVP_PKEY::_duplicate (this->evp_.in ());
}
