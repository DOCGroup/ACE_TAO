// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {

    ACE_INLINE
    SSL_X509Cert::SSL_X509Cert ()
      : ssl_cert_ (nullptr)
      {
      }

    ACE_INLINE
    SSL_X509Cert::SSL_X509Cert (::X509* ssl_cert)
      : ssl_cert_ (nullptr)
      {
        *this = ssl_cert;
      }

    ACE_INLINE
    SSL_X509Cert::SSL_X509Cert (const SSL_X509Cert& cert)
      {
        *this = cert;
      }

    ACE_INLINE
    SSL_X509Cert::~SSL_X509Cert () = default;

    ACE_INLINE
    SSL_X509Cert& SSL_X509Cert::operator= (::X509* ssl_cert)
      {
        if (this->ssl_cert_ != nullptr)
          ::X509_free (this->ssl_cert_);

        if (ssl_cert != nullptr)
          {
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
            ::X509_up_ref(ssl_cert);
#else
            CRYPTO_add (&(ssl_cert->references),
                        1,
                        CRYPTO_LOCK_X509);
#endif
          }
        this->ssl_cert_ = ssl_cert;
        return *this;
      }

    ACE_INLINE
    SSL_X509Cert& SSL_X509Cert::operator= (const SSL_X509Cert& ssl_cert)
      {
        return (*this = &(const_cast<SSL_X509Cert&> (ssl_cert)));
      }

    ACE_INLINE
    ::X509* SSL_X509Cert::operator & ()
      {
        return this->ssl_cert_;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
