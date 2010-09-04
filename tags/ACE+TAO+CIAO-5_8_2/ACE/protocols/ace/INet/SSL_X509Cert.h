// $Id$

/**
 * @file SSL_X509Cert.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_SSL_X509CERT_H
#define ACE_SSL_X509CERT_H

#include /**/ "ace/pre.h"

#include <openssl/x509.h>
#include <openssl/crypto.h>
#include "ace/INet/INet_SSL_Export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {
        /**
        * @class ACE_INet_SSL_X509Cert
        *
        * @brief Encapsulates an SSL X509 certificate object.
        *
        * Provides reference counting for the X509 certificate object.
        */
        class ACE_INET_SSL_Export SSL_X509Cert
          {
            public:
              SSL_X509Cert ();
              SSL_X509Cert (::X509* ssl_cert);
              SSL_X509Cert (const SSL_X509Cert& cert);
              ~SSL_X509Cert ();

              SSL_X509Cert& operator= (::X509* ssl_cert);
              SSL_X509Cert& operator= (const SSL_X509Cert& ssl_cert);

              ::X509* operator & (void);

            private:
              ::X509* ssl_cert_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/SSL_X509Cert.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_SSL_X509CERT_H */
