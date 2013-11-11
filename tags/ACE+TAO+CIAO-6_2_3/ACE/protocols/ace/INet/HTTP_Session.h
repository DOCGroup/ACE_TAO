// $Id$

/**
 * @file HTTP_Session.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTP_SESSION_H
#define ACE_HTTP_SESSION_H

#include /**/ "ace/pre.h"

#include "ace/INet/HTTP_SessionBase.h"
#include "ace/SOCK_Connector.h"
#include "ace/INet/StreamHandler.h"
#include "ace/INet/Sock_IOStream.h"

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
        class Session_T : public SessionBase
          {
            public:
              typedef ACE::IOS::StreamHandler<ACE_SOCK_STREAM, ACE_SYNCH_USE> connection_type;

              Session_T (bool keep_alive = false);

              Session_T (const ACE_Time_Value& timeout,
                         bool keep_alive = false,
                         const ACE_Time_Value* alive_timeout = 0);

              virtual ~Session_T ();

              virtual bool is_connected () const;

              bool attach_connection (connection_type* connection);

            protected:

              void close_connection ();

              virtual bool connect_i (const ACE_Synch_Options& sync_opt);

              virtual void close_i ();

              virtual std::iostream& sock_stream ();

            private:
              typedef ACE::IOS::Sock_IOStreamBase<ACE_SYNCH_USE> sock_stream_type;

              connection_type* connection_;
              sock_stream_type* sock_stream_;
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
