// $Id$

#include "ace/INet/FTP_ClientRequestHandler.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/FTP_ClientRequestHandler.inl"
#endif

#include "ace/INet/INet_Log.h"
#include "ace/INet/String_IOStream.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_ctype.h"
#include "ace/Connector.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"



ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace FTP
  {

    ClientRequestHandler::SessionHolder::SessionHolder ()
      : session_ ()
      {
      }

    ClientRequestHandler::SessionHolder::~SessionHolder()
      {
      }

    ClientRequestHandler::SessionFactory::SessionFactory ()
      {
      }

    ClientRequestHandler::SessionFactory::~SessionFactory ()
      {
      }

    ACE::INet::ConnectionHolder*
    ClientRequestHandler::SessionFactory::create_connection (
        const ACE::INet::ConnectionKey& key) const
      {
        INET_TRACE ("FTP::ClientRequestHandler::SessionFactory::create_connection");

        const INetConnectionKey& ikey = dynamic_cast<const INetConnectionKey&> (key);

        SessionHolder* session_holder = 0;
        ACE_NEW_RETURN (session_holder,
                        SessionHolder (),
                        0);
        ACE_Auto_Ptr<SessionHolder> session_safe_ref (session_holder);

        (*session_holder)->set_host (ikey.host (), ikey.port ());

        if ((*session_holder)->connect (true))
          {
            return session_safe_ref.release ();
          }

        return 0;
      }

    ClientRequestHandler::Authentication::Authentication (
        const ACE_CString& realm,
        ACE_CString& user,
        ACE_CString& pw)
      : AuthenticationBase (),
        realm_ (realm),
        user_ (user),
        password_ (pw)
      {
      }

    ClientRequestHandler::Authentication::~Authentication () {}

    const ACE_CString& ClientRequestHandler::Authentication::scheme () const
      {
        return URL::protocol ();
      }

    const ACE_CString& ClientRequestHandler::Authentication::realm () const
      {
        return this->realm_;
      }

    const ACE_CString& ClientRequestHandler::Authentication::user () const
      {
        return this->user_;
      }

    void ClientRequestHandler::Authentication::user (const ACE_CString& usr)
      {
        this->user_ = usr;
      }

    const ACE_CString& ClientRequestHandler::Authentication::password () const
      {
        return this->password_;
      }

    void ClientRequestHandler::Authentication::password (const ACE_CString& pw)
      {
        this->password_ = pw;
      }

    const ACE_CString ClientRequestHandler::anonymous_user_ = "anonymous";
    const ACE_CString ClientRequestHandler::empty_;

    ClientRequestHandler::ClientRequestHandler (bool passive_mode)
      : session_ (0),
        use_passive_mode_ (passive_mode),
        active_port_ (0),
        out_data_stream_ (0),
        in_data_stream_ (0),
        transfer_active_ (false)
      {
        this->out_data_stream_.set_interceptor (*this);
        this->in_data_stream_.set_interceptor (*this);
      }

    ClientRequestHandler::~ClientRequestHandler ()
      {
        this->release_connection ();
      }

    bool ClientRequestHandler::is_response_ok () const
      {
        return this->response_.is_completed_ok () ||
               (this->response_.is_preliminary_ok () &&
                    !this->in_data_stream_.bad ());
      }

    std::istream& ClientRequestHandler::handle_open_request (
        const ACE::INet::URL_Base& url)
      {
        const URL& ftp_url = dynamic_cast<const URL&> (url);
        return this->handle_get_request (ftp_url);
      }

    std::istream& ClientRequestHandler::handle_get_request (
        const URL& ftp_url,
        bool binary)
      {
        if (this->initialize_connection (ftp_url.get_host (),
                                         ftp_url.get_port ()))
          {
            bool rc = true;
            if (this->session ()->is_new_connection () ||
                (!ftp_url.get_user_info ().empty () &&
                    ftp_url.get_user_info () != this->current_user_))
              {
                if (!this->session ()->is_new_connection ())
                  rc = this->logout ();

                if (rc)
                  {
                    ACE_CString user = ftp_url.get_user_info ().empty () ?
                        anonymous_user_ : ftp_url.get_user_info ();
                    ACE_CString password = user;
                    rc = this->handle_credentials (ftp_url, user, password);
                    if (rc) rc = this->login (user, password);
                  }
              }

            if (rc) rc = this->finish_transfer ();

            if (rc)
              {
                stream_type* data_stream =
                    this->start_download (ftp_url.get_path (), binary);
                if (data_stream)
                  {
                    // no need to check prev stream since we just called finish_transfer
                    // to clean everything up
                    this->in_data_stream_.set_stream (data_stream);
                  }
                else
                  {
                    rc = false;
                  }
              }

            if (!rc)
              {
                this->close_connection ();

                this->handle_request_error(ftp_url);
              }
          }
        else
          {
            this->handle_connection_error (ftp_url);
          }

        return this->response_stream ();
      }

    void ClientRequestHandler::on_eof ()
      {
        this->finish_transfer ();
      }

    bool ClientRequestHandler::initialize_connection (const ACE_CString& host,
                                                      u_short port)
      {
        static const SessionFactory session_factory;

        ACE::INet::ConnectionHolder* pch = 0;
        if (this->connection_cache ().claim_connection (INetConnectionKey (host, port),
                                                        pch,
                                                        session_factory))
          {
            this->session (dynamic_cast<SessionHolder*> (pch));
            return true;
          }
        else
          return false;
      }

    bool ClientRequestHandler::handle_credentials (const URL& url,
                                                   ACE_CString& user,
                                                   ACE_CString& password)
      {
        Authentication authentication (url.get_host(),
                                       user,
                                       password);
        return URL::authenticate (authentication);
      }

    void ClientRequestHandler::handle_request_error (const URL& /*url*/)
      {
      }

    void ClientRequestHandler::handle_connection_error (const URL& /*url*/)
      {
      }

    void ClientRequestHandler::release_connection ()
      {
        if (this->session_)
          {
            this->connection_cache ().release_connection (
                INetConnectionKey (this->session ()->get_host (),
                                   this->session ()->get_port ()),
                this->session_);
            this->session_ = 0;
          }
      }

    void ClientRequestHandler::close_connection ()
      {
        if (this->session_)
          {
            this->connection_cache ().close_connection (
                INetConnectionKey (this->session ()->get_host (),
                                   this->session ()->get_port ()),
                this->session_);
            this->session_ = 0;
          }
      }

    void ClientRequestHandler::quit_connection ()
      {
        this->logout ();
        this->close_connection ();
      }

    bool ClientRequestHandler::logout ()
      {
        bool rc = true;
        if (this->session ()->is_connected ())
          {
            try
              {
                this->finish_transfer ();
              }
            catch (...)
              {
              }
            this->process_command (Request::FTP_QUIT);
            rc = this->response_.is_completed_ok ();
            this->current_user_ = anonymous_user_;
            this->session ()->close ();
          }
        return rc;
      }

    bool ClientRequestHandler::login (const ACE_CString& user,
                                      const ACE_CString& password)
      {
        // read startup message
        this->session ()->receive_response (this->response_);
        if (this->response_.is_completed_ok ())
          {
            // send user name
            this->process_command (Request::FTP_USER, user);
            if (this->response_.is_intermediate_ok ())
              {
                this->process_command (Request::FTP_PASS, password);
              }
          }
        return this->response_.is_completed_ok ();
      }

    bool ClientRequestHandler::is_valid_path (const ACE_CString& path)
      {
        return this->process_command (Request::FTP_STAT, path) == Response::COMPLETED_OK;
      }

    bool ClientRequestHandler::is_file (const ACE_CString& path)
      {
        return (this->is_valid_path (path) &&
                    this->process_command (Request::FTP_CWD, path) != Response::COMPLETED_OK);
      }

    bool ClientRequestHandler::is_dir (const ACE_CString& path)
      {
        return (this->is_valid_path (path) &&
                    this->process_command (Request::FTP_CWD, path) == Response::COMPLETED_OK);
      }

    bool ClientRequestHandler::set_filetype (bool binary)
      {
        return this->process_command (Request::FTP_TYPE, binary ? "I" : "A") == Response::COMPLETED_OK;
      }

    ClientRequestHandler::stream_type*
    ClientRequestHandler::start_download (const ACE_CString& path, bool binary)
      {
        if (path.empty () || this->is_dir (path))
          {
            if (this->set_filetype (false))
              {
                return this->open_data_connection (Request::FTP_LIST, path);
              }
          }
        else
          {
            if (this->set_filetype (binary))
              {
                return this->open_data_connection (Request::FTP_RETR, path);
              }
          }
        return 0;
      }

    ClientRequestHandler::stream_type*
    ClientRequestHandler::start_upload (const ACE_CString& path, bool binary)
      {
        if (this->set_filetype (binary))
          {
            return this->open_data_connection (Request::FTP_STOR, path);
          }
        return 0;
      }

    bool ClientRequestHandler::finish_transfer ()
      {
        if (transfer_active_)
          {
            stream_type* old_stream = dynamic_cast<stream_type*> (this->in_data_stream_.set_stream (0));
            if (old_stream)
              {
                // decrease ref count to 1
                const_cast<connection_type&> (old_stream->stream ()).remove_reference ();
                // also removes last ref count on stream handler
                delete old_stream;
              }
            old_stream = dynamic_cast<stream_type*> (this->out_data_stream_.set_stream (0));
              {
                // decrease ref count to 1
                const_cast<connection_type&> (old_stream->stream ()).remove_reference ();
                // also removes last ref count on stream handler
                delete old_stream;
              }

            this->transfer_active_ = false;

            this->session ()->receive_response (this->response_);
            return this->response_.is_completed_ok ();
          }
        return true;
      }

    bool ClientRequestHandler::abort_transfer ()
      {
        if (transfer_active_)
          {
            this->session ()->send_interrupt ();
            this->process_command (Request::FTP_ABOR);
            if (this->response_.status () == 426)
              this->session ()->receive_response (this->response_);

            stream_type* old_stream = dynamic_cast<stream_type*> (this->in_data_stream_.set_stream (0));
              {
                // decrease ref count to 1
                const_cast<connection_type&> (old_stream->stream ()).remove_reference ();
                // also removes last ref count on stream handler
                delete old_stream;
              }
            old_stream = dynamic_cast<stream_type*> (this->out_data_stream_.set_stream (0));
              {
                // decrease ref count to 1
                const_cast<connection_type&> (old_stream->stream ()).remove_reference ();
                // also removes last ref count on stream handler
                delete old_stream;
              }

            this->transfer_active_ = false;

            return this->response_.is_completed_ok ();
          }
        return true;
      }

    Response::StatusType
    ClientRequestHandler::process_command (const ACE_CString& cmd,
                                           const ACE_CString& arg)
      {
        if (this->session ()->send_request (this->request_(cmd) << arg))
          {
            this->session ()->receive_response (this->response_);
          }
        else
          {
            this->response_(0);
          }
        return this->response_.status_type ();
      }

    ClientRequestHandler::stream_type*
    ClientRequestHandler::open_data_connection (const ACE_CString& cmd,
                                                const ACE_CString& arg)
      {
        if (this->use_passive_mode_)
          {
            // get address for passive data connection
            ACE_INET_Addr data_addr;
            if (this->get_passive_address (data_addr))
              {
                // establish data connection

                // copy sync settings from session
                unsigned long f_reactor =
                    this->session ()->is_reactive() ? ACE_Synch_Options::USE_REACTOR : 0;
                ACE_Synch_Options sync_opt (ACE_Synch_Options::USE_TIMEOUT | f_reactor,
                                            this->session ()->timeout ());

                typedef ACE_Connector<SessionHolder::session_type::connection_type,
                                      ACE_SOCK_CONNECTOR> connector_type;
                connector_type connector;

                // create connection object (stream handler)
                SessionHolder::session_type::connection_type* data_connection = 0;
                ACE_NEW_NORETURN (data_connection,
                                SessionHolder::session_type::connection_type(sync_opt));
                if (data_connection == 0)
                  {
                    return 0;
                  }

                // connect to data connection address
                if (connector.connect (data_connection,
                                       data_addr,
                                       ACE_Synch_Options (0,
                                                          this->session ()->timeout ())) == -1)
                  {
                    INET_ERROR (1, (LM_ERROR, DLINFO
                                ACE_TEXT ("(%d) ACE_FTP_ClientRequestHandler::open_data_connection - ")
                                ACE_TEXT ("failed to connect to %C:%d\n"),
                                ACE_OS::last_error (),
                                data_addr.get_host_name (),
                                data_addr.get_port_number ()));
                    // as the connection was dynamically allocated
                    // the connector causes it to be destroyed after
                    // the connection failure
                    return 0;
                  }

                // enable ref counting so we can control when to destroy
                data_connection->reference_counting_policy ().value (
                    ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

                // create io stream for connection
                stream_type* data_stream = 0;
                ACE_NEW_NORETURN (data_stream,
                                  stream_type (data_connection));
                if (data_stream)
                  {
                    if (this->process_command (cmd, arg) == Response::PRELIM_OK)
                      return data_stream;

                    delete data_stream; // decreases ref count on connection
                  }
                // remove last ref count -> delete
                data_connection->remove_reference ();
              }
          }
        else
          {
            // address for active data connection
            ACE_INET_Addr data_addr;
            this->session ()->get_local_addr (data_addr);
            data_addr.set_port_number (this->active_port_);

            // copy sync settings from session
            unsigned long f_reactor =
                this->session ()->is_reactive() ? ACE_Synch_Options::USE_REACTOR : 0;
            ACE_Synch_Options sync_opt (ACE_Synch_Options::USE_TIMEOUT | f_reactor,
                                        this->session ()->timeout ());

            typedef ACE_Oneshot_Acceptor<SessionHolder::session_type::connection_type,
                                         ACE_SOCK_ACCEPTOR> acceptor_type;
            acceptor_type acceptor;

            // start data connection acceptor listening and retrieve actual listening address
            if (acceptor.open (data_addr) == 0 &&
                acceptor.acceptor ().get_local_addr (data_addr) == 0)
              {
                // send listen address to peer followed by data command to execute
                if (this->send_active_address (data_addr) &&
                    this->process_command (cmd, arg) == Response::PRELIM_OK)
                  {
                    // create connection object (stream handler)
                    SessionHolder::session_type::connection_type* data_connection = 0;
                    ACE_NEW_NORETURN (data_connection,
                                    SessionHolder::session_type::connection_type(sync_opt));
                    if (data_connection == 0)
                      {
                        return 0;
                      }

                    // accept data connection from peer
                    if (acceptor.accept (data_connection,
                                         0,
                                         ACE_Synch_Options (ACE_Synch_Options::USE_TIMEOUT,
                                                            this->session ()->timeout ())) == -1)
                      {
                        INET_ERROR (1, (LM_ERROR, DLINFO
                                    ACE_TEXT ("(%d) ACE_FTP_ClientRequestHandler::open_data_connection - ")
                                    ACE_TEXT ("failed to accept connection to %C:%d\n"),
                                    ACE_OS::last_error (),
                                    data_addr.get_host_name (),
                                    data_addr.get_port_number ()));

                        // as the connection was dynamically allocated
                        // the acceptor causes it to be destroyed after
                        // the connection failure
                        return 0;
                      }

                    // enable ref counting so we can control when to destroy
                    data_connection->reference_counting_policy ().value (
                        ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

                    // create io stream for connection
                    stream_type* data_stream = 0;
                    ACE_NEW_NORETURN (data_stream,
                                      stream_type (data_connection));
                    if (data_stream)
                      {
                        return data_stream;
                      }
                    // remove last ref count -> delete
                    data_connection->remove_reference ();
                  }
              }
          }
        return 0;
      }


    bool
    ClientRequestHandler::get_passive_address (ACE_INET_Addr& addr)
      {
        // can we use the RFC 2428 extensions?
        if (this->session ()->supports_ftp_extensions ())
          {
            if (this->process_command (Request::FTP_EPSV) == Response::COMPLETED_OK)
              {
                return this->parse_ext_address (this->response_.response ()[0],
                                            addr);
              }
            // flag RFC 2428 extensions as unsupported
            this->session ()->set_ftp_extension_support (false);
          }
        // use 'old' command
        if (this->process_command (Request::FTP_PASV) == Response::COMPLETED_OK)
          {
            return this->parse_address (this->response_.response ()[0],
                                        addr);
          }
        return false;
      }

    bool
    ClientRequestHandler::parse_address (const ACE_CString& str, ACE_INET_Addr& addr)
      {
        static const int eof_ = std::char_traits<char>::eof ();

        ACE::IOS::CString_OStream sos_host;
        u_short port_hi = 0, port_lo = 0;

        ACE::IOS::CString_IStream sis (str);
        sis.ignore (str.length (), '(');
        int ch = sis.get ();
        if (ACE_OS::ace_isdigit (ch))
          {
            for (int i=0; i<4 ;++i)
              {
                if (ch == ',')
                  {
                    sos_host.put ('.');
                    ch = sis.get ();
                  }
                while (ch != eof_ && ACE_OS::ace_isdigit (ch))
                  {
                    sos_host.put (ch);
                    ch = sis.get ();
                  }
              }
            if (ch == ',')
              {
                sis >> port_hi;
                ch = sis.get ();
                if (ch == ',')
                  {
                    sis >> port_lo;

                    u_short port = port_hi*256 + port_lo;
                    addr.set (port, sos_host.str ().c_str ());
                    return true;
                  }
              }
          }
        return false;
      }

    bool
    ClientRequestHandler::parse_ext_address (const ACE_CString& str, ACE_INET_Addr& addr)
      {
        static const int eof_ = std::char_traits<char>::eof ();

        ACE::IOS::CString_IStream sis (str);
        sis.ignore (str.length (), '(');
        int ch = sis.get ();
        if (ch != eof_)
          {
            int delim = ch;
            sis.ignore (str.length (), delim);
            sis.ignore (str.length (), delim);
            if (sis.peek () != eof_)
              {
                u_short port = 0;
                sis >> port;
                addr.set (port, this->session ()->get_host ().c_str ());
                return true;
              }
          }
        return false;
      }

    bool
    ClientRequestHandler::send_active_address (const ACE_INET_Addr& addr)
      {
        ACE::IOS::CString_OStream arg;
        char ip_buf[128];
        if (addr.get_host_addr (ip_buf, sizeof(ip_buf)) == 0)
          {
            return false;
          }
        u_short port = addr.get_port_number ();
        // can we use the RFC 2428 extensions?
        if (this->session ()->supports_ftp_extensions ())
          {
            arg << '|'
                << (addr.get_type () == AF_INET ? '1' : '2')
                << '|'
                << (&ip_buf[0])
                << '|'
                << port
                << '|';
            if (this->process_command (Request::FTP_EPRT, arg.str ()) == Response::COMPLETED_OK)
              {
                return true;
              }
            // flag RFC 2428 extensions as unsupported
            this->session ()->set_ftp_extension_support (false);

            arg.clear ();
          }
        // use 'old' command
        ACE_CString ip_addr = addr.get_host_addr (ip_buf, sizeof(ip_buf));
        ACE_CString::size_type pos;
        while ((pos = ip_addr.find ('.')) != ACE_CString::npos)
          {
            ip_addr[pos] = ',';
          }
        arg << ip_addr << ','
            << (port / 256)   // port hi
            << ','
            << (port % 256);  // port lo
        return (this->process_command (Request::FTP_PORT, arg.str ()) == Response::COMPLETED_OK);
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
