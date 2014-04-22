// $Id$

#include "orbsvcs/SSLIOP/SSLIOP_Credentials.h"
#include "tao/ORB_Constants.h"
#include "ace/SString.h"

#if !defined (__ACE_INLINE__)
# include "orbsvcs/SSLIOP/SSLIOP_Credentials.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP_Credentials::SSLIOP_Credentials (::X509 *cert, ::EVP_PKEY *evp)
  : x509_ (TAO::SSLIOP::OpenSSL_traits< ::X509 >::_duplicate (cert)),
    evp_ (TAO::SSLIOP::OpenSSL_traits< ::EVP_PKEY >::_duplicate (evp)),
    id_ (),
    creds_usage_ (SecurityLevel3::CU_Indefinite),
    expiry_time_ (),
    creds_state_ (SecurityLevel3::CS_Invalid)
{
  ::X509 *x = cert;

  if (x != 0)
    {
      // We use the X.509 certificate's serial number as the
      // credentials Id.
      BIGNUM * bn = ASN1_INTEGER_to_BN (::X509_get_serialNumber (x), 0);
      if (BN_is_zero (bn))
        this->id_ = CORBA::string_dup ("X509: 00");
      else
        {
          char * id = BN_bn2hex (bn);

          ACE_CString s =
            ACE_CString ("X509: ")
            + ACE_CString (const_cast<const char *> (id));

          this->id_ = CORBA::string_dup (s.c_str ());

#ifdef OPENSSL_free
          OPENSSL_free (id);
#else
          // Older versions of OpenSSL didn't define the OpenSSL
          // macro.
          CRYPTO_free (id);
#endif  /* OPENSSL_free */
        }
      BN_free (bn);

      // -------------------------------------------

      TimeBase::UtcT & t = this->expiry_time_;

      const ASN1_TIME * exp = X509_get_notAfter (x);

      if (exp->length > ACE_SIZEOF_LONG_LONG)
        {
          // @@ Will this ever happen?

          // Overflow!
          t.time = ACE_UINT64_LITERAL (0xffffffffffffffff);
        }
      else
        {
          t.time = 0;
          for (int i = 0; i < exp->length; ++i)
            {
              t.time <<= 8;
              t.time |= (unsigned char) exp->data[i];
            }
        }
    }
}

TAO::SSLIOP_Credentials::~SSLIOP_Credentials (void)
{
}

char *
TAO::SSLIOP_Credentials::creds_id (void)
{
  return CORBA::string_dup (this->id_.in ());
}

SecurityLevel3::CredentialsUsage
TAO::SSLIOP_Credentials::creds_usage (void)
{
  return SecurityLevel3::CU_Indefinite;
}

TimeBase::UtcT
TAO::SSLIOP_Credentials::expiry_time (void)
{
  return this->expiry_time_;
}

SecurityLevel3::CredentialsState
TAO::SSLIOP_Credentials::creds_state (void)
{
  const ::X509 *x = this->x509_.in ();

  // The pointer to the underlying X509 structure should only be zero
  // if destroy() was called on this Credentials object.
  if (x == 0)
    throw CORBA::BAD_OPERATION ();

  if (this->creds_state_ == SecurityLevel3::CS_Valid)
    {
      // Make sure the X.509 certificate is still valid.

      const int after_status =
        ::X509_cmp_current_time (X509_get_notAfter (x));

      if (after_status == 0)
        {
          // Error in certificate's "not after" field.
          throw CORBA::BAD_PARAM ();
        }
      else if (after_status > 0)     // Certificate has expired.
        this->creds_state_ = SecurityLevel3::CS_Expired;
    }
  else if (this->creds_state_ == SecurityLevel3::CS_Invalid)
    {
      // Check if the X.509 certificate has become valid.

      const int before_status =
        ::X509_cmp_current_time (X509_get_notBefore (x));

      if (before_status == 0)
        {
          // Error in certificate's "not before" field.
          throw CORBA::BAD_PARAM ();
        }
      else if (before_status < 0)     // Certificate is now valid.
        this->creds_state_ = SecurityLevel3::CS_Valid;
    }

  return this->creds_state_;
}

char *
TAO::SSLIOP_Credentials::add_relinquished_listener (
    SecurityLevel3::RelinquishedCredentialsListener_ptr /* listener */)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
TAO::SSLIOP_Credentials::remove_relinquished_listener (const char * /* id */)
{
  throw CORBA::NO_IMPLEMENT ();
}

bool
TAO::SSLIOP_Credentials::operator== (const TAO::SSLIOP_Credentials &rhs)
{
  ::X509 * xa = this->x509_.in ();
  ::X509 * xb = rhs.x509_.in ();
  // EVP_PKEY *ea = this->evp_.in ();
  // EVP_PKEY *eb = rhs.evp_.in ();

  // No need for a full blown ACE_TRY/CATCH block.

  const SecurityLevel3::CredentialsType lct =
    this->creds_type ();

  const SecurityLevel3::CredentialsType rct =
    const_cast<TAO::SSLIOP_Credentials &> (rhs).creds_type ();

  // Don't bother check the creds_id and expiry_time attributes.  They
  // are checked implicitly by the below X509_cmp() call.
  //
  // Additionally, the creds_state attribute is not included in the
  // check since it is not considered important when distinguishing
  // between two Credentials.

  return
    lct == rct
    && this->creds_usage_ == rhs.creds_usage_
    && ((xa == xb) || (xa != 0 && xb != 0 && ::X509_cmp (xa, xb) == 0))
//     && ((ea == eb) || (ea != 0 && eb != 0 && ::EVP_PKEY_cmp (ea, eb) == 0))
    ;
}

CORBA::ULong
TAO::SSLIOP_Credentials::hash (void) const
{
  ::X509 * x509 = this->x509_.in ();

  return (x509 == 0 ? 0 : ::X509_issuer_name_hash (x509));
}

TAO::SSLIOP::Credentials_ptr
TAO::SSLIOP_Credentials::_narrow (CORBA::Object_ptr obj)
{
  return  TAO::SSLIOP_Credentials::_duplicate (
              dynamic_cast<TAO::SSLIOP_Credentials *> (obj));
}

TAO::SSLIOP::Credentials_ptr
TAO::SSLIOP_Credentials::_duplicate (TAO::SSLIOP::Credentials_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();

  return obj;
}

// -----------------------------------------------------------

TAO::SSLIOP::Credentials_ptr
tao_TAO_SSLIOP_Credentials_duplicate (TAO::SSLIOP::Credentials_ptr p)
{
  return TAO::SSLIOP_Credentials::_duplicate (p);
}

void
tao_TAO_SSLIOP_Credentials_release (TAO::SSLIOP::Credentials_ptr p)
{
  CORBA::release (p);
}

TAO::SSLIOP::Credentials_ptr
tao_TAO_SSLIOP_Credentials_nil (void)
{
  return TAO::SSLIOP_Credentials::_nil ();
}

TAO::SSLIOP::Credentials_ptr
tao_TAO_SSLIOP_Credentials_narrow (CORBA::Object *p)
{
  return TAO::SSLIOP_Credentials::_narrow (p);
}

CORBA::Object_ptr
tao_TAO_SSLIOP_Credentials_upcast (void *src)
{
  TAO::SSLIOP_Credentials **tmp =
    static_cast<TAO::SSLIOP_Credentials **> (src);

  return *tmp;
}

TAO_END_VERSIONED_NAMESPACE_DECL
