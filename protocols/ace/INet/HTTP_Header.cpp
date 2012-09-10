// $Id$

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

    const ACE_CString Header::HTTP_1_0                   = "HTTP/1.0";
    const ACE_CString Header::HTTP_1_1                   = "HTTP/1.1";
    const ACE_CString Header::CHUNKED_TRANSFER_ENCODING  = "chunked";
    const ACE_CString Header::TRANSFER_ENCODING          = "Transfer-Encoding";
    const ACE_CString Header::CONNECTION                 = "Connection";
    const ACE_CString Header::CONNECTION_KEEP_ALIVE      = "Keep-Alive";
    const ACE_CString Header::CONNECTION_CLOSE           = "Close";

    Header::Header()
      : ACE::INet::HeaderBase (),
        version_ (HTTP_1_0)
      {
      }

    Header::Header(const ACE_CString& version)
      : ACE::INet::HeaderBase (),
        version_ (version)
      {
      }

    Header::~Header()
      {
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
