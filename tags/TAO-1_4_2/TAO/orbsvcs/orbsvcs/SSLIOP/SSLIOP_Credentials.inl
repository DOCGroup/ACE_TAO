// -*- C++ -*-
//
// $Id$

ACE_INLINE ::X509 *
TAO::SSLIOP::Credentials::x509 (void)
{
  return TAO::SSLIOP::_duplicate (this->x509_.in ());
}

ACE_INLINE ::EVP_PKEY *
TAO::SSLIOP::Credentials::evp (void)
{
  return TAO::SSLIOP::_duplicate (this->evp_.in ());
}
