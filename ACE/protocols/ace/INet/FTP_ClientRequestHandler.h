/**
 * @file FTP_ClientRequestHandler.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_FTP_CLIENT_REQUEST_HANDLER_H
#define ACE_FTP_CLIENT_REQUEST_HANDLER_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"
#include "ace/INet/INet_Export.h"
#include "ace/INet/IOS_util.h"
#include "ace/INet/StreamInterceptor.h"
#include "ace/INet/ClientRequestHandler.h"
#include "ace/INet/FTP_Request.h"
#include "ace/INet/FTP_Response.h"
#include "ace/INet/FTP_Session.h"
#include "ace/INet/FTP_URL.h"
#include "ace/INet/FTP_IOStream.h"
#include "ace/INet/Sock_IOStream.h"

#include <memory>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace FTP
      {
        /**
        * @class ACE_FTP_ClientRequestHandler
        *
        * @brief This class implements clientside request handling
        *   for FTP URLs.
        *
        * The class supports the FTP protocol as specified in RFCs 959
        * and 2428.
        */
        class ACE_INET_Export ClientRequestHandler
          : public ACE::INet::ClientINetRequestHandler,
            public ACE::IOS::StreamInterceptor
          {
            public:
              ClientRequestHandler (bool passive_mode = true);
              ~ClientRequestHandler () override;

              bool uses_passive_mode () const;

              void use_passive_mode ();

              void use_active_mode (u_short port = 0);

              u_short uses_active_port () const;

              Request& request () override;

              const Request& request () const override;

              Response& response () override;

              const Response& response () const override;

              std::ostream& request_stream ();

              std::istream& response_stream () override;

              bool is_response_ok () const override;

              std::istream& handle_open_request (const ACE::INet::URL_Base& url) override;

              virtual std::istream& handle_get_request (const URL& url, bool binary=true);
/*
              virtual std::ostream& start_put_request (const URL& url, bool binary=true);

              virtual bool finish_put_request ();
*/
              void quit_connection ();

              static ACE_CString const anonymous_user_;

            protected:
              static ACE_CString const empty_;

              using connection_type = typename Session_T<ACE_SYNCH>::connection_type;
              using stream_type = ACE::IOS::Sock_IOStreamBase<ACE_SYNCH>;

              void on_eof () override;

              class Authentication
                : public ACE::INet::AuthenticationBase
                {
                  public:
                    Authentication (ACE_CString const& realm,
                                    ACE_CString& user,
                                    ACE_CString& pw);

                    ~Authentication () override;

                    ACE_CString const& scheme () const override;

                    ACE_CString const& realm () const override;

                    ACE_CString const& user () const override;

                    void user (ACE_CString const& usr) override;

                    ACE_CString const& password () const override;

                    void password (ACE_CString const& pw) override;

                  private:
                    ACE_CString const& realm_;
                    ACE_CString& user_;
                    ACE_CString& password_;
                };

              class SessionHolder
                : public ACE::INet::ConnectionHolder
                {
                  public:
                    using session_type = Session_T<ACE_SYNCH>;

                    SessionHolder ();
                    ~SessionHolder () override;

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
                    ~SessionFactory () override;

                    ACE::INet::ConnectionHolder* create_connection (
                        const ACE::INet::ConnectionKey& key) const override;
                };

              SessionHolder& session ();

              void session (SessionHolder* session);

              virtual bool initialize_connection (ACE_CString const& host,
                                                  u_short port);

              virtual bool handle_credentials (const URL& url,
                                               ACE_CString& user,
                                               ACE_CString& password);

              virtual void handle_request_error (const URL& url);

              virtual void handle_connection_error (const URL& url);

              void release_connection ();

              void close_connection ();

              /*
               * FTP protocol helpers
               */

              bool login (ACE_CString const& user, ACE_CString const& password);

              bool logout ();

              bool is_valid_path (ACE_CString const& path);

              bool is_file (ACE_CString const& path);

              bool is_dir (ACE_CString const& path);

              bool set_filetype (bool binary);

              stream_type* start_download (ACE_CString const& path, bool binary);

              stream_type* start_upload (ACE_CString const& path, bool binary);

              bool finish_transfer ();

              bool abort_transfer ();

              Response::StatusType process_command (ACE_CString const& cmd,
                                                    ACE_CString const& arg = empty_);

              stream_type* open_data_connection (ACE_CString const& cmd,
                                                 ACE_CString const& arg);

              bool get_passive_address (ACE_INET_Addr& addr);

              bool parse_address (ACE_CString const& str, ACE_INET_Addr& addr);

              bool parse_ext_address (ACE_CString const& str, ACE_INET_Addr& addr);

              bool send_active_address (const ACE_INET_Addr& addr);

            private:
              Request request_;
              Response response_;
              SessionHolder* session_;
              bool use_passive_mode_;
              u_short active_port_;

              OStream out_data_stream_;
              IStream in_data_stream_;
              bool transfer_active_;

              ACE_CString current_user_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/FTP_ClientRequestHandler.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_INET_CLIENT_REQUEST_HANDLER_H */
