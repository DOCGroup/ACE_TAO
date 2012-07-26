// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTPS
  {

    ACE_INLINE
    Context::operator bool (void) const
      {
        return this->ssl_ctx_ != 0;
      }

    ACE_INLINE
    bool Context::operator ! (void) const
      {
        return this->ssl_ctx_ == 0;
      }

    ACE_INLINE
    ACE_SSL_Context& Context::ssl_context (void)
      {
        return *this->ssl_ctx_;
      }

    ACE_INLINE
    const ACE_SSL_Context& Context::ssl_context (void) const
      {
        return *this->ssl_ctx_;
      }

    ACE_INLINE
    bool Context::use_default_ca ()
      {
        if (::SSL_CTX_set_default_verify_paths(this->ssl_ctx_->context ()) != 1)
          {
            ACE_SSL_Context::report_error ();
            return false;
          }
        return true;
      }

    ACE_INLINE
    bool Context::set_key_files (const char* certificate_filename,
                                 const char* private_key_filename,
                                 int file_type)
      {
        if (this->ssl_ctx_->certificate (certificate_filename, file_type) == 0)
          {
            if (this->ssl_ctx_->private_key (private_key_filename, file_type) == 0)
              {
                return true;
              }
          }
        return false;
      }

    ACE_INLINE
    int Context::has_trusted_ca ()
      {
        return this->ssl_ctx_->have_trusted_ca ();
      }

    ACE_INLINE
    void Context::set_default_ssl_mode (int ssl_mode)
      {
        Context::ssl_mode_ = ssl_mode;
      }

    ACE_INLINE
    void Context::set_default_verify_mode (bool verify_peer)
      {
        Context::ssl_verify_peer_ = verify_peer;
      }

    ACE_INLINE
    void Context::set_default_verify_settings (bool strict,
                                                bool once,
                                                int depth)
      {
        Context::ssl_strict_ = strict;
        Context::ssl_once_ = once;
        Context::ssl_depth_ = depth;
      }


  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
