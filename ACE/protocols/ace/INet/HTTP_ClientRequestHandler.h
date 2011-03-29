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
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/INet/INet_Export.h"
#include "ace/INet/IOS_util.h"
#include "ace/INet/StreamInterceptor.h"
#include "ace/INet/ClientRequestHandler.h"
#include "ace/INet/HTTP_Request.h"
#include "ace/INet/HTTP_Response.h"
#include "ace/INet/HTTP_URL.h"
#include "ace/INet/HTTP_Session.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace HTTP
      {
        /**
        * @class ACE_HTTP_SessionHolder
        *
        * @brief Abstract base class for HTTP session objects.
        *
        */
        class ACE_INET_Export SessionHolder
          : public ACE::INet::ConnectionHolder
          {
            protected:
              SessionHolder ();
              virtual ~SessionHolder();

              virtual SessionBase& session () = 0;

            public:
              SessionBase& operator *();
              SessionBase* operator ->();
          };

        /**
        * @class ACE_HTTP_SessionFactory
        *
        * @brief Abstract base class for HTTP session factories.
        *
        */
        class ACE_INET_Export SessionFactory
          : public ACE::INet::ConnectionFactory
          {
            protected:
              SessionFactory () {}
              virtual ~SessionFactory () {}
          };

        /**
        * @class ACE_HTTP_SessionFactory_Impl
        *
        * @brief Implementation of HTTP session factory.
        *
        */
        class ACE_INET_Export SessionFactory_Impl
          : public SessionFactory
          {
            private:
              SessionFactory_Impl ();
              virtual ~SessionFactory_Impl ();

              friend class ACE_Singleton<SessionFactory_Impl, ACE_SYNCH::NULL_MUTEX>;

              static SessionFactory_Impl& factory_;

              class SessionHolder_Impl : public SessionHolder
                {
                  public:
                    SessionHolder_Impl ();
                    virtual ~SessionHolder_Impl();

                  protected:
                    virtual SessionBase& session ();

                  private:
                    Session_T<ACE_SYNCH> session_;
                };

            public:
              virtual ACE::INet::ConnectionHolder* create_connection (
                  const ACE::INet::ConnectionKey& key) const;
          };

        /**
        * @class ACE_HTTP_SessionFactoryRegistry
        *
        * @brief Implements registry of HTTP session factories.
        *
        */
        class ACE_INET_Export SessionFactoryRegistry
          {
            private:
              SessionFactoryRegistry ();
              ~SessionFactoryRegistry ();

              friend class ACE_Singleton<SessionFactoryRegistry, ACE_SYNCH::MUTEX>;

            public:

              void register_session_factory (const ACE_CString& scheme,
                                             SessionFactory* factory);

              SessionFactory* find_session_factory (const ACE_CString& scheme);

              static SessionFactoryRegistry& instance ();

            private:
              typedef ACE_Map_Manager<ACE_CString,
                                      SessionFactory*,
                                      ACE_SYNCH::MUTEX> TSessionFactoryMap;

              TSessionFactoryMap factory_map_;
          };

        /**
        * @class ACE_HTTP_ClientRequestHandler
        *
        * @brief This class implements clientside request handling
        *   for HTTP(S) URLs.
        *
        * The class supports the HTTP protocol as specified in RFC 2616.
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

#if defined(_MSC_VER)
              class ACE_INET_Export HttpConnectionKey
#else
              class HttpConnectionKey
#endif
                : public INetConnectionKey
                {
                  public:
                    HttpConnectionKey (const ACE_CString& host,
                                       u_short port);
                    HttpConnectionKey (const ACE_CString& proxy_host,
                                       u_short proxy_port,
                                       const ACE_CString& target_host,
                                       u_short target_port);
                    virtual ~HttpConnectionKey();

                    virtual u_long hash () const;

                    virtual ConnectionKey* duplicate () const;

                    bool is_proxy_connection () const;

                    const ACE_CString& proxy_target_host () const;

                    u_short proxy_target_port () const;

                  protected:
                    virtual bool equal (const ConnectionKey& key) const;

                  private:
                    bool proxy_connection_;
                    ACE_CString proxy_target_host_;
                    u_short proxy_target_port_;
                };

            protected:
              virtual void on_eof ();

              SessionHolder& session ();

              void session (SessionHolder* session);

              virtual bool initialize_connection (const ACE_CString& scheme,
                                                  const ACE_CString& host,
                                                  u_short port,
                                                  bool proxy_conn = false,
                                                  const ACE_CString& proxy_host = Request::EMPTY,
                                                  u_short proxy_port = 0);

              virtual void initialize_request (const URL& url, Request& request);

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
#include "ace/INet/HTTP_ClientRequestHandler.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_INET_CLIENT_REQUEST_HANDLER_H */
