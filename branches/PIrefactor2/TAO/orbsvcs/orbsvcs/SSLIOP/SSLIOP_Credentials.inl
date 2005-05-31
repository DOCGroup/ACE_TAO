// -*- C++ -*-
//
// $Id$

ACE_INLINE ::X509 *
TAO::SSLIOP_Credentials::x509 (void)
{
  return
    TAO::SSLIOP::OpenSSL_traits< ::X509 >::_duplicate (this->x509_.in ());
}

ACE_INLINE ::EVP_PKEY *
TAO::SSLIOP_Credentials::evp (void)
{
  return
    TAO::SSLIOP::OpenSSL_traits< ::EVP_PKEY >::_duplicate (this->evp_.in ());
}
