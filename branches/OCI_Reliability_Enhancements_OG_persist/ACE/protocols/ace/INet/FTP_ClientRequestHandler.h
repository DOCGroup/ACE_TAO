// $Id$

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
              virtual ~ClientRequestHandler ();

              bool uses_passive_mode () const;

              void use_passive_mode ();

              void use_active_mode (u_short port = 0);

              u_short uses_active_port () const;

              virtual Request& request ();

              virtual const Request& request () const;

              virtual Response& response ();

              virtual const Response& response () const;

              std::ostream& request_stream ();

              virtual std::istream& response_stream ();

              virtual bool is_response_ok () const;

              virtual std::istream& handle_open_request (const ACE::INet::URL_Base& url);

              virtual std::istream& handle_get_request (const URL& url, bool binary=true);
/*
              virtual std::ostream& start_put_request (const URL& url, bool binary=true);

              virtual bool finish_put_request ();
*/
              void quit_connection ();

              static const ACE_CString anonymous_user_;

            protected:
              static const ACE_CString empty_;

              typedef Session_T<ACE_SYNCH>::connection_type connection_type;
              typedef ACE::IOS::Sock_IOStreamBase<ACE_SYNCH> stream_type;

              virtual void on_eof ();

              class Authentication
                : public ACE::INet::AuthenticationBase
                {
                  public:
                    Authentication (const ACE_CString& realm,
                                    ACE_CString& user,
                                    ACE_CString& pw);

                    virtual ~Authentication ();

                    virtual const ACE_CString& scheme () const;

                    virtual const ACE_CString& realm () const;

                    virtual const ACE_CString& user () const;

                    virtual void user (const ACE_CString& usr);

                    virtual const ACE_CString& password () const;

                    virtual void password (const ACE_CString& pw);

                  private:
                    const ACE_CString& realm_;
                    ACE_CString& user_;
                    ACE_CString& password_;
                };

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

              bool login (const ACE_CString& user, const ACE_CString& password);

              bool logout ();

              bool is_valid_path (const ACE_CString& path);

              bool is_file (const ACE_CString& path);

              bool is_dir (const ACE_CString& path);

              bool set_filetype (bool binary);

              stream_type* start_download (const ACE_CString& path, bool binary);

              stream_type* start_upload (const ACE_CString& path, bool binary);

              bool finish_transfer ();

              bool abort_transfer ();

              Response::StatusType process_command (const ACE_CString& cmd,
                                                    const ACE_CString& arg = empty_);

              stream_type* open_data_connection (const ACE_CString& cmd,
                                                 const ACE_CString& arg);

              bool get_passive_address (ACE_INET_Addr& addr);

              bool parse_address (const ACE_CString& str, ACE_INET_Addr& addr);

              bool parse_ext_address (const ACE_CString& str, ACE_INET_Addr& addr);

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
