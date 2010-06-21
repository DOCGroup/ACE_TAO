// $Id$

#include "ace/INet/HTTP_ClientRequestHandler.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/HTTP_ClientRequestHandler.inl"
#endif

#include "ace/Auto_Ptr.h"

ACE_RCSID(NET_CLIENT,ACE_HTTP_ClientRequestHandler,"$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    ClientRequestHandler::SessionHolder::SessionHolder ()
      : session_ (true)
      {
      }

    ClientRequestHandler::SessionHolder::~SessionHolder()
      {
      }

    ClientRequestHandler::SessionFactory::SessionFactory ()
      {
      }

    ClientRequestHandler::SessionFactory::~SessionFactory ()
      {
      }

    ACE::INet::ConnectionHolder*
    ClientRequestHandler::SessionFactory::create_connection (
        const ACE::INet::ConnectionKey& key) const
      {
        ACE_TRACE ("ClientRequestHandler::SessionFactory::create_connection");

        const INetConnectionKey& ikey = dynamic_cast<const INetConnectionKey&> (key);

        SessionHolder* session_holder = 0;
        ACE_NEW_RETURN (session_holder,
                        SessionHolder (),
                        0);
        ACE_Auto_Ptr<SessionHolder> session_safe_ref (session_holder);

        (*session_holder)->set_host (ikey.host (), ikey.port ());

        if ((*session_holder)->connect (true))
          {
            return session_safe_ref.release ();
          }

        return 0;
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
        if (this->initialize_connection (http_url.has_proxy () ?
                                            http_url.get_proxy_host () :
                                            http_url.get_host (),
                                         http_url.has_proxy () ?
                                            http_url.get_proxy_port () :
                                            http_url.get_port ()))
          {
            this->request_.reset (Request::HTTP_GET,
                                  http_url.get_request_uri (),
                                  this->request_.get_version ());

            this->response_.reset ();

            this->initialize_request (http_url, this->request_);

            if (this->session ()->send_request (this->request_) &&
                this->session ()->receive_response (this->response_))
              {
                this->handle_response (http_url, this->response_);
              }
            else
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

    bool ClientRequestHandler::initialize_connection (const ACE_CString& host,
                                                      u_short port)
      {
        static const SessionFactory session_factory;

        ACE::INet::ConnectionHolder* pch = 0;
        if (this->connection_cache ().claim_connection (INetConnectionKey (host, port),
                                                        pch,
                                                        session_factory))
          {
            this->session (dynamic_cast<SessionHolder*> (pch));
            return true;
          }
        else
          return false;
      }

    void ClientRequestHandler::initialize_request (const URL& /*url*/, Request& /*request*/)
      {
      }

    bool ClientRequestHandler::send_request (std::ostream& /*request_stream*/)
      {
        return true;
      }

    void ClientRequestHandler::handle_response (const URL& /*url*/, const Response& /*response*/)
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
            this->connection_cache ().release_connection (
                INetConnectionKey (this->session ()->get_host (),
                                   this->session ()->get_port ()),
                this->session_);
            this->session_ = 0;
          }
      }

    void ClientRequestHandler::close_connection ()
      {
        if (this->session_)
          {
            this->connection_cache ().close_connection (
                INetConnectionKey (this->session ()->get_host (),
                                   this->session ()->get_port ()),
                this->session_);
            this->session_ = 0;
          }
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
