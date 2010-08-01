// $Id$

/**
 * @file HTTP_SessionBase.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTP_SESSION_BASE_H
#define ACE_HTTP_SESSION_BASE_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"
#include "ace/Countdown_Time.h"
#include "ace/Synch_Options.h"
#include "ace/INet/INet_Export.h"
#include "ace/INet/HTTP_Request.h"
#include "ace/INet/HTTP_Response.h"
#include "ace/INet/HTTP_IOStream.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTP
      {
        /**
        * @class ACE_HTTP_SessionBase
        *
        * @brief Abstract base class for HTTP(S) sessions.
        *
        */
        class ACE_INET_Export SessionBase
          {
            public:
              SessionBase (u_short port,
                           bool keep_alive = false);

              SessionBase (u_short port,
                           const ACE_Time_Value& timeout,
                           bool keep_alive = false,
                           const ACE_Time_Value* alive_timeout = 0);

              virtual ~SessionBase ();

              void set_keep_alive (bool f);

              bool keep_alive () const;

              virtual bool is_connected () const = 0;

              void set_host (const ACE_CString& host,
                             u_short port);

              void set_host (const ACE_CString& host);

              void set_port (u_short port);

              void set_proxy_target (const ACE_CString& host, u_short port);

              const ACE_CString& get_host () const;

              u_short get_port () const;

              bool is_proxy_connection () const;

              const ACE_CString& get_proxy_target_host () const;

              u_short get_proxy_target_port () const;

              bool connect (bool use_reactor = false);

              std::ostream& send_request (Request& request);

              std::ostream& request_stream ();

              std::ostream& request_stream (ACE::IOS::StreamInterceptor& interceptor);

              std::istream& receive_response (Response& response);

              std::istream& response_stream ();

              std::istream& response_stream (ACE::IOS::StreamInterceptor& interceptor);

              void close ();

            protected:

              bool reconnect_needed ();

              void close_streams ();

              virtual bool connect_i (const ACE_Synch_Options& sync_opt) = 0;

              virtual void close_i () = 0;

              virtual std::iostream& sock_stream () = 0;

              enum
              {
                DEFAULT_TIMEOUT = 30,           // sec
                DEFAULT_KEEP_ALIVE_TIMEOUT = 8  // sec
              };

              ACE_CString host_;
              u_short port_;
              bool proxy_connection_;
              ACE_CString proxy_target_host_;
              u_short proxy_target_port_;

              bool reactive_;
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
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/HTTP_SessionBase.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_HTTP_SESSION_BASE_H */
