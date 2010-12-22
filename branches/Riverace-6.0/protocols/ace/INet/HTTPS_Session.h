// $Id$

/**
 * @file HTTPS_Session.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTPS_SESSION_H
#define ACE_HTTPS_SESSION_H

#include /**/ "ace/pre.h"

#include "ace/SSL/SSL_SOCK_Connector.h"
#include "ace/INet/HTTP_SessionBase.h"
#include "ace/INet/StreamHandler.h"
#include "ace/INet/SSLSock_IOStream.h"
#include "ace/INet/HTTPS_Context.h"

// we need OpenSSL > v0.9.8e to get support for SSL_set_SSL_CTX
#define OPENSSL_VERSION_098E 0x0090805fL
#if OPENSSL_VERSION_NUMBER > OPENSSL_VERSION_098E
# define SSL_HAS_SSL_set_SSL_CTX 1
#else
# define SSL_HAS_SSL_set_SSL_CTX 0
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTPS
      {
        /**
        * @class ACE_HTTPS_Session
        *
        * @brief Encapsulates HTTPS session.
        *
        */
        template <ACE_SYNCH_DECL>
        class Session_T : public ACE::HTTP::SessionBase
          {
            public:
              typedef ACE::IOS::StreamHandler<ACE_SSL_SOCK_Stream, ACE_SYNCH_USE> connection_type;

              Session_T (bool keep_alive = true
#if defined (SSL_HAS_SSL_set_SSL_CTX) && (SSL_HAS_SSL_set_SSL_CTX == 1)
                         , Context* ctx = 0
#endif
                         );

              Session_T (const ACE_Time_Value& timeout,
                         bool keep_alive = true,
                         const ACE_Time_Value* alive_timeout = 0
#if defined (SSL_HAS_SSL_set_SSL_CTX) && (SSL_HAS_SSL_set_SSL_CTX == 1)
                         , Context* ctx = 0
#endif
                         );

              virtual ~Session_T ();

              virtual bool is_connected () const;

              bool attach_connection (connection_type* connection);

            protected:

              void close_connection ();

              virtual bool connect_i (const ACE_Synch_Options& sync_opt);

              virtual void close_i ();

              virtual std::iostream& sock_stream ();

            private:
              typedef ACE::IOS::SSLSock_IOStreamBase<ACE_SYNCH_USE> sock_stream_type;

              connection_type* connection_;
              sock_stream_type* sock_stream_;
#if defined (SSL_HAS_SSL_set_SSL_CTX) && (SSL_HAS_SSL_set_SSL_CTX == 1)
              Context* context_;
#endif
          };

        typedef Session_T<ACE_NULL_SYNCH> Session;
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/INet/HTTPS_Session.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("HTTPS_Session.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_HTTPS_SESSION_H */
