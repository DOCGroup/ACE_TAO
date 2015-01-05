// $Id$

#ifndef ACE_HTTPS_SESSION_CPP
#define ACE_HTTPS_SESSION_CPP

#include "ace/INet/HTTPS_Session.h"
#include "ace/INet/INet_Log.h"
#include "ace/INet/IOS_util.h"
#include "ace/INet/HTTPS_URL.h"
#include "ace/INet/Sock_IOStream.h"
#include "ace/INet/String_IOStream.h"
#include "ace/INet/SSL_Proxy_Connector.h"
#include "ace/INET_Addr.h"
#include "ace/Event_Handler.h"
#include "ace/Connector.h"
#include "ace/SSL/SSL_SOCK_Connector.h"
#include "ace/String_Base.h"
#include <istream>
#include <ostream>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTPS
  {

    template <ACE_SYNCH_DECL>
    Session_T<ACE_SYNCH_USE>::Session_T (bool keep_alive
#if defined (SSL_HAS_SSL_set_SSL_CTX) && (SSL_HAS_SSL_set_SSL_CTX == 1)
                                         , Context* ctx
#endif
                                         )
      : SessionBase (URL::HTTPS_PORT, keep_alive),
        connection_ (0),
        sock_stream_ (0)
#if defined (SSL_HAS_SSL_set_SSL_CTX) && (SSL_HAS_SSL_set_SSL_CTX == 1)
        , context_ (ctx)
#endif
      {
        INET_TRACE ("ACE_HTTPS_Session - ctor");
      }

    template <ACE_SYNCH_DECL>
    Session_T<ACE_SYNCH_USE>::Session_T (const ACE_Time_Value& timeout,
                                         bool keep_alive,
                                         const ACE_Time_Value* alive_timeout
#if defined (SSL_HAS_SSL_set_SSL_CTX) && (SSL_HAS_SSL_set_SSL_CTX == 1)
                                         , Context* ctx
#endif
                                         )
      : SessionBase (URL::HTTPS_PORT, timeout, keep_alive, alive_timeout),
        connection_ (0),
        sock_stream_ (0)
#if defined (SSL_HAS_SSL_set_SSL_CTX) && (SSL_HAS_SSL_set_SSL_CTX == 1)
        , context_ (ctx)
#endif
      {
        INET_TRACE ("ACE_HTTPS_Session - ctor");
        this->close_streams ();
        this->close_connection ();
      }

    template <ACE_SYNCH_DECL>
    Session_T<ACE_SYNCH_USE>::~Session_T ()
      {
        INET_TRACE ("ACE_HTTPS_Session - dtor");
      }

    template <ACE_SYNCH_DECL>
    bool Session_T<ACE_SYNCH_USE>::is_connected () const
      {
        return this->connection_ && this->connection_->is_connected ();
      }

    template <ACE_SYNCH_DECL>
    bool Session_T<ACE_SYNCH_USE>::connect_i (const ACE_Synch_Options& sync_opt)
      {
        INET_TRACE ("ACE_HTTPS_Session::connect_i");

        connection_type* new_connection = 0;

        if (this->is_proxy_connection ())
          {
            typedef ACE::IOS::StreamHandler<ACE_SOCK_STREAM, ACE_SYNCH_USE> proxy_connection_type;
            typedef ACE_Connector<proxy_connection_type, ACE_SOCK_CONNECTOR> proxy_connector_type;
            typedef ACE::IOS::Sock_IOStreamBase<ACE_SYNCH_USE> proxy_stream_type;

            proxy_connection_type proxy_connection(sync_opt);
            proxy_connector_type proxy_connector;

            proxy_connection_type* proxy_connection_ptr = &proxy_connection;
            if (proxy_connector.connect (proxy_connection_ptr,
                                         ACE_INET_Addr (this->port_,
                                                        this->host_.c_str ()),
                                         ACE_Synch_Options (0,this->http_timeout_)) == -1)
              {
                INET_ERROR (1, (LM_ERROR, DLINFO
                                ACE_TEXT ("(%d) ACE_HTTPS_Session::connect_i - ")
                                ACE_TEXT ("failed to connect to proxy; host=%C, port=%d\n"),
                                ACE_OS::last_error (), this->host_.c_str (), this->port_));
                return false;
              }

            proxy_stream_type proxy_stream (&proxy_connection);
            ACE::IOS::CString_OStream target_address;
            target_address << this->proxy_target_host_ << ':' << this->proxy_target_port_;
            ACE::HTTP::Request connect_request (ACE::HTTP::Request::HTTP_CONNECT,
                                                target_address.str ().c_str (),
                                                ACE::HTTP::Request::HTTP_1_1);
            connect_request.set("Proxy-Connection", "keep-alive");
            connect_request.set_host(this->proxy_target_host_);
            ACE::HTTP::Response connect_response;

            connect_request.write (proxy_stream);
            proxy_stream.flush ();
            if (!connect_response.read (proxy_stream) ||
                      !connect_response.get_status ().is_ok ())
              {
                INET_ERROR (1, (LM_ERROR, DLINFO
                                ACE_TEXT ("ACE_HTTPS_Session::connect_i - ")
                                ACE_TEXT ("cannot setup proxy tunnel; proxy replied: %d:%C\n"),
                                connect_response.get_status ().get_status(),
                                connect_response.get_status ().get_reason().c_str ()));
                return false;
              }

            ACE_NEW_RETURN (new_connection,
                            connection_type(sync_opt),
                            false);
#if defined (SSL_HAS_SSL_set_SSL_CTX) && (SSL_HAS_SSL_set_SSL_CTX == 1)
            // set the provided SSL context for the SSL structure of the SSL_SOCK_Stream
            if (this->context_ != 0)
              {
                ::SSL * ssl_ptr = new_connection->peer ().ssl ();
                ::SSL_set_SSL_CTX (ssl_ptr, this->context_->ssl_context ().context ());
              }
#endif
            ACE_HANDLE proxy_conn_handle = proxy_connection.peer ().get_handle ();
            proxy_connection.peer ().set_handle (ACE_INVALID_HANDLE);

            ACE::INet::SSL_Proxy_Connector proxy_ssl_connector;
            ACE_Time_Value conn_timeout (this->http_timeout_);
            if (proxy_ssl_connector.connect (new_connection->peer (),
                                             proxy_conn_handle,
                                             &conn_timeout) != 0)
              {
                INET_ERROR (1, (LM_ERROR, DLINFO
                                ACE_TEXT ("ACE_HTTPS_Session::connect_i - ")
                                ACE_TEXT ("failed to setup proxy SSL connection\n")));
                return false;
              }
            new_connection->open (); // mark stream handler as connected
          }
        else
          {
            ACE_NEW_RETURN (new_connection,
                            connection_type(sync_opt),
                            false);
#if defined (SSL_HAS_SSL_set_SSL_CTX) && (SSL_HAS_SSL_set_SSL_CTX == 1)
            // set the provided SSL context for the SSL structure of the SSL_SOCK_Stream
            if (this->context_ != 0)
              {
                ::SSL * ssl_ptr = new_connection->peer ().ssl ();
                ::SSL_set_SSL_CTX (ssl_ptr, this->context_->ssl_context ().context ());
              }
#endif
            typedef ACE_Connector<connection_type, ACE_SSL_SOCK_Connector> connector_type;

            connector_type connector;

            if (connector.connect (new_connection,
                                  ACE_INET_Addr (this->port_,
                                                  this->host_.c_str ()),
                                  ACE_Synch_Options (0,this->http_timeout_)) == -1)
              {
                INET_ERROR (1, (LM_ERROR, DLINFO
                                ACE_TEXT ("(%d) ACE_HTTPS_Session::connect_i - ")
                                ACE_TEXT ("failed to connect; host=%C, port=%d\n"),
                                ACE_OS::last_error (), this->host_.c_str (), this->port_));
                // as the connection was dynamically allocated
                // the connector causes it to be destroyed after
                // the connection failure
                return false;
              }
          }

        this->connection_ = new_connection;
        this->connection_->reference_counting_policy ().value (
            ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

        ACE_NEW_NORETURN (this->sock_stream_,
                          sock_stream_type (this->connection_));
        if (this->sock_stream_)
          {
            this->cannot_reconnect_ = false;
            this->reactive_ = sync_opt[ACE_Synch_Options::USE_REACTOR];

            // reset reconnect timer
            this->reconnect_timer_ = this->keep_alive_timeout_;
            this->reconnect_countdown_.start ();

            return true;
          }
        else
          {
            this->close ();
            return false;
          }
      }

    template <ACE_SYNCH_DECL>
    bool Session_T<ACE_SYNCH_USE>::attach_connection (connection_type* connection)
      {
        INET_TRACE ("ACE_HTTPS_Session::attach_connection");

        if (!connection->is_connected ())
          return false;

        this->close ();

        ACE_INET_Addr remote;
        connection->peer ().get_remote_addr (remote);
        this->host_ = remote.get_host_name ();
        this->port_ = remote.get_port_number ();

        this->connection_ = connection;
        this->connection_->add_reference ();

        ACE_NEW_NORETURN (this->sock_stream_,
                          sock_stream_type (this->connection_));

        if (this->sock_stream_)
          {
            this->keep_alive_ = true;
            this->keep_alive_timeout_ = ACE_Time_Value::zero;
            this->cannot_reconnect_ = true;
            return true;
          }
        else
          {
            this->close ();
            return false;
          }
      }

    template <ACE_SYNCH_DECL>
    void Session_T<ACE_SYNCH_USE>::close_connection ()
      {
        if (this->sock_stream_)
          {
            delete this->sock_stream_;
            this->sock_stream_ = 0;
          }

        if (this->connection_)
          {
            // this should be the last referece and removing it
            // causes the connection to be destroyed
            this->connection_->remove_reference ();
            this->connection_ = 0;
          }
      }

    template <ACE_SYNCH_DECL>
    void Session_T<ACE_SYNCH_USE>::close_i ()
      {
        INET_TRACE ("ACE_HTTPS_Session::close_i");

        this->close_connection ();
      }

    template <ACE_SYNCH_DECL>
    std::iostream& Session_T<ACE_SYNCH_USE>::sock_stream ()
      {
        return *this->sock_stream_;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HTTPS_SESSION_CPP */
