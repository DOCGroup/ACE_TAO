// $Id$

/**
 * @file SSL_CertificateCallback.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_SSL_CERTIFICATECALLBACK_H
#define ACE_SSL_CERTIFICATECALLBACK_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"
#include "ace/SSL/SSL_Context.h"
#include "ace/INet/SSL_X509Cert.h"
#include "ace/INet/INet_SSL_Export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {
        /**
        * @class ACE_INet_SSL_CertificateCallbackArg
        *
        * @brief Encapsulates the arguments for an SSL certificate
        *   verification callback.
        *
        */
        class ACE_INET_SSL_Export SSL_CertificateCallbackArg
          {
            public:
              SSL_CertificateCallbackArg (const ACE_SSL_Context* ssl_ctx,
                                          ::X509_STORE_CTX* cert_ctx);
              ~SSL_CertificateCallbackArg ();

              const ACE_SSL_Context& context () const;

              SSL_X509Cert& certificate (void);

              int error_depth () const;

              int error_code () const;

              ACE_CString error_message () const;

              bool ignore_error () const;

              void ignore_error (bool f);

            private:
              const ACE_SSL_Context* ssl_ctx_;
              SSL_X509Cert ssl_cert_;
              int err_depth_;
              int err_code_;
              bool ignore_err_;
          };

        /**
        * @class ACE_INet_SSL_CertificateCallback
        *
        * @brief Abstract base class for SSL certificate
        *   verification callbacks.
        *
        */
        class ACE_INET_SSL_Export SSL_CertificateCallback
          {
            public:
              SSL_CertificateCallback ();
              virtual ~SSL_CertificateCallback ();

              virtual void handle_certificate_failure (SSL_CertificateCallbackArg& arg) = 0;
          };

        /**
        * @class ACE_INet_SSL_CertificateAcceptor
        *
        * @brief Implements an SSL certificate callback that accepts
        *   all peer certificates.
        *
        */
        class ACE_INET_SSL_Export SSL_CertificateAcceptor
          : public SSL_CertificateCallback
          {
            public:
              SSL_CertificateAcceptor ();
              virtual ~SSL_CertificateAcceptor ();

              virtual void handle_certificate_failure (SSL_CertificateCallbackArg& arg);
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/SSL_CertificateCallback.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_SSL_CERTIFICATECALLBACK_H */
