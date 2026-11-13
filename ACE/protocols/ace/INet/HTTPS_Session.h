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
#include <memory>

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
              using connection_type = ACE::IOS::StreamHandler<ACE_SSL_SOCK_Stream, ACE_SYNCH_USE>;

              Session_T (bool keep_alive = true
#if defined (SSL_HAS_SSL_set_SSL_CTX) && (SSL_HAS_SSL_set_SSL_CTX == 1)
                         , Context* ctx = nullptr
#endif
                         );

              Session_T (Session_T const&) = delete;
              Session_T& operator= (Session_T const&) = delete;
              Session_T (Session_T&&) = delete;
              Session_T& operator= (Session_T&&) = delete;

              Session_T (const ACE_Time_Value& timeout,
                         bool keep_alive = true,
                         const ACE_Time_Value* alive_timeout = nullptr
#if defined (SSL_HAS_SSL_set_SSL_CTX) && (SSL_HAS_SSL_set_SSL_CTX == 1)
                         , Context* ctx = nullptr
#endif
                         );

              ~Session_T () override;

              bool is_connected () const override;

              bool attach_connection (connection_type* connection);

            protected:
              void close_connection ();

              bool connect_i (const ACE_Synch_Options& sync_opt) override;

              void close_i () override;

              std::iostream& sock_stream () override;

            private:
              using sock_stream_type = ACE::IOS::SSLSock_IOStreamBase<ACE_SYNCH_USE>;

              /// The session holds an ACE event-handler reference to this connection.
              connection_type* connection_ {};
              std::unique_ptr<sock_stream_type> sock_stream_ {};
#if defined (SSL_HAS_SSL_set_SSL_CTX) && (SSL_HAS_SSL_set_SSL_CTX == 1)
              /// Borrowed; the caller retains ownership of the SSL context.
              Context* context_;
#endif
          };

        using Session = Session_T<ACE_NULL_SYNCH>;
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#include "ace/INet/HTTPS_Session.cpp"

#include /**/ "ace/post.h"
#endif /* ACE_HTTPS_SESSION_H */
