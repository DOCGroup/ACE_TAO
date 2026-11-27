#include "ace/INet/ClientRequestHandler.h"
#include "ace/Functor_String.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/ClientRequestHandler.inl"
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {
    ClientRequestHandler::ClientRequestHandler () = default;

    ClientRequestHandler::~ClientRequestHandler () = default;

    ConnectionCache& ClientRequestHandler::connection_cache ()
      {
        return *TConnectionCacheSingleton::instance ();
      }

    ClientINetRequestHandler::ClientINetRequestHandler () = default;

    ClientINetRequestHandler::~ClientINetRequestHandler () = default;

    ClientINetRequestHandler::INetConnectionKey::INetConnectionKey (
        ACE_CString const& host,
        u_short port)
      : ConnectionKey (),
        host_ (host),
        port_ (port)
      {
      }

    ClientINetRequestHandler::INetConnectionKey::~INetConnectionKey () = default;

    u_long ClientINetRequestHandler::INetConnectionKey::hash () const
      {
        return ACE_Hash<ACE_CString>()(this->host_) + this->port_;
      }

    ConnectionKey* ClientINetRequestHandler::INetConnectionKey::duplicate () const
      {
        ConnectionKey* k {};
        ACE_NEW_RETURN (k,
                        INetConnectionKey (this->host_, this->port_),
                        nullptr);
        return k;
      }

    bool ClientINetRequestHandler::INetConnectionKey::equal (ConnectionKey const& key) const
      {
        try {
          INetConnectionKey const& ikey = dynamic_cast<INetConnectionKey const&> (key);
          return this->host_ == ikey.host_ && this->port_ == ikey.port_;
        }
        catch (...) {
          return false;
        }
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
