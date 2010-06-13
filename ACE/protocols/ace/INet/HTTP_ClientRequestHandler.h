// $Id$

/**
 * @file HTTP_ClientRequestHandler.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_HTTP_CLIENT_REQUEST_HANDLER_H
#define ACE_HTTP_CLIENT_REQUEST_HANDLER_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"
#include "INet_Export.h"
#include "IOS_util.h"
#include "StreamInterceptor.h"
#include "ClientRequestHandler.h"
#include "HTTP_Request.h"
#include "HTTP_Response.h"
#include "HTTP_Session.h"
#include "HTTP_URL.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTP
      {
        /**
        * @class ACE_HTTP_ClientRequestHandler
        *
        * @brief
        *
        */
        class ACE_INET_Export ClientRequestHandler
          : public ACE::INet::ClientINetRequestHandler,
            public ACE::IOS::StreamInterceptor
          {
            public:
              ClientRequestHandler ();
              virtual ~ClientRequestHandler ();

              virtual Request& request ();

              virtual const Request& request () const;

              virtual Response& response ();

              virtual const Response& response () const;

              std::ostream& request_stream ();

              virtual std::istream& response_stream ();

              virtual bool is_response_ok () const;

              virtual std::istream& handle_open_request (const ACE::INet::URL_Base& url);

              virtual std::istream& handle_get_request (const URL& url);
/*
              virtual std::istream& handle_post_request (const URL& url,
                                                         const PostData& post_data);

              virtual std::istream& handle_put_request (const URL& url,
                                                        std::istream* put_data = 0);
*/
            protected:
              virtual void on_eof ();

              class SessionHolder
                : public ACE::INet::ConnectionHolder
                {
                  public:
                    typedef Session_T<ACE_SYNCH> session_type;

                    SessionHolder ();
                    virtual ~SessionHolder();

                    session_type& operator *();
                    session_type* operator ->();

                  private:
                    session_type session_;
                };

              class SessionFactory
                : public ACE::INet::ConnectionFactory
                {
                  public:
                    SessionFactory ();
                    virtual ~SessionFactory ();

                    virtual ACE::INet::ConnectionHolder* create_connection (
                        const ACE::INet::ConnectionKey& key) const;
                };

              SessionHolder& session ();

              void session (SessionHolder* session);

              virtual bool initialize_connection (const ACE_CString& host,
                                                  u_short port);

              virtual void initialize_request (const URL& url, Request& request);

              virtual bool send_request (std::ostream& request_stream);

              virtual void handle_response (const URL& url, const Response& response);

              virtual void handle_request_error (const URL& url);

              virtual void handle_connection_error (const URL& url);

              void release_connection ();

              void close_connection ();

            private:
              Request request_;
              Response response_;
              SessionHolder* session_;
          };

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "HTTP_ClientRequestHandler.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_INET_CLIENT_REQUEST_HANDLER_H */
