// $Id$

#include "ace/INet/HTTP_ClientRequestHandler.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/HTTP_ClientRequestHandler.inl"
#endif

#include "ace/INet/INet_Log.h"
#include "ace/Auto_Ptr.h"
#include "ace/Functor_String.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    SessionFactoryRegistry::SessionFactoryRegistry ()
      {
      }

    SessionFactoryRegistry::~SessionFactoryRegistry ()
      {
      }

    void SessionFactoryRegistry::register_session_factory (
        const ACE_CString& scheme,
        SessionFactory* factory)
      {
        if (factory == 0)
          this->factory_map_.unbind (scheme);
        else
          this->factory_map_.rebind (scheme, factory);
      }

    SessionFactory*
    SessionFactoryRegistry::find_session_factory (const ACE_CString& scheme)
      {
        SessionFactory* factory = 0;
        this->factory_map_.find (scheme, factory);
        return factory;
      }

    SessionFactoryRegistry& SessionFactoryRegistry::instance ()
      {
        return *ACE_Singleton<SessionFactoryRegistry, ACE_SYNCH::MUTEX>::instance ();
      }

    SessionHolder::SessionHolder ()
      {
      }

    SessionHolder::~SessionHolder()
      {
      }

    SessionFactory_Impl::SessionHolder_Impl::SessionHolder_Impl ()
      : session_ (true)
      {
      }

    SessionFactory_Impl::SessionHolder_Impl::~SessionHolder_Impl()
      {
      }

    SessionBase& SessionFactory_Impl::SessionHolder_Impl::session ()
      {
        return this->session_;
      }

    SessionFactory_Impl& SessionFactory_Impl::factory_ =
        *ACE_Singleton<SessionFactory_Impl,ACE_SYNCH::NULL_MUTEX>::instance ();

    SessionFactory_Impl::SessionFactory_Impl ()
      {
        INET_DEBUG (6, (LM_INFO, DLINFO
                                 ACE_TEXT ("HTTP_SessionFactory_Impl::ctor - ")
                                 ACE_TEXT ("registering session factory for scheme [%C]\n"),
                                 URL::protocol ().c_str ()));
        SessionFactoryRegistry::instance ().register_session_factory (URL::protocol (), this);
      }

    SessionFactory_Impl::~SessionFactory_Impl ()
      {
      }

    ACE::INet::ConnectionHolder*
    SessionFactory_Impl::create_connection (
        const ACE::INet::ConnectionKey& key) const
      {
        INET_TRACE ("HTTP_SessionFactory_Impl::create_connection");

        const ClientRequestHandler::HttpConnectionKey& ikey =
            dynamic_cast<const ClientRequestHandler::HttpConnectionKey&> (key);

        SessionHolder_Impl* session_holder = 0;
        ACE_NEW_RETURN (session_holder,
                        SessionHolder_Impl (),
                        0);
        ACE_Auto_Ptr<SessionHolder_Impl> session_safe_ref (session_holder);

        (*session_holder)->set_host (ikey.host (), ikey.port ());
        if (ikey.is_proxy_connection ())
          {
            (*session_holder)->set_proxy_target (ikey.proxy_target_host (),
                                                 ikey.proxy_target_port ());
          }

        if ((*session_holder)->connect (true))
          {
            return session_safe_ref.release ();
          }

        return 0;
      }

    ClientRequestHandler::HttpConnectionKey::HttpConnectionKey (
        const ACE_CString& host,
        u_short port)
      : INetConnectionKey (host, port),
        proxy_connection_ (false),
        proxy_target_port_ (0)
      {
      }

    ClientRequestHandler::HttpConnectionKey::HttpConnectionKey (
        const ACE_CString& proxy_host,
        u_short proxy_port,
        const ACE_CString& host,
        u_short port)
      : INetConnectionKey (proxy_host, proxy_port),
        proxy_connection_ (true),
        proxy_target_host_ (host),
        proxy_target_port_ (port)
      {
      }

    ClientRequestHandler::HttpConnectionKey::~HttpConnectionKey()
      {
      }

    u_long ClientRequestHandler::HttpConnectionKey::hash () const
      {
        if (this->proxy_connection_)
          return ACE_Hash<ACE_CString>()(this->proxy_target_host_) +
                this->proxy_target_port_ +
                (this->proxy_connection_ ? 1 : 0);
        else
          return INetConnectionKey::hash () +
                (this->proxy_connection_ ? 1 : 0);
      }

    ACE::INet::ConnectionKey* ClientRequestHandler::HttpConnectionKey::duplicate () const
      {
        ACE::INet::ConnectionKey* k = 0;
        if (this->proxy_connection_)
          {
            ACE_NEW_RETURN (k,
                            HttpConnectionKey (this->host (), this->port (),
                                               this->proxy_target_host_,
                                               this->proxy_target_port_),
                            0);
          }
        else
          {
            ACE_NEW_RETURN (k,
                            HttpConnectionKey (this->host (), this->port ()),
                            0);
          }
        return k;
      }

    bool ClientRequestHandler::HttpConnectionKey::equal (const ACE::INet::ConnectionKey& key) const
      {
        try {
          const HttpConnectionKey& http_key = dynamic_cast<const HttpConnectionKey&> (key);
          return (INetConnectionKey::equal (key) &&
                    this->proxy_connection_ == http_key.is_proxy_connection () &&
                    (!this->proxy_connection_ ||
                     (this->proxy_target_host_ == http_key.proxy_target_host () &&
                      this->proxy_target_port_ == http_key.proxy_target_port ())));
        }
        catch (...) {
          return false;
        }
      }

    ClientRequestHandler::ClientRequestHandler ()
      : request_ (Request::HTTP_1_0),
        session_ (0)
      {
      }

    ClientRequestHandler::~ClientRequestHandler ()
      {
        this->release_connection ();
      }

    bool ClientRequestHandler::is_response_ok () const
      {
        return this->response_.get_status ().is_ok () &&
                !const_cast<ClientRequestHandler*> (this)->response_stream ().bad ();
      }

    std::istream& ClientRequestHandler::handle_open_request (
        const ACE::INet::URL_Base& url)
      {
        const URL& http_url = dynamic_cast<const URL&> (url);
        return this->handle_get_request (http_url);
      }

    std::istream& ClientRequestHandler::handle_get_request (
        const URL& http_url)
      {
        bool connected = false;
        if (http_url.has_proxy ())
          connected = this->initialize_connection (http_url.get_scheme (),
                                                   http_url.get_host(),
                                                   http_url.get_port (),
                                                   true,
                                                   http_url.get_proxy_host(),
                                                   http_url.get_proxy_port ());
        else
          connected = this->initialize_connection (http_url.get_scheme (),
                                                   http_url.get_host(),
                                                   http_url.get_port ());

        if (connected)
          {
            this->request_.reset (Request::HTTP_GET,
                                  http_url.get_request_uri (),
                                  this->request_.get_version ());

            this->response_.reset ();

            this->initialize_request (http_url, this->request_);

            if (!this->session ()->send_request (this->request_) ||
                !this->session ()->receive_response (this->response_))
              {
                this->close_connection ();

                this->handle_request_error(http_url);
              }
          }
        else
          {
            this->handle_connection_error (http_url);
          }

        return this->response_stream ();
      }

    void ClientRequestHandler::on_eof ()
      {
        this->release_connection ();
      }

    bool ClientRequestHandler::initialize_connection (const ACE_CString& scheme,
                                                      const ACE_CString& host,
                                                      u_short port,
                                                      bool proxy_conn,
                                                      const ACE_CString& proxy_host,
                                                      u_short proxy_port)
      {
        SessionFactory* session_factory =
          SessionFactoryRegistry::instance ().find_session_factory (scheme);

        if (session_factory == 0)
          {
            INET_ERROR (1, (LM_ERROR, DLINFO
                            ACE_TEXT ("ClientRequestHandler::initialize_connection - ")
                            ACE_TEXT ("unable to find session factory for scheme [%C]\n"),
                            scheme.c_str ()));
            return false;
          }

        ACE::INet::ConnectionHolder* pch = 0;
        if (proxy_conn)
          {
            if (!this->connection_cache ().claim_connection (HttpConnectionKey (proxy_host,
                                                                                proxy_port,
                                                                                host,
                                                                                port),
                                                            pch,
                                                            *session_factory))
              return false;
          }
        else
          {
            if (!this->connection_cache ().claim_connection (HttpConnectionKey (host,
                                                                                port),
                                                            pch,
                                                            *session_factory))
              return false;
          }

        this->session (dynamic_cast<SessionHolder*> (pch));
        return true;
      }

    void ClientRequestHandler::initialize_request (const URL& /*url*/, Request& /*request*/)
      {
      }

    void ClientRequestHandler::handle_request_error (const URL& /*url*/)
      {
      }

    void ClientRequestHandler::handle_connection_error (const URL& /*url*/)
      {
      }

    void ClientRequestHandler::release_connection ()
      {
        if (this->session_)
          {
            if (this->session ()->is_proxy_connection ())
              {
                this->connection_cache ().release_connection (
                    HttpConnectionKey (this->session ()->get_host (),
                                       this->session ()->get_port (),
                                       this->session ()->get_proxy_target_host (),
                                       this->session ()->get_proxy_target_port ()),
                    this->session_);
              }
            else
              {
                this->connection_cache ().release_connection (
                    HttpConnectionKey (this->session ()->get_host (),
                                       this->session ()->get_port ()),
                    this->session_);
              }
            this->session_ = 0;
          }
      }

    void ClientRequestHandler::close_connection ()
      {
        if (this->session_)
          {
            if (this->session ()->is_proxy_connection ())
              {
                this->connection_cache ().release_connection (
                    HttpConnectionKey (this->session ()->get_host (),
                                       this->session ()->get_port (),
                                       this->session ()->get_proxy_target_host (),
                                       this->session ()->get_proxy_target_port ()),
                    this->session_);
              }
            else
              {
                this->connection_cache ().release_connection (
                    HttpConnectionKey (this->session ()->get_host (),
                                       this->session ()->get_port ()),
                    this->session_);
              }
            this->session_ = 0;
          }
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
