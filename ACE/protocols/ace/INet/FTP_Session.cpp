// $Id$

#ifndef ACE_FTP_SESSION_CPP
#define ACE_FTP_SESSION_CPP

#include "ace/INet/INet_Log.h"
#include "ace/INet/FTP_Session.h"
#include "ace/INet/String_IOStream.h"
#include "ace/INet/IOS_util.h"
#include "ace/INET_Addr.h"
#include "ace/Event_Handler.h"
#include "ace/Connector.h"
#include "ace/String_Base.h"
#include "ace/Truncate.h"
#include <istream>
#include <ostream>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace FTP
  {

    template <ACE_SYNCH_DECL>
    Session_T<ACE_SYNCH_USE>::Session_T ()
      : port_ (FTP_PORT),
        reactive_ (false),
        connection_ (0),
        sock_stream_ (0),
        ftp_timeout_ (DEFAULT_TIMEOUT),
        cannot_reconnect_ (false),
        has_ftp_ext_ (true),
        new_connect_ (true)
      {
        INET_TRACE ("ACE_FTP_Session - ctor");
      }

    template <ACE_SYNCH_DECL>
    Session_T<ACE_SYNCH_USE>::Session_T (const ACE_Time_Value& timeout)
      : port_ (FTP_PORT),
        reactive_ (false),
        connection_ (0),
        sock_stream_ (0),
        ftp_timeout_ (timeout),
        cannot_reconnect_ (false),
        has_ftp_ext_ (true),
        new_connect_ (true)
      {
        INET_TRACE ("ACE_FTP_Session - ctor");
      }

    template <ACE_SYNCH_DECL>
    Session_T<ACE_SYNCH_USE>::~Session_T ()
      {
        INET_TRACE ("ACE_FTP_Session - dtor");
        this->close ();
      }

    template <ACE_SYNCH_DECL>
    bool Session_T<ACE_SYNCH_USE>::is_connected () const
      {
        return this->connection_ && this->connection_->is_connected ();
      }

    template <ACE_SYNCH_DECL>
    bool Session_T<ACE_SYNCH_USE>::is_new_connection () const
      {
        return this->new_connect_;
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
        INET_TRACE ("ACE_FTP_Session::connect");

        typedef ACE_Connector<connection_type, ACE_SOCK_CONNECTOR> connector_type;

        this->close ();

        unsigned long f_reactor = use_reactor ? ACE_Synch_Options::USE_REACTOR : 0;
        ACE_Synch_Options sync_opt (ACE_Synch_Options::USE_TIMEOUT | f_reactor,
                                    this->ftp_timeout_);
        connector_type connector;

        connection_type* new_connection = 0;
        ACE_NEW_RETURN (new_connection,
                        connection_type(sync_opt),
                        false);
        if (connector.connect (new_connection,
                               ACE_INET_Addr (this->port_,
                                              this->host_.c_str ()),
                               ACE_Synch_Options (0,this->ftp_timeout_)) == -1)
          {
            INET_ERROR (1, (LM_ERROR, DLINFO
                            ACE_TEXT ("(%d) ACE_FTP_Session::connect - ")
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
            this->new_connect_ = true;
            this->cannot_reconnect_ = false;
            this->reactive_ = use_reactor;

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
        INET_TRACE ("ACE_FTP_Session::connect(connection)");

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
                INET_ERROR (1, (LM_ERROR, DLINFO
                                ACE_TEXT ("(%d) ACE_FTP_Session::connect(connection) - ")
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
            this->new_connect_ = true;
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
    bool Session_T<ACE_SYNCH_USE>::send_request (Request& request)
      {
        INET_TRACE ("ACE_FTP_Session::send_request");

        if (!this->is_connected ())
          {
            if (this->cannot_reconnect_ || !this->connect(this->reactive_))
              {
                if (!this->cannot_reconnect_)
                  INET_ERROR (1, (LM_ERROR, DLINFO
                                  ACE_TEXT ("(%d) FTP_Session::send_request - ")
                                  ACE_TEXT ("reconnect failed\n"),
                                  ACE_OS::last_error ()));
                return false;
              }
          }

        this->new_connect_ = false;

        request.write (*this->sock_stream_);

        return this->is_connected () && this->sock_stream_->good ();
      }

    template <ACE_SYNCH_DECL>
    bool Session_T<ACE_SYNCH_USE>::receive_response (Response& response)
      {
        INET_TRACE ("ACE_FTP_Session::receive_response");

        this->sock_stream_->flush ();

        response.reset ();
        return response.read (*this->sock_stream_);
      }

    template <ACE_SYNCH_DECL>
    bool Session_T<ACE_SYNCH_USE>::send_interrupt ()
      {
        INET_TRACE ("ACE_FTP_Session::send_interrupt");

        if (this->is_connected ())
          {
            this->sock_stream_->put (ACE_Utils::truncate_cast<char> (int (INTERRUPT)));
            this->sock_stream_->sync ();
            return this->sock_stream_->good ();
          }

        return false;
      }

    template <ACE_SYNCH_DECL>
    void Session_T<ACE_SYNCH_USE>::close ()
      {
        INET_TRACE ("ACE_FTP_Session::close");

        if (this->connection_)
          {
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
    bool Session_T<ACE_SYNCH_USE>::supports_ftp_extensions () const
      {
        return this->has_ftp_ext_;
      }

    template <ACE_SYNCH_DECL>
    void Session_T<ACE_SYNCH_USE>::set_ftp_extension_support (bool f)
      {
        this->has_ftp_ext_ = f;
      }

    template <ACE_SYNCH_DECL>
    bool Session_T<ACE_SYNCH_USE>::is_reactive () const
      {
        return this->reactive_;
      }

    template <ACE_SYNCH_DECL>
    const ACE_Time_Value& Session_T<ACE_SYNCH_USE>::timeout () const
      {
        return this->ftp_timeout_;
      }

    template <ACE_SYNCH_DECL>
    void Session_T<ACE_SYNCH_USE>::get_local_addr (ACE_INET_Addr& loc_addr) const
      {
        if (this->is_connected ())
          this->connection_->peer ().get_local_addr (loc_addr);
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_FTP_SESSION_CPP */
