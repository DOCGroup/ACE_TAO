// $Id$

#include "ace/INet/SSL_CertificateCallback.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/SSL_CertificateCallback.inl"
#endif

#include "ace/Log_Msg.h"
#include "ace/INet/INet_Log.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {

    SSL_CertificateCallbackArg::SSL_CertificateCallbackArg (const ACE_SSL_Context* ssl_ctx,
                                                            ::X509_STORE_CTX* cert_ctx)
      : ssl_ctx_ (ssl_ctx),
        err_depth_ (0),
        err_code_ (0),
        ignore_err_ (false)
      {
        this->ssl_cert_ = ::X509_STORE_CTX_get_current_cert (cert_ctx);
        this->err_depth_ = ::X509_STORE_CTX_get_error_depth (cert_ctx);
        this->err_code_ = ::X509_STORE_CTX_get_error (cert_ctx);
      }

    SSL_CertificateCallbackArg::~SSL_CertificateCallbackArg ()
      {
      }

    SSL_CertificateCallback::SSL_CertificateCallback ()
      {
      }

    SSL_CertificateCallback::~SSL_CertificateCallback ()
      {
      }

    SSL_CertificateAcceptor::SSL_CertificateAcceptor ()
      {
      }

    SSL_CertificateAcceptor::~SSL_CertificateAcceptor ()
      {
      }

    void SSL_CertificateAcceptor::handle_certificate_failure (SSL_CertificateCallbackArg& arg)
      {
        INET_DEBUG (3, (LM_INFO, DLINFO
                        ACE_TEXT ("SSL_CertificateAcceptor::handle_certificate_failure - ")
                        ACE_TEXT ("ignored certificate verification error: %C\n"),
                        arg.error_message ().c_str ()));
        arg.ignore_error (true);
      }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
