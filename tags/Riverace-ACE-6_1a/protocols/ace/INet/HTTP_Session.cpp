// $Id$

#ifndef ACE_HTTP_SESSION_CPP
#define ACE_HTTP_SESSION_CPP

#include "ace/INet/HTTP_Session.h"
#include "ace/INet/INet_Log.h"
#include "ace/INet/IOS_util.h"
#include "ace/INet/HTTP_URL.h"
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
      : SessionBase (URL::HTTP_PORT, keep_alive),
        connection_ (0),
        sock_stream_ (0)
      {
        INET_TRACE ("ACE_HTTP_Session - ctor");
      }

    template <ACE_SYNCH_DECL>
    Session_T<ACE_SYNCH_USE>::Session_T (const ACE_Time_Value& timeout,
                                         bool keep_alive,
                                         const ACE_Time_Value* alive_timeout)
      : SessionBase (URL::HTTP_PORT, timeout, keep_alive, alive_timeout),
        connection_ (0),
        sock_stream_ (0)
      {
        INET_TRACE ("ACE_HTTP_Session - ctor");
      }

    template <ACE_SYNCH_DECL>
    Session_T<ACE_SYNCH_USE>::~Session_T ()
      {
        INET_TRACE ("ACE_HTTP_Session - dtor");
        this->close_streams ();
        this->close_connection ();
      }

    template <ACE_SYNCH_DECL>
    bool Session_T<ACE_SYNCH_USE>::is_connected () const
      {
        return this->connection_ && this->connection_->is_connected ();
      }

    template <ACE_SYNCH_DECL>
    bool Session_T<ACE_SYNCH_USE>::connect_i (const ACE_Synch_Options& sync_opt)
      {
        INET_TRACE ("ACE_HTTP_Session::connect_i");

        typedef ACE_Connector<connection_type, ACE_SOCK_CONNECTOR> connector_type;

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
            INET_ERROR (1, (LM_ERROR, DLINFO
                            ACE_TEXT ("(%d) ACE_HTTP_Session::connect_i - ")
                            ACE_TEXT ("failed to connect; host=%C, port=%d\n"),
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
        INET_TRACE ("ACE_HTTP_Session::attach_connection");

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
        INET_TRACE ("ACE_HTTP_Session::close_i");

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

#endif /* ACE_HTTP_SESSION_CPP */
