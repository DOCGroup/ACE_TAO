// $Id$

#include "ace/INet/HTTPS_Context.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/HTTPS_Context.inl"
#endif

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/INet/INet_Log.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTPS
  {

    int Context::ssl_mode_ =  ACE_SSL_Context::SSLv3;
    bool Context::ssl_strict_ = false;
    bool Context::ssl_once_ = true;
    int Context::ssl_depth_ = 0;
    bool Context::ssl_verify_peer_ = true;

    Context::Context (bool verify_peer,
                      bool strict,
                      bool once,
                      int depth,
                      int ssl_mode,
                      ACE_SSL_Context* ssl_ctx,
                      bool release,
                      ACE::INet::SSL_CallbackManager* ssl_cbmngr)
      : ssl_ctx_ (0)
      {
        if (ssl_ctx == 0)
          {
            ACE_NEW_NORETURN (ssl_ctx, ACE_SSL_Context ());
            release = true;
          }
        if (ssl_ctx != 0)
          {
            if (release)
              {
                this->alloc_safe.reset (ssl_ctx);
              }
            this->ssl_ctx_ = ssl_ctx;

            this->ssl_ctx_->set_mode (ssl_mode);
            if (verify_peer)
              this->ssl_ctx_->set_verify_peer (strict ? 1 : 0,
                                              once ? 1 : 0,
                                              depth);
            if (ssl_cbmngr != 0)
              ssl_cbmngr->initialize_callbacks (this->ssl_ctx_);
            // do this to be sure that these settings have been properly set
            // ACE_SSL_Context does not handle this quite correctly
            ::SSL_CTX_set_verify (this->ssl_ctx_->context (),
                                  this->ssl_ctx_->default_verify_mode (),
                                  this->ssl_ctx_->default_verify_callback ());
            INET_DEBUG (9,(LM_INFO, DLINFO
                                    ACE_TEXT ("HTTPS_Context::ctor - ")
                                    ACE_TEXT ("ssl_mode = [%d], ")
                                    ACE_TEXT ("verify_peer = [%d], ")
                                    ACE_TEXT ("verify_mode = [%d]\n"),
                                    this->ssl_ctx_->get_mode (),
                                    (verify_peer ? 1 : 0),
                                    this->ssl_ctx_->default_verify_mode ()));
          }
      }

    Context::Context (ACE_SSL_Context* ssl_ctx,
                      bool release,
                      ACE::INet::SSL_CallbackManager* ssl_cbmngr)
      : ssl_ctx_ (ssl_ctx)
      {
        if (this->ssl_ctx_ != 0)
          {
            if (release)
              this->alloc_safe.reset (this->ssl_ctx_);

            if (ssl_cbmngr != 0)
              ssl_cbmngr->initialize_callbacks (this->ssl_ctx_);
          }
      }

    Context& Context::instance ()
      {
        return *ACE_Unmanaged_Singleton<Context, ACE_SYNCH::MUTEX>::instance ();
      }

    Context::~Context ()
      {
      }

    bool Context::load_trusted_ca (const char* ca_location)
      {
        ACE_stat stat;
        if (ca_location != 0 && ACE_OS::stat (ca_location, &stat) == 0)
          {
            bool is_dir = ((stat.st_mode & S_IFMT) == S_IFDIR);
            if (this->ssl_ctx_->load_trusted_ca (is_dir ? 0 : ca_location,
                                                 is_dir ? ca_location : 0,
                                                 false) == 0)
              return true;
          }
        else
          {
            INET_ERROR (1, (LM_ERROR, DLINFO
                            ACE_TEXT ("Context::load_trusted_ca - ")
                            ACE_TEXT ("invalid ca_location [%C]\n"),
                            ca_location == 0 ? "(null)" : ca_location));
          }
        return false;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
