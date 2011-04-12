// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {

    ACE_INLINE
    const ACE_SSL_Context* SSL_CallbackManager::context () const
      {
        return this->ssl_ctx_;
      }

    ACE_INLINE
    void SSL_CallbackManager::set_certificate_callback (ACE::INet::SSL_CertificateCallback* cb)
      {
        this->cert_callback_.reset (cb);
      }

    ACE_INLINE
    void SSL_CallbackManager::set_password_callback (ACE::INet::SSL_PasswordCallback* cb)
      {
        this->passwd_callback_.reset (cb);
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
