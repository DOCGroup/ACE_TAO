// -*- C++ -*-
//
// $Id$

ACE_INLINE RSA *
TAO_SSLIOP_RSA::_duplicate (RSA *cert)
{
  // OpenSSL provides no function to increase the reference count on
  // the RSA structure, so we do it manually.  (RSA_dup() performs a
  // deep copy, not a shallow copy.)
  CRYPTO_add (&(cert->references), 1, CRYPTO_LOCK_RSA);

  return cert;
}

ACE_INLINE void
TAO_SSLIOP_RSA::release (RSA *cert)
{
  // OpenSSL's RSA_free() function already handles reference counting
  // properly.
  ::RSA_free (cert);
}


// -------------------------------------------------------------------

ACE_INLINE
TAO_SSLIOP_RSA_var::TAO_SSLIOP_RSA_var (void)
  : rsa_ (0)
{
}

ACE_INLINE
TAO_SSLIOP_RSA_var::TAO_SSLIOP_RSA_var (RSA *x)
  : rsa_ (x)
{
}

ACE_INLINE
TAO_SSLIOP_RSA_var::TAO_SSLIOP_RSA_var (const ::TAO_SSLIOP_RSA_var &p)
  : TAO_Base_var (),
    rsa_ (TAO_SSLIOP_RSA::_duplicate (p.ptr ()))
{
}

ACE_INLINE
TAO_SSLIOP_RSA_var::~TAO_SSLIOP_RSA_var (void)
{
  TAO_SSLIOP_RSA::release (this->rsa_);
}

ACE_INLINE TAO_SSLIOP_RSA_var &
TAO_SSLIOP_RSA_var::operator= (RSA *p)
{
  TAO_SSLIOP_RSA::release (this->rsa_);
  this->rsa_ = p;
  return *this;
}

ACE_INLINE ::TAO_SSLIOP_RSA_var &
TAO_SSLIOP_RSA_var::operator= (const ::TAO_SSLIOP_RSA_var &p)
{
  if (this != &p)
    {
      TAO_SSLIOP_RSA::release (this->rsa_);
      this->rsa_ = TAO_SSLIOP_RSA::_duplicate (p.ptr ());
    }

  return *this;  
}

ACE_INLINE const RSA *
TAO_SSLIOP_RSA_var::operator-> (void) const
{
  return this->rsa_;
}

ACE_INLINE RSA *
TAO_SSLIOP_RSA_var::operator-> (void)
{
  return this->rsa_;
}

ACE_INLINE
TAO_SSLIOP_RSA_var::operator const RSA &() const
{
  return *this->rsa_;
}

ACE_INLINE
TAO_SSLIOP_RSA_var::operator RSA &()
{
  return *this->rsa_;
}

ACE_INLINE RSA *
TAO_SSLIOP_RSA_var::in (void) const
{
  return this->rsa_;
}

ACE_INLINE RSA *&
TAO_SSLIOP_RSA_var::inout (void)
{
  return this->rsa_;
}

ACE_INLINE RSA *&
TAO_SSLIOP_RSA_var::out (void)
{
  RSA_free (this->rsa_);
  this->rsa_ = 0;
  return this->rsa_;
}

ACE_INLINE RSA *
TAO_SSLIOP_RSA_var::_retn (void)
{
  // Yield ownership of the RSA structure.
  RSA *x = this->rsa_;
  this->rsa_ = 0;
  return x;
}

ACE_INLINE RSA *
TAO_SSLIOP_RSA_var::ptr (void) const
{
  return this->rsa_;
}
