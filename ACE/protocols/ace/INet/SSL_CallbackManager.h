// $Id$

/**
 * @file SSL_CallbackManager.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_SSL_CALLBACKMANAGER_H
#define ACE_SSL_CALLBACKMANAGER_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/SSL/SSL_Context.h"
#include "ace/INet/SSL_CertificateCallback.h"
#include "ace/INet/SSL_PasswordCallback.h"
#include "ace/INet/INet_SSL_Export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {
        /**
        * @class ACE_INet_SSL_CallbackManager
        *
        * @brief Implements manager class for configuring and handling
        *   SSL callbacks.
        *
        */
        class ACE_INET_SSL_Export SSL_CallbackManager
          {
            public:
              SSL_CallbackManager ();
              ~SSL_CallbackManager ();

              void initialize_callbacks (ACE_SSL_Context* ssl_ctx = ACE_SSL_Context::instance ());

              const ACE_SSL_Context* context () const;

              void set_certificate_callback (ACE::INet::SSL_CertificateCallback* cb);
              void set_password_callback (ACE::INet::SSL_PasswordCallback* cb);

              static SSL_CallbackManager* instance ();

            private:
              int verify_certificate_callback (SSL_CertificateCallbackArg& arg);
              void passwd_callback (ACE_CString& pwd);

              ACE_SSL_Context* ssl_ctx_;

              typedef ACE_Refcounted_Auto_Ptr<ACE::INet::SSL_CertificateCallback,
                                              ACE_SYNCH::MUTEX>        TCertificateCallback;
              typedef ACE_Refcounted_Auto_Ptr<ACE::INet::SSL_PasswordCallback,
                                              ACE_SYNCH::MUTEX>        TPasswordCallback;

              TCertificateCallback cert_callback_;
              TPasswordCallback passwd_callback_;

              static int verify_certificate_callback (int ok, X509_STORE_CTX* cert_ctx);
              static int passwd_callback (char* buf, int size, int rwflag, void* user_data);

              static int ssl_ctx_mngr_index_;
          };

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/SSL_CallbackManager.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_SSL_CALLBACKMANAGER_H */
