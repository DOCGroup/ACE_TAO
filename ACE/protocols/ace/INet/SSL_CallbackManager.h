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
        // NOTE: Solaris studio compilers amongst others will issue warnings if the
        // the correct type of function pointer (i.e. extern "C" ) is not stored/used
        // of the form:
        //   Warning (Anachronism): Formal argument 1 of type
        //     extern "C" int(*)(int,x509_store_ctx_st*) in call to
        //     ACE_SSL_Context::default_verify_callback(extern "C" int(*)(int,x509_store_ctx_st*))
        //     is being passed int(*)(int,x509_store_ctx_st*).
        // and
        //   Warning (Anachronism): Formal argument cb of type
        //     extern "C" int(*)(char*,int,int,void*) in call to
        //     SSL_CTX_set_default_passwd_cb(ssl_ctx_st*, extern "C" int(*)(char*,int,int,void*))
        //     is being passed int(*)(char*,int,int,void*).
        // when C library routines are passed CallBack functions pointers that are
        // actually C++ functions. (Static class member functions are NOTE extern "C" by default.)
        //
        // Unfortunatly you can not specify extern "C" linkage anywhere inside a class
        // declaration or inside a function prototype for individual parameters. We are therefore
        // forced to declare friend functions external to the class to do this job (and of course
        // the function names can not be overloaded as they are C functions). They can not be
        // static functions to make them private to the cpp file as they need to be seen by the
        // class declaration and therefore included in the header file) thus:
        extern "C" {
          int extern_C_verify_certificate_callback (int ok, X509_STORE_CTX* cert_ctx);
          int extern_C_passwd_callback (char* buf, int size, int rwflag, void* user_data);
        }

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

              friend int extern_C_verify_certificate_callback (int ok, X509_STORE_CTX* cert_ctx);
              friend int extern_C_passwd_callback (char* buf, int size, int rwflag, void* user_data);

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
