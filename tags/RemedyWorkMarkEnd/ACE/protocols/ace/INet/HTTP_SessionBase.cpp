// $Id$

#include "ace/INet/HTTP_SessionBase.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/HTTP_SessionBase.inl"
#endif

#include "ace/INet/INet_Log.h"
#include "ace/INet/HTTP_StreamPolicy.h"
#include "ace/INet/String_IOStream.h"
#include "ace/INet/IOS_util.h"
#include "ace/INet/HTTP_URL.h"
#include "ace/INET_Addr.h"
#include "ace/String_Base.h"
#include <istream>
#include <ostream>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    SessionBase::SessionBase (u_short port, bool keep_alive)
      : port_ (port),
        reactive_ (false),
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
        INET_TRACE ("ACE_HTTP_SessionBase - ctor");
      }

    SessionBase::SessionBase (u_short port,
                              const ACE_Time_Value& timeout,
                              bool keep_alive,
                              const ACE_Time_Value* alive_timeout)
      : port_ (port),
        reactive_ (false),
        in_stream_ (0),
        out_stream_ (0),
        http_timeout_ (timeout),
        keep_alive_timeout_ (DEFAULT_KEEP_ALIVE_TIMEOUT),
        reconnect_timer_ (DEFAULT_KEEP_ALIVE_TIMEOUT),
        reconnect_countdown_ (&reconnect_timer_),
        keep_alive_ (keep_alive),
        needs_reconnect_ (false),
        cannot_reconnect_ (false),
        expects_response_body_ (false)
      {
        INET_TRACE ("ACE_HTTP_SessionBase - ctor");
        if (keep_alive && alive_timeout)
          {
            this->keep_alive_timeout_ = *alive_timeout;
          }
      }

    SessionBase::~SessionBase ()
      {
        INET_TRACE ("ACE_HTTP_SessionBase - dtor");
        this->close_streams ();
      }

    bool SessionBase::connect (bool use_reactor)
      {
        INET_TRACE ("ACE_HTTP_SessionBase::connect");

        this->close ();

        unsigned long f_reactor = use_reactor ? ACE_Synch_Options::USE_REACTOR : 0;
        ACE_Synch_Options sync_opt (ACE_Synch_Options::USE_TIMEOUT | f_reactor,
                                    this->http_timeout_);

        return this->connect_i (sync_opt);
      }

    std::ostream& SessionBase::send_request (Request& request)
      {
        INET_TRACE ("ACE_HTTP_SessionBase::send_request");

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
                  INET_ERROR (1, (LM_ERROR, DLINFO
                                  ACE_TEXT ("(%d) HTTP_SessionBase::send_request - ")
                                  ACE_TEXT ("reconnect failed\n"),
                                  ACE_OS::last_error ()));
                return ACE::IOS::Null::out_stream_;
              }
          }
        if (!keep_alive)
          request.set_keep_alive (false);
        if (!request.has_host ())
          {
            if (this->port_ == URL::HTTP_PORT)
              request.set_host (this->host_);
            else
              request.set_host (this->host_, this->port_);
          }

        this->expects_response_body_ = request.get_method() != Request::HTTP_HEAD;

        if (request.has_chunked_transfer_encoding ())
        {
          request.write (this->sock_stream ());
          ChunkedTransferStreamPolicy* pol;
          ACE_NEW_RETURN (pol,
                          ChunkedTransferStreamPolicy (),
                          ACE::IOS::Null::out_stream_);
          ACE_NEW_RETURN (this->out_stream_,
                          OStream (this->sock_stream (), pol),
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
                          OStream (this->sock_stream (), pol),
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
                          OStream (this->sock_stream (), pol),
                          ACE::IOS::Null::out_stream_);
          (*this->out_stream_) << cs.str ().c_str ();
        }
        else
        {
          ACE_NEW_RETURN (this->out_stream_,
                          OStream (this->sock_stream ()),
                          ACE::IOS::Null::out_stream_);
          request.write (*this->out_stream_);
        }
        // reset reconnect timer
        this->reconnect_timer_ = this->keep_alive_timeout_;
        this->reconnect_countdown_.start ();

        return *this->out_stream_;
      }

    std::ostream& SessionBase::request_stream ()
      {
        if (this->out_stream_)
          return *this->out_stream_;
        else
          return ACE::IOS::Null::out_stream_;
      }

    std::ostream& SessionBase::request_stream (
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

    std::istream& SessionBase::receive_response (Response& response)
      {
        INET_TRACE ("ACE_HTTP_SessionBase::receive_response");

        if (this->in_stream_)
          {
            INET_ERROR (1, (LM_ERROR, DLINFO
                            ACE_TEXT ("HTTP_Session::receive_response - ")
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

        this->sock_stream ().flush ();

        do
        {
          response.clear ();
          if (!response.read (this->sock_stream ()))
            {
              INET_ERROR (1, (LM_ERROR, DLINFO
                              ACE_TEXT ("(%d) HTTP_Session::receive_response - ")
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
                            IStream (this->sock_stream (), pol),
                            ACE::IOS::Null::in_stream_);
          }
        else if (response.has_chunked_transfer_encoding ())
          {
            ChunkedTransferStreamPolicy* pol;
            ACE_NEW_RETURN (pol,
                            ChunkedTransferStreamPolicy (),
                            ACE::IOS::Null::in_stream_);
            ACE_NEW_RETURN (this->in_stream_,
                            IStream (this->sock_stream (), pol),
                            ACE::IOS::Null::in_stream_);
          }
        else if (response.get_content_length () != Header::UNKNOWN_CONTENT_LENGTH)
          {
            FixedLengthStreamPolicy* pol;
            ACE_NEW_RETURN (pol,
                            FixedLengthStreamPolicy (response.get_content_length ()),
                            ACE::IOS::Null::in_stream_);
            ACE_NEW_RETURN (this->in_stream_,
                            IStream (this->sock_stream (), pol),
                            ACE::IOS::Null::in_stream_);
          }
        else
          {
            ACE_NEW_RETURN (this->in_stream_,
                            IStream (this->sock_stream ()),
                            ACE::IOS::Null::in_stream_);
          }

        return *this->in_stream_;
      }

    std::istream& SessionBase::response_stream ()
      {
        if (this->in_stream_)
          return *this->in_stream_;
        else
          return ACE::IOS::Null::in_stream_;
      }

    std::istream& SessionBase::response_stream (
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

    void SessionBase::close ()
      {
        INET_TRACE ("ACE_HTTP_SessionBase::close");

        this->close_streams ();

        this->close_i ();
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
