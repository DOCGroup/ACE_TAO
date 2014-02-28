// $Id$

#include "ace/INet/SSL_CallbackManager.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/SSL_CallbackManager.inl"
#endif

#include "ace/Truncate.h"
#include "ace/Singleton.h"
#include "ace/INet/INet_Log.h"

#include <openssl/x509.h>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {

    int SSL_CallbackManager::ssl_ctx_mngr_index_ = (-2);

    SSL_CallbackManager::SSL_CallbackManager ()
      {
      }

    SSL_CallbackManager::~SSL_CallbackManager ()
      {
      }

    void SSL_CallbackManager::initialize_callbacks (ACE_SSL_Context* ssl_ctx)
      {
        if (ssl_ctx_mngr_index_ < -1)
          {
            ssl_ctx_mngr_index_ = ::SSL_CTX_get_ex_new_index (0, 0, 0,0,0);
            if (ssl_ctx_mngr_index_ < 0)
              {
                INET_ERROR (1, (LM_ERROR, DLINFO
                                ACE_TEXT ("SSL_CallbackManager::initialize_callbacks - ")
                                ACE_TEXT ("failed to allocate SSL_CTX ex_data index.\n")));
                return;
              }
          }

        this->ssl_ctx_ = ssl_ctx == 0 ? ACE_SSL_Context::instance () : ssl_ctx;
        ::SSL_CTX_set_ex_data (this->ssl_ctx_->context (), ssl_ctx_mngr_index_, this);
        this->ssl_ctx_->default_verify_callback (extern_C_verify_certificate_callback);
        ::SSL_CTX_set_default_passwd_cb (ssl_ctx->context(), extern_C_passwd_callback);
        ::SSL_CTX_set_default_passwd_cb_userdata (ssl_ctx->context(), this);
      }

    SSL_CallbackManager* SSL_CallbackManager::instance ()
      {
        return ACE_Singleton<SSL_CallbackManager, ACE_SYNCH::MUTEX>::instance ();
      }

    int SSL_CallbackManager::verify_certificate_callback (SSL_CertificateCallbackArg& arg)
      {
        TCertificateCallback cert_cb = this->cert_callback_;
        if (cert_cb)
          {
            cert_cb->handle_certificate_failure (arg);
          }
        return (arg.ignore_error () ? 1 : 0);
      }

    void SSL_CallbackManager::passwd_callback (ACE_CString& pwd)
      {
        TPasswordCallback pw_cb = passwd_callback_;
        if (pw_cb)
          {
            pw_cb->get_privatekey_password (pwd);
          }
      }

    int extern_C_verify_certificate_callback (int ok, X509_STORE_CTX* cert_ctx)
      {
        if (!ok && SSL_CallbackManager::ssl_ctx_mngr_index_>=0)
        {
          // Retrieve the pointer to the SSL of the connection currently treated
          void* ex_data = ::X509_STORE_CTX_get_ex_data (cert_ctx, ::SSL_get_ex_data_X509_STORE_CTX_idx());
          ::SSL* ssl = reinterpret_cast< ::SSL* > (ex_data);
          // Retrieve SSL_CTX pointer of the connection currently treated
          ::SSL_CTX* ssl_ctx = ::SSL_get_SSL_CTX (ssl);
          // Retrieve our SSL_CallbackManager
          ex_data = ::SSL_CTX_get_ex_data (ssl_ctx, SSL_CallbackManager::ssl_ctx_mngr_index_);
          SSL_CallbackManager* cbmngr = reinterpret_cast<SSL_CallbackManager*> (ex_data);

          SSL_CertificateCallbackArg arg (cbmngr->context(), cert_ctx);
          ok = cbmngr->verify_certificate_callback (arg);
        }

        return ok;
      }

    int extern_C_passwd_callback (char* buf, int size, int /*rwflag*/, void* user_data)
      {
        if (user_data == 0)
          return 0;

        SSL_CallbackManager* cbmngr = reinterpret_cast<SSL_CallbackManager*> (user_data);

        ACE_CString pwd;
        cbmngr->passwd_callback (pwd);
        if (!pwd.empty ())
          {
            ACE_OS::strncpy (buf, pwd.c_str (), size);
            buf[size - 1] = '\0';
            if (size > ACE_Utils::truncate_cast<int> (pwd.length ()))
              size = ACE_Utils::truncate_cast<int> (pwd.length ());

            return size;
          }
        else
          return 0;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
