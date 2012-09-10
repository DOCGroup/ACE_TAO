// $Id$

#include "ace/INet/HTTP_BasicAuthentication.h"
#include "ace/Codecs.h"
#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/HTTP_BasicAuthentication.inl"
#endif



ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    const char* BasicAuthentication::SCHEME = "Basic";

    BasicAuthentication::BasicAuthentication()
      {
      }

    BasicAuthentication::BasicAuthentication(const ACE_CString& user, const ACE_CString& passwd)
      : user_ (user), passwd_ (passwd)
      {
      }

    BasicAuthentication::BasicAuthentication(const Request& request)
      {
        if (request.has_credentials ())
        {
          ACE_CString scheme;
          ACE_CString info;
          request.get_credentials (scheme, info);
          if (scheme == SCHEME)
            {
              size_t out_len = 0;
              ACE_Auto_Array_Ptr<ACE_Byte> safe_buf (ACE_Base64::decode ((const ACE_Byte*)info.c_str (),
                                                                         &out_len));
              ACE_CString credentials ((char*)safe_buf.get (), out_len);
              ACE_CString::size_type pos = credentials.find (':');
              if (pos != ACE_CString::npos)
                {
                  this->user_ = credentials.substr (0, pos);
                  this->passwd_ = credentials.substr (pos+1);
                }
            }
        }
      }

    BasicAuthentication::~BasicAuthentication()
      {
      }

    void BasicAuthentication::set_credentials (Request& request) const
      {
        ACE_CString credentials (this->user_);
        credentials += ':';
        credentials += this->passwd_;
        size_t out_len = 0;
        ACE_Auto_Array_Ptr<ACE_Byte> safe_buf (
            ACE_Base64::encode ((const ACE_Byte*)credentials.c_str (),
                                credentials.length (),
                                &out_len,
                                false));
        ACE_CString enc_cred ((char*)safe_buf.get (), out_len);
        request.set_credentials (SCHEME, enc_cred);
      }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
