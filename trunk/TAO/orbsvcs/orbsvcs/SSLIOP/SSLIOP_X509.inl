// -*- C++ -*-
//
// $Id$

ACE_INLINE ::X509 *
TAO::SSLIOP::_duplicate (::X509 *cert)
{
  // OpenSSL provides no function to increase the reference count on
  // the X509 structure, so we do it manually.  (X509_dup() performs a
  // deep copy, not a shallow copy.)
  if (cert != 0)
    CRYPTO_add (&(cert->references), 1, CRYPTO_LOCK_X509);

  return cert;
}

ACE_INLINE void
TAO::SSLIOP::release (::X509 *cert)
{
  // OpenSSL's X509_free() function already handles reference counting
  // properly.
  ::X509_free (cert);
}


// -------------------------------------------------------------------

ACE_INLINE
TAO::SSLIOP::X509_var::X509_var (void)
  : x509_ (0)
{
}

ACE_INLINE
TAO::SSLIOP::X509_var::X509_var (::X509 *x)
  : x509_ (x)
{
}

ACE_INLINE
TAO::SSLIOP::X509_var::X509_var (const TAO::SSLIOP::X509_var &p)
  : TAO_Base_var (),
    x509_ (TAO::SSLIOP::_duplicate (p.ptr ()))
{
}

ACE_INLINE
TAO::SSLIOP::X509_var::X509_var (const ::X509 &p)
{
  this->x509_ = X509_dup (const_cast< ::X509 * > (&p));
}

ACE_INLINE
TAO::SSLIOP::X509_var::~X509_var (void)
{
  TAO::SSLIOP::release (this->x509_);
}

ACE_INLINE TAO::SSLIOP::X509_var &
TAO::SSLIOP::X509_var::operator= (::X509 *p)
{
  TAO::SSLIOP::release (this->x509_);
  this->x509_ = p;
  return *this;
}

ACE_INLINE ::TAO::SSLIOP::X509_var &
TAO::SSLIOP::X509_var::operator= (const ::TAO::SSLIOP::X509_var &p)
{
  if (this != &p)
    {
      TAO::SSLIOP::release (this->x509_);
      this->x509_ = TAO::SSLIOP::_duplicate (p.ptr ());
    }

  return *this;
}

ACE_INLINE TAO::SSLIOP::X509_var &
TAO::SSLIOP::X509_var::operator= (const ::X509 &p)
{
  if (this->x509_ != &p)
    {
      TAO::SSLIOP::release (this->x509_);
      this->x509_ = X509_dup (const_cast< ::X509 * > (&p));
    }

  return *this;
}

ACE_INLINE const ::X509 *
TAO::SSLIOP::X509_var::operator-> (void) const
{
  return this->x509_;
}

ACE_INLINE ::X509 *
TAO::SSLIOP::X509_var::operator-> (void)
{
  return this->x509_;
}

ACE_INLINE
TAO::SSLIOP::X509_var::operator const ::X509 &() const
{
  return *this->x509_;
}

ACE_INLINE
TAO::SSLIOP::X509_var::operator ::X509 &()
{
  return *this->x509_;
}

ACE_INLINE ::X509 *
TAO::SSLIOP::X509_var::in (void) const
{
  return this->x509_;
}

ACE_INLINE ::X509 *&
TAO::SSLIOP::X509_var::inout (void)
{
  return this->x509_;
}

ACE_INLINE ::X509 *&
TAO::SSLIOP::X509_var::out (void)
{
  X509_free (this->x509_);
  this->x509_ = 0;
  return this->x509_;
}

ACE_INLINE ::X509 *
TAO::SSLIOP::X509_var::_retn (void)
{
  // Yield ownership of the X509 structure.
  ::X509 *x = this->x509_;
  this->x509_ = 0;
  return x;
}

ACE_INLINE ::X509 *
TAO::SSLIOP::X509_var::ptr (void) const
{
  return this->x509_;
}
