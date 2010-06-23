// $Id$

#ifndef ACE_HTTP_SESSION_CPP
#define ACE_HTTP_SESSION_CPP

#include "ace/INet/HTTP_Session.h"
#include "ace/INet/HTTP_StreamPolicy.h"
#include "ace/INet/String_IOStream.h"
#include "ace/INet/IOS_util.h"
#include "ace/INET_Addr.h"
#include "ace/Event_Handler.h"
#include "ace/Connector.h"
#include "ace/String_Base.h"
#include <istream>
#include <ostream>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    template <ACE_SYNCH_DECL>
    Session_T<ACE_SYNCH_USE>::Session_T (bool keep_alive)
      : port_ (HTTP_PORT),
        reactive_ (false),
        connection_ (0),
        sock_stream_ (0),
        in_stream_ (0),
        out_stream_ (0),
        http_timeout_ (DEFAULT_TIMEOUT),
        keep_alive_timeout_ (DEFAULT_KEEP_ALIVE_TIMEOUT),
        reconnect_timer_ (DEFAULT_KEEP_ALIVE_TIMEOUT),
        reconnect_countdown_ (&reconnect_timer_),
        keep_alive_ (keep_alive),
        needs_reconnect_ (false),
        cannot_reconnect_ (false),
        expects_response_body_ (false)
      {
        ACE_TRACE ("ACE_HTTP_Session - ctor");
      }

    template <ACE_SYNCH_DECL>
    Session_T<ACE_SYNCH_USE>::~Session_T ()
      {
        ACE_TRACE ("ACE_HTTP_Session - dtor");
        this->close ();
      }

    template <ACE_SYNCH_DECL>
    void Session_T<ACE_SYNCH_USE>::set_keep_alive (bool f)
      {
        this->keep_alive_ = f;
      }

    template <ACE_SYNCH_DECL>
    bool Session_T<ACE_SYNCH_USE>::keep_alive () const
      {
        return this->keep_alive_;
      }

    template <ACE_SYNCH_DECL>
    bool Session_T<ACE_SYNCH_USE>::is_connected () const
      {
        return this->connection_ && this->connection_->is_connected ();
      }

    template <ACE_SYNCH_DECL>
    void Session_T<ACE_SYNCH_USE>::set_host (const ACE_CString& host, u_short port)
      {
        if (!this->is_connected ())
          {
            this->host_ = host;
            this->port_ = port;
          }
      }

    template <ACE_SYNCH_DECL>
    void Session_T<ACE_SYNCH_USE>::set_host (const ACE_CString& host)
      {
        if (!this->is_connected ())
          {
            this->host_ = host;
          }
      }

    template <ACE_SYNCH_DECL>
    void Session_T<ACE_SYNCH_USE>::set_port (u_short port)
      {
        if (!this->is_connected ())
          {
            this->port_ = port;
          }
      }

    template <ACE_SYNCH_DECL>
    const ACE_CString& Session_T<ACE_SYNCH_USE>::get_host () const
      {
        return this->host_;
      }

    template <ACE_SYNCH_DECL>
    u_short Session_T<ACE_SYNCH_USE>::get_port () const
      {
        return this->port_;
      }

    template <ACE_SYNCH_DECL>
    bool Session_T<ACE_SYNCH_USE>::connect (bool use_reactor)
      {
        ACE_TRACE ("ACE_HTTP_Session::connect");

        typedef ACE_Connector<connection_type, ACE_SOCK_CONNECTOR> connector_type;

        this->close ();

        unsigned long f_reactor = use_reactor ? ACE_Synch_Options::USE_REACTOR : 0;
        ACE_Synch_Options sync_opt (ACE_Synch_Options::USE_TIMEOUT | f_reactor,
                                    this->http_timeout_);
        connector_type connector;

        connection_type* new_connection = 0;
        ACE_NEW_RETURN (new_connection,
                        connection_type(sync_opt),
                        false);
        if (connector.connect (new_connection,
                               ACE_INET_Addr (this->port_,
                                              this->host_.c_str ()),
                               ACE_Synch_Options (0,this->http_timeout_)) == -1)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) (%d) ACE_HTTP_Session::connect - ")
                                  ACE_TEXT ("failed to connect; host=%C, port=%d"),
                                  ACE_OS::last_error (), this->host_.c_str (), this->port_));
            // as the connection was dynamically allocated
            // the connector causes it to be destroyed after
            // the connection failure
            return false;
          }

        this->connection_ = new_connection;
        this->connection_->reference_counting_policy ().value (
            ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

        ACE_NEW_NORETURN (this->sock_stream_,
                          sock_stream_type (this->connection_));
        if (this->sock_stream_)
          {
            this->cannot_reconnect_ = false;
            this->reactive_ = use_reactor;

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
    bool Session_T<ACE_SYNCH_USE>::connect (connection_type* connection)
      {
        ACE_TRACE ("ACE_HTTP_Session::connect(connection)");

        this->close ();

        if (connection->is_connected ())
          {
            ACE_INET_Addr remote;
            connection->peer ().get_remote_addr (remote);
            this->host_ = remote.get_host_name ();
            this->port_ = remote.get_port_number ();
          }
        else
          {
            typedef ACE_Connector<connection_type, ACE_SOCK_CONNECTOR> connector_type;

            connector_type connector;
            if (connector.connect (connection,
                                   ACE_INET_Addr (this->host_.c_str (),
                                                  this->port_)) == -1)
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) (%d) ACE_HTTP_Session::connect(connection) - ")
                                      ACE_TEXT ("failed to connect; host=%C, port=%d"),
                                      ACE_OS::last_error (), this->host_.c_str (), this->port_));
                return false;
              }
          }

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
    std::ostream& Session_T<ACE_SYNCH_USE>::send_request (Request& request)
      {
        ACE_TRACE ("ACE_HTTP_Session::send_request");

        if (this->in_stream_)
          {
            delete this->in_stream_;
            this->in_stream_ = 0;
          }

        bool keep_alive = this->keep_alive ();
        if ((this->is_connected () && !keep_alive) || this->reconnect_needed ())
        {
          close();
          this->needs_reconnect_ = false;
        }

        if (this->out_stream_)
          {
            delete this->out_stream_;
            this->out_stream_ = 0;
          }

        if (!this->is_connected ())
          {
            if (this->cannot_reconnect_ || !this->connect(this->reactive_))
              {
                if (!this->cannot_reconnect_)
                  ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) (%d) HTTP_Session::send_request - ")
                                        ACE_TEXT ("reconnect failed\n"),
                                        ACE_OS::last_error ()));
                return ACE::IOS::Null::out_stream_;
              }
          }
        if (!keep_alive)
          request.set_keep_alive (false);
        if (!request.has_host ())
          {
            if (this->port_ == HTTP_PORT)
              request.set_host (this->host_);
            else
              request.set_host (this->host_, this->port_);
          }

        this->expects_response_body_ = request.get_method() != Request::HTTP_HEAD;

        if (request.has_chunked_transfer_encoding ())
        {
          request.write (*this->sock_stream_);
          ChunkedTransferStreamPolicy* pol;
          ACE_NEW_RETURN (pol,
                          ChunkedTransferStreamPolicy (),
                          ACE::IOS::Null::out_stream_);
          ACE_NEW_RETURN (this->out_stream_,
                          OStream (*this->sock_stream_, pol),
                          ACE::IOS::Null::out_stream_);
        }
        else if (request.get_content_length () != Header::UNKNOWN_CONTENT_LENGTH)
        {
          ACE::IOS::CString_OStream cs;
          request.write (cs);
          FixedLengthStreamPolicy* pol;
          ACE_NEW_RETURN (pol,
                          FixedLengthStreamPolicy (cs.str ().length () + request.get_content_length ()),
                          ACE::IOS::Null::out_stream_);
          ACE_NEW_RETURN (this->out_stream_,
                          OStream (*this->sock_stream_, pol),
                          ACE::IOS::Null::out_stream_);
          (*this->out_stream_) << cs.str ().c_str ();
        }
        else if (request.get_method () != Request::HTTP_PUT && request.get_method() != Request::HTTP_POST)
        {
          ACE::IOS::CString_OStream cs;
          request.write (cs);
          FixedLengthStreamPolicy* pol;
          ACE_NEW_RETURN (pol,
                          FixedLengthStreamPolicy (cs.str ().length ()),
                          ACE::IOS::Null::out_stream_);
          ACE_NEW_RETURN (this->out_stream_,
                          OStream (*this->sock_stream_, pol),
                          ACE::IOS::Null::out_stream_);
          (*this->out_stream_) << cs.str ().c_str ();
        }
        else
        {
          ACE_NEW_RETURN (this->out_stream_,
                          OStream (*this->sock_stream_),
                          ACE::IOS::Null::out_stream_);
          request.write (*this->out_stream_);
        }
        // reset reconnect timer
        this->reconnect_timer_ = this->keep_alive_timeout_;
        this->reconnect_countdown_.start ();

        return *this->out_stream_;
      }

    template <ACE_SYNCH_DECL>
    std::ostream& Session_T<ACE_SYNCH_USE>::request_stream ()
      {
        return this->out_stream_ ? *this->out_stream_ : ACE::IOS::Null::out_stream_;
      }

    template <ACE_SYNCH_DECL>
    std::ostream& Session_T<ACE_SYNCH_USE>::request_stream (
        ACE::IOS::StreamInterceptor& interceptor)
      {
        if (this->out_stream_)
          {
            this->out_stream_->set_interceptor (interceptor);
            return *this->out_stream_;
          }
        else
          return ACE::IOS::Null::out_stream_;
      }

    template <ACE_SYNCH_DECL>
    std::istream& Session_T<ACE_SYNCH_USE>::receive_response (Response& response)
      {
        ACE_TRACE ("ACE_HTTP_Session::receive_response");

        if (this->in_stream_)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) HTTP_Session::receive_response - ")
                                  ACE_TEXT ("invalid invocation without send_request\n")));
            // receive_response called second time without
            // new send_request in between
            return ACE::IOS::Null::in_stream_;
          }

        if (this->out_stream_)
          {
            delete this->out_stream_;
            this->out_stream_ = 0;
          }

        this->sock_stream_->flush ();

        do
        {
          response.clear ();
          if (!response.read (*this->sock_stream_))
            {
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) (%d) HTTP_Session::receive_response - ")
                                    ACE_TEXT ("failed to read response\n"),
                                    ACE_OS::last_error ()));
              return ACE::IOS::Null::in_stream_;
            }
        }
        while (response.get_status ().get_status() == Status::HTTP_CONTINUE);

        this->needs_reconnect_ = this->keep_alive () && !response.has_keep_alive ();

        if (!this->expects_response_body_)
          {
            FixedLengthStreamPolicy* pol;
            ACE_NEW_RETURN (pol,
                            FixedLengthStreamPolicy (0),
                            ACE::IOS::Null::in_stream_);
            ACE_NEW_RETURN (this->in_stream_,
                            IStream (*this->sock_stream_, pol),
                            ACE::IOS::Null::in_stream_);
          }
        else if (response.has_chunked_transfer_encoding ())
          {
            ChunkedTransferStreamPolicy* pol;
            ACE_NEW_RETURN (pol,
                            ChunkedTransferStreamPolicy (),
                            ACE::IOS::Null::in_stream_);
            ACE_NEW_RETURN (this->in_stream_,
                            IStream (*this->sock_stream_, pol),
                            ACE::IOS::Null::in_stream_);
          }
        else if (response.get_content_length () != Header::UNKNOWN_CONTENT_LENGTH)
          {
            FixedLengthStreamPolicy* pol;
            ACE_NEW_RETURN (pol,
                            FixedLengthStreamPolicy (response.get_content_length ()),
                            ACE::IOS::Null::in_stream_);
            ACE_NEW_RETURN (this->in_stream_,
                            IStream (*this->sock_stream_, pol),
                            ACE::IOS::Null::in_stream_);
          }
        else
          {
            ACE_NEW_RETURN (this->in_stream_,
                            IStream (*this->sock_stream_),
                            ACE::IOS::Null::in_stream_);
          }

        return *this->in_stream_;
      }

    template <ACE_SYNCH_DECL>
    std::istream& Session_T<ACE_SYNCH_USE>::response_stream ()
      {
        return this->in_stream_ ? *this->in_stream_ : ACE::IOS::Null::in_stream_;
      }

    template <ACE_SYNCH_DECL>
    std::istream& Session_T<ACE_SYNCH_USE>::response_stream (
        ACE::IOS::StreamInterceptor& interceptor)
      {
        if (this->in_stream_)
          {
            this->in_stream_->set_interceptor (interceptor);
            return *this->in_stream_;
          }
        else
          return ACE::IOS::Null::in_stream_;
      }

    template <ACE_SYNCH_DECL>
    void Session_T<ACE_SYNCH_USE>::close ()
      {
        ACE_TRACE ("ACE_HTTP_Session::close");

        if (this->connection_)
          {
            if (this->in_stream_)
              {
                delete this->in_stream_;
                this->in_stream_ = 0;
              }
            if (this->out_stream_)
              {
                delete this->out_stream_;
                this->out_stream_ = 0;
              }
            if (this->sock_stream_)
              {
                delete this->sock_stream_;
                this->sock_stream_ = 0;
              }
            // this should be the last referece and removing it
            // causes the connection to be destroyed
            this->connection_->remove_reference ();
            this->connection_ = 0;
          }
      }

    template <ACE_SYNCH_DECL>
    bool Session_T<ACE_SYNCH_USE>::reconnect_needed ()
      {
        if (this->cannot_reconnect_)
          return false;
        if (!this->needs_reconnect_)
          {
            this->reconnect_countdown_.update ();
            return this->reconnect_timer_ == ACE_Time_Value::zero;
          }
        return true;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HTTP_SESSION_CPP */
