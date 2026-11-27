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

#include <memory>

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
              ~SessionHolder () override;

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
              SessionFactory () = default;
              ~SessionFactory () override = default;
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
              ~SessionFactory_Impl () override;

              friend class ACE_Singleton<SessionFactory_Impl, ACE_SYNCH::NULL_MUTEX>;

              static SessionFactory_Impl& factory_;

              class SessionHolder_Impl : public SessionHolder
                {
                  public:
                    SessionHolder_Impl ();
                    ~SessionHolder_Impl () override;

                  protected:
                    SessionBase& session () override;

                  private:
                    Session_T<ACE_SYNCH> session_;
                };

            public:
              ACE::INet::ConnectionHolder* create_connection (
                  ACE::INet::ConnectionKey const& key) const override;
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
              void register_session_factory (ACE_CString const& scheme,
                                             SessionFactory* factory);

              SessionFactory* find_session_factory (ACE_CString const& scheme);

              static SessionFactoryRegistry& instance ();

            private:
              using TSessionFactoryMap = ACE_Map_Manager<ACE_CString, SessionFactory*, ACE_SYNCH::MUTEX>;

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
              ~ClientRequestHandler () override;

              Request& request () override;

              Request const& request () const override;

              Response& response () override;

              Response const& response () const override;

              std::ostream& request_stream ();

              std::istream& response_stream () override;

              bool is_response_ok () const override;

              std::istream& handle_open_request (ACE::INet::URL_Base const& url) override;

              virtual std::istream& handle_get_request (URL const& url);
/*
              virtual std::istream& handle_post_request (URL const& url,
                                                         PostData const& post_data);

              virtual std::istream& handle_put_request (URL const& url,
                                                        std::istream* put_data = nullptr);
*/

              class ACE_INET_Export HttpConnectionKey
                : public INetConnectionKey
                {
                  public:
                    HttpConnectionKey (ACE_CString const& host,
                                       u_short port);
                    HttpConnectionKey (ACE_CString const& proxy_host,
                                       u_short proxy_port,
                                       ACE_CString const& target_host,
                                       u_short target_port);
                    ~HttpConnectionKey () override;

                    u_long hash () const override;

                    ACE::INet::ConnectionKey* duplicate () const override;

                    bool is_proxy_connection () const;

                    ACE_CString const& proxy_target_host () const;

                    u_short proxy_target_port () const;

                  protected:
                    bool equal (ACE::INet::ConnectionKey const& key) const override;

                  private:
                    bool proxy_connection_;
                    ACE_CString proxy_target_host_;
                    u_short proxy_target_port_;
                };

            protected:
              void on_eof () override;

              SessionHolder& session ();

              void session (SessionHolder* session);

              virtual bool initialize_connection (ACE_CString const& scheme,
                                                  ACE_CString const& host,
                                                  u_short port,
                                                  bool proxy_conn = false,
                                                  ACE_CString const& proxy_host = Request::EMPTY,
                                                  u_short proxy_port = 0);

              virtual void initialize_request (URL const& url, Request& request);

              virtual void handle_request_error (URL const& url);

              virtual void handle_connection_error (URL const& url);

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
