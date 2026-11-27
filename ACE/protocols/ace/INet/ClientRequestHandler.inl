ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {

    ACE_INLINE
    ACE_CString const& ClientINetRequestHandler::INetConnectionKey::host () const
      {
        return this->host_;
      }

    ACE_INLINE
    u_short ClientINetRequestHandler::INetConnectionKey::port () const
      {
        return this->port_;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
