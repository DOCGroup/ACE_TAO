// -*- C++ -*-
//
// $Id$

ACE_INLINE X509 *
TAO_SSLIOP_Credentials::x509 (void)
{
  return TAO_SSLIOP_X509::_duplicate (this->x509_.in ());
}
