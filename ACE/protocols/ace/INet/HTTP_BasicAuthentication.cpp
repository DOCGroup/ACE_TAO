#include "ace/INet/HTTP_BasicAuthentication.h"
#include "ace/Codecs.h"

#include <memory>

#if !defined (__ACE_INLINE__)
#include "ace/INet/HTTP_BasicAuthentication.inl"
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {
    char const * BasicAuthentication::SCHEME = "Basic";

    BasicAuthentication::BasicAuthentication()
      {
      }

    BasicAuthentication::BasicAuthentication(ACE_CString const& user, ACE_CString const& passwd)
      : user_ (user), passwd_ (passwd)
      {
      }

    BasicAuthentication::BasicAuthentication(Request const& request)
      {
        if (request.has_credentials ())
        {
          ACE_CString scheme;
          ACE_CString info;
          request.get_credentials (scheme, info);
          if (scheme == SCHEME)
            {
              size_t out_len = 0;
              std::unique_ptr<ACE_Byte[]> const safe_buf (ACE_Base64::decode (
                reinterpret_cast<ACE_Byte const *> (info.c_str ()),
                std::addressof (out_len)));
              ACE_CString const credentials (reinterpret_cast<char *> (safe_buf.get ()), out_len);
              ACE_CString::size_type const pos = credentials.find (':');
              if (pos != ACE_CString::npos)
                {
                  this->user_ = credentials.substr (0, pos);
                  this->passwd_ = credentials.substr (pos+1);
                }
            }
        }
      }

    BasicAuthentication::~BasicAuthentication() = default;

    void BasicAuthentication::set_credentials (Request& request) const
      {
        ACE_CString credentials (this->user_);
        credentials += ':';
        credentials += this->passwd_;
        size_t out_len = 0;
        std::unique_ptr<ACE_Byte[]> const safe_buf (
            ACE_Base64::encode (reinterpret_cast<ACE_Byte const *> (credentials.c_str ()),
                                credentials.length (),
                                std::addressof (out_len),
                                false));
        ACE_CString const enc_cred (reinterpret_cast<char const *> (safe_buf.get ()), out_len);
        request.set_credentials (SCHEME, enc_cred);
      }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
