#include "ace/Get_Opt.h"
#include "ace/INet/FTP_URL.h"
#include "ace/INet/FTP_ClientRequestHandler.h"
#include <array>
#include <cstddef>
#include <iostream>
#include <fstream>

struct FTP_Options
{
  ACE_CString url;
  ACE_CString outfile;
  ACE_CString password;
  bool do_login {};
  bool do_active {};
};

void
usage ()
{
  std::cout << "usage: ftp_simple_wget [options] <url>\n";
  std::cout << "Executes an FTP download request and sends the result to STDOUT or file\n";
  std::cout << "\t-a\t\tuse active mode for data connections\n";
  std::cout << "\t-o <filename>\t\tfile to write output to\n";
  std::cout << "\t-l\t\task for password at login\n";
  std::cout << "\t-p <password>\t\tuse <password> for login\n";
}

bool
parse_args (int argc, ACE_TCHAR *argv [], FTP_Options &options)
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("ao:lp:h"), 0, 0, ACE_Get_Opt::RETURN_IN_ORDER);

  int c {};
  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 1:
          options.url = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;

        case 'a':
          options.do_active = true;
          break;

        case 'o':
          options.outfile = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;

        case 'l':
          options.do_login = true;
          break;

        case 'p':
          options.password = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;

        case 'h':
        default:
          usage ();
          return false;
        }
    }

  return true;
}

class My_Authenticator
  : public ACE::INet::AuthenticatorBase
{
  public:
    explicit My_Authenticator (FTP_Options const &options)
      : options_ (options)
    {
    }

    My_Authenticator (My_Authenticator const &) = delete;
    My_Authenticator (My_Authenticator &&) = delete;
    My_Authenticator &operator= (My_Authenticator const &) = delete;
    My_Authenticator &operator= (My_Authenticator &&) = delete;

    bool authenticate (ACE::INet::AuthenticationBase &auth) const override
      {
        if (!this->options_.password.empty ())
          {
            auth.password (this->options_.password);
          }
        if (this->options_.do_login)
          {
            std::cout << "Authentication (" << auth.scheme () << ") required." << std::endl;
            std::cout << "Realm : " << auth.realm () << std::endl
                      << "User: " << auth.user () << std::endl
                      << "Password: \t\t\t (Enter password or press enter for default)\rPassword: ";
            std::array<char, 80> buf {};
            size_t n {};
            int ch {};
            while (n < buf.size () - 1 &&
                   (ch = std::cin.get ()) != '\n' &&
                   ch != std::char_traits<char>::eof ())
              {
                buf[n++] = static_cast<char> (ch);
              }
            if (n > 0)
              auth.password (buf.data ());
          }
        return true;
      }

  private:
    FTP_Options options_;
};

class My_FTP_RequestHandler
  : public ACE::FTP::ClientRequestHandler
{
  public:
    My_FTP_RequestHandler () = default;
    ~My_FTP_RequestHandler () override = default;

    My_FTP_RequestHandler (My_FTP_RequestHandler const &) = delete;
    My_FTP_RequestHandler (My_FTP_RequestHandler &&) = delete;
    My_FTP_RequestHandler &operator= (My_FTP_RequestHandler const &) = delete;
    My_FTP_RequestHandler &operator= (My_FTP_RequestHandler &&) = delete;

  protected:
    void handle_request_error (ACE::FTP::URL const &url) override
      {
        std::cout << "ERROR" << std::endl;
        std::cerr << "Failed to handle download for " << url.to_string ().c_str () << std::endl;
      }

    void handle_connection_error (ACE::FTP::URL const &url) override
      {
        std::cout << "ERROR" << std::endl;
        std::cerr << "Failed to set up connection for " << url.to_string ().c_str () << std::endl;
      }

    void after_read (char_type const * /*buffer*/, int length_read) override
      {
        this->read_length_ += static_cast<size_t> (length_read);
        std::cout << "\r [" << this->read_length_ << "/???]";
      }

    void on_eof () override
      {
        ACE::FTP::ClientRequestHandler::on_eof ();
        std::cout << std::endl;
      }

  private:
    size_t read_length_ {};
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  FTP_Options options {};
  std::ofstream fout;
  std::ostream *sout = &std::cout;

  if (!parse_args (argc, argv, options))
    {
      return 1;
    }

  ACE::INet::URL_INetAuthBase::add_authenticator (
    "my_auth",
    new My_Authenticator (options));

  std::cout << "Starting..." << std::endl;

  if (!options.url.empty ())
    {
      if (!options.outfile.empty ())
        {
          fout.open (options.outfile.c_str (), std::ios_base::binary | std::ios_base::out);

          if (!fout)
            {
              std::cerr << "Failed to open output file : " << options.outfile.c_str () << std::endl;
              return 1;
            }

          sout = &fout;
        }

      ACE::FTP::URL ftp_url;

      std::cout << "Parsing url [" << options.url.c_str () << "]" << std::endl;

      if (!ftp_url.parse (options.url))
        {
          std::cerr << "Failed parsing url [" << options.url << "]" << std::endl;
          std::cerr << "\tresult = " << ftp_url.to_string ().c_str ();
          return 1;
        }

      std::cout << "Opening url..." << std::endl;
      My_FTP_RequestHandler my_rh;
      if (options.do_active)
        {
          my_rh.use_active_mode ();
        }
      ACE::INet::URLStream urlin = ftp_url.open (my_rh);
      if (urlin)
        {
          std::cout << "Saving to: ";
          if (!options.outfile.empty ())
            std::cout << '\'' << options.outfile.c_str () << '\'' << std::endl;
          else
            std::cout << "(stdout)" << std::endl;

          (*sout) << urlin->rdbuf ();
          sout->flush ();
        }
    }
  else
    {
      std::cerr << "ERROR: No URL specified!" << std::endl;
      usage ();
      return 1;
    }

  std::cout << "Done" << std::endl;

  return 0;
}
