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
    ClientRequestHandler::ClientRequestHandler () {}

    ClientRequestHandler::~ClientRequestHandler () = default;

    ConnectionCache& ClientRequestHandler::connection_cache ()
      {
        return *TConnectionCacheSingleton::instance ();
      }

    ClientINetRequestHandler::ClientINetRequestHandler () {}

    ClientINetRequestHandler::~ClientINetRequestHandler () = default;

    ClientINetRequestHandler::INetConnectionKey::INetConnectionKey (
        const ACE_CString& host,
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
        ConnectionKey* k = nullptr;
        ACE_NEW_RETURN (k,
                        INetConnectionKey (this->host_, this->port_),
                        0);
        return k;
      }

    bool ClientINetRequestHandler::INetConnectionKey::equal (const ConnectionKey& key) const
      {
        try {
          const INetConnectionKey& ikey = dynamic_cast<const INetConnectionKey&> (key);
          return this->host_ == ikey.host_ && this->port_ == ikey.port_;
        }
        catch (...) {
          return false;
        }
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
