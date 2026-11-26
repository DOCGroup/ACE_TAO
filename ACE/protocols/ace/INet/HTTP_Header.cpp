#include "ace/INet/HTTP_Header.h"
#include "ace/OS_NS_stdlib.h"
#include <string>

#if !defined (__ACE_INLINE__)
#include "ace/INet/HTTP_Header.inl"
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {
    ACE_CString const Header::HTTP_1_0                   = "HTTP/1.0";
    ACE_CString const Header::HTTP_1_1                   = "HTTP/1.1";
    ACE_CString const Header::CHUNKED_TRANSFER_ENCODING  = "chunked";
    ACE_CString const Header::TRANSFER_ENCODING          = "Transfer-Encoding";
    ACE_CString const Header::CONNECTION                 = "Connection";
    ACE_CString const Header::CONNECTION_KEEP_ALIVE      = "Keep-Alive";
    ACE_CString const Header::CONNECTION_CLOSE           = "Close";

    Header::Header()
      : ACE::INet::HeaderBase (),
        version_ (HTTP_1_0)
      {
      }

    Header::Header(ACE_CString const& version)
      : ACE::INet::HeaderBase (),
        version_ (version)
      {
      }

    Header::~Header() = default;

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
