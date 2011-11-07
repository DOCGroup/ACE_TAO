// $Id$

/**
 * @file HTTPS_Context.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTPS_CONTEXT_H
#define ACE_HTTPS_CONTEXT_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"
#include "ace/Auto_Ptr.h"
#include "ace/Singleton.h"
#include "ace/SSL/SSL_Context.h"
#include "ace/INet/SSL_CallbackManager.h"
#include "ace/INet/INet_SSL_Export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTPS
      {
        /**
        * @class ACE_HTTPS_Context
        *
        * @brief Implements security (SSL) context for HTTPS sessions.
        *
        */
        class ACE_INET_SSL_Export Context
          {
            public:
              Context (bool verify_peer = Context::ssl_verify_peer_,
                       bool strict = Context::ssl_strict_,
                       bool once = Context::ssl_once_,
                       int depth = Context::ssl_depth_,
                       int ssl_mode = Context::ssl_mode_,
                       ACE_SSL_Context* ssl_ctx =
                          ACE_SSL_Context::instance (),
                       bool release = false,
                       ACE::INet::SSL_CallbackManager* ssl_cbmngr =
                          ACE::INet::SSL_CallbackManager::instance ());

              Context (ACE_SSL_Context* ssl_ctx,
                       bool release = false,
                       ACE::INet::SSL_CallbackManager* ssl_cbmngr = 0);

              ~Context ();

              operator bool (void) const;

              bool operator ! (void) const;

              ACE_SSL_Context& ssl_context (void);

              const ACE_SSL_Context& ssl_context (void) const;

              bool use_default_ca ();

              bool set_key_files (const char* certificate_filename,
                                  const char* private_key_filename,
                                  int file_type = SSL_FILETYPE_PEM);

              bool load_trusted_ca (const char* ca_location);

              int has_trusted_ca ();

              static void set_default_ssl_mode (int ssl_mode);

              static void set_default_verify_mode (bool verify_peer);

              static void set_default_verify_settings (bool strict,
                                                       bool once = true,
                                                       int depth = 0);

              static Context& instance ();

            private:
              friend class ACE_Singleton<Context, ACE_SYNCH::MUTEX>;

              //Context ();
              Context (const Context&);

              ACE_SSL_Context* ssl_ctx_;
              ACE_Auto_Ptr<ACE_SSL_Context> alloc_safe;

              static int ssl_mode_;
              static bool ssl_strict_;
              static bool ssl_once_;
              static int ssl_depth_;
              static bool ssl_verify_peer_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/HTTPS_Context.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_SSL_CALLBACKMANAGER_H */
