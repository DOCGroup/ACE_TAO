// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {

    ACE_INLINE
    const ACE_SSL_Context& SSL_CertificateCallbackArg::context () const
      {
        return *this->ssl_ctx_;
      }

    ACE_INLINE
    SSL_X509Cert& SSL_CertificateCallbackArg::certificate (void)
      {
        return this->ssl_cert_;
      }

    ACE_INLINE
    int SSL_CertificateCallbackArg::error_depth () const
      {
        return this->err_depth_;
      }

    ACE_INLINE
    int SSL_CertificateCallbackArg::error_code () const
      {
        return this->err_code_;
      }

    ACE_INLINE
    ACE_CString SSL_CertificateCallbackArg::error_message () const
      {
        return ACE_CString (X509_verify_cert_error_string (this->err_code_));
      }

    ACE_INLINE
    bool SSL_CertificateCallbackArg::ignore_error () const
      {
        return this->ignore_err_;
      }

    ACE_INLINE
    void SSL_CertificateCallbackArg::ignore_error (bool f)
      {
        this->ignore_err_ = f;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
