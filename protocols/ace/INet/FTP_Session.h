// $Id$

/**
 * @file FTP_Session.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_FTP_SESSION_H
#define ACE_FTP_SESSION_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"
#include "ace/Countdown_Time.h"
#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
#include "ace/INet/INet_Export.h"
#include "ace/INet/FTP_Request.h"
#include "ace/INet/FTP_Response.h"
#include "ace/INet/StreamHandler.h"
#include "ace/INet/Sock_IOStream.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace FTP
      {
        /**
        * @class ACE_FTP_Session
        *
        * @brief Encapsulates FTP session.
        *
        */
        template <ACE_SYNCH_DECL>
        class Session_T
          {
            public:
              typedef ACE::IOS::StreamHandler<ACE_SOCK_STREAM, ACE_SYNCH_USE> connection_type;

              enum
              {
                FTP_PORT = 21
              };

              Session_T ();

              Session_T (const ACE_Time_Value& timeout);

              virtual ~Session_T ();

              bool is_connected () const;

              bool is_new_connection () const;

              void set_host (const ACE_CString& host, u_short port);

              void set_host (const ACE_CString& host);

              void set_port (u_short port);

              const ACE_CString& get_host () const;

              u_short get_port () const;

              bool connect (bool use_reactor = false);

              bool connect (connection_type* connection);

              bool send_request (Request& request);

              bool receive_response (Response& response);

              bool send_interrupt ();

              void close ();

              bool supports_ftp_extensions () const;

              void set_ftp_extension_support (bool f);

              bool is_reactive () const;

              const ACE_Time_Value& timeout () const;

              void get_local_addr (ACE_INET_Addr& loc_addr) const;

            private:
              enum
              {
                DEFAULT_TIMEOUT = 30,           // sec

                INTERRUPT = 244   // Telnet IP signal
              };

              ACE_CString host_;
              u_short port_;

              typedef ACE::IOS::Sock_IOStreamBase<ACE_SYNCH_USE> sock_stream_type;

              bool reactive_;
              connection_type* connection_;
              sock_stream_type* sock_stream_;
              ACE_Time_Value ftp_timeout_;
              bool cannot_reconnect_;
              bool has_ftp_ext_; // RFC 2428 - IPv6 and NAT
              bool new_connect_;
          };

        typedef Session_T<ACE_NULL_SYNCH> Session;
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/INet/FTP_Session.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("FTP_Session.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_FTP_SESSION_H */
