// $Id$

/**
 * @file HTTP_Session.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTP_SESSION_H
#define ACE_HTTP_SESSION_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"
#include "ace/Countdown_Time.h"
#include "ace/SOCK_Connector.h"
#include "ace/INet/INet_Export.h"
#include "ace/INet/HTTP_Request.h"
#include "ace/INet/HTTP_Response.h"
#include "ace/INet/StreamHandler.h"
#include "ace/INet/Sock_IOStream.h"
#include "ace/INet/HTTP_IOStream.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTP
      {
        /**
        * @class ACE_HTTP_Session
        *
        * @brief Encapsulates HTTP session.
        *
        */
        template <ACE_SYNCH_DECL>
        class Session_T
          {
            public:
              typedef ACE::IOS::StreamHandler<ACE_SOCK_STREAM, ACE_SYNCH_USE> connection_type;

              enum
              {
                HTTP_PORT = 80
              };

              Session_T (bool keep_alive = false);
              virtual ~Session_T ();

              void set_keep_alive (bool f);

              bool keep_alive () const;

              bool is_connected () const;

              void set_host (const ACE_CString& host, u_short port);

              void set_host (const ACE_CString& host);

              void set_port (u_short port);

              const ACE_CString& get_host () const;

              u_short get_port () const;

              bool connect (bool use_reactor = false);

              bool connect (connection_type* connection);

              std::ostream& send_request (Request& request);

              std::ostream& request_stream ();

              std::ostream& request_stream (ACE::IOS::StreamInterceptor& interceptor);

              std::istream& receive_response (Response& response);

              std::istream& response_stream ();

              std::istream& response_stream (ACE::IOS::StreamInterceptor& interceptor);

              void close ();

            protected:

              bool reconnect_needed ();

            private:
              enum
              {
                DEFAULT_TIMEOUT = 30,           // sec
                DEFAULT_KEEP_ALIVE_TIMEOUT = 8  // sec
              };

              ACE_CString host_;
              u_short port_;

              typedef ACE::IOS::Sock_IOStreamBase<ACE_SYNCH_USE> sock_stream_type;

              bool reactive_;
              connection_type* connection_;
              sock_stream_type* sock_stream_;
              IStream* in_stream_;
              OStream* out_stream_;
              ACE_Time_Value http_timeout_;
              ACE_Time_Value keep_alive_timeout_;
              ACE_Time_Value reconnect_timer_;
              ACE_Countdown_Time reconnect_countdown_;
              bool keep_alive_;
              bool needs_reconnect_;
              bool cannot_reconnect_;
              bool expects_response_body_;
          };

        typedef Session_T<ACE_NULL_SYNCH> Session;
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/INet/HTTP_Session.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("HTTP_Session.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_HTTP_SESSION_H */
