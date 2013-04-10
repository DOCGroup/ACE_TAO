// $Id$

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_errno.h"
#include "ace/INet/FTP_URL.h"
#include "ace/INet/FTP_ClientRequestHandler.h"
#include <iostream>
#include <fstream>

ACE_CString url;
ACE_CString outfile;
bool do_login = false;
ACE_CString password;
bool do_active = false;

void
usage (void)
{
  std::cout << "usage: ftp_simple_wget [options] <url>\n";
  std::cout << "Executes an FTP download request and sends the result to STDOUT or file\n";
  std::cout << "\t-a\t\tuse active mode for data connections\n";
  std::cout << "\t-o <filename>\t\tfile to write output to\n";
  std::cout << "\t-l\t\task for password at login\n";
  std::cout << "\t-p <password>\t\tuse <password> for login\n";
}

bool
parse_args (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("ao:lp:h"), 0, 0, ACE_Get_Opt::RETURN_IN_ORDER);

  int c;
  ACE_CString s;
  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 1:
          url = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;

        case 'a':
          do_active = true;
          break;

        case 'o':
          outfile = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;

        case 'l':
          do_login = true;
          break;

        case 'p':
          password = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
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
    My_Authenticator () {}
    ~My_Authenticator () {}

    virtual bool authenticate(ACE::INet::AuthenticationBase& auth) const
      {
        if (!password.empty ()) auth.password (password);
        if (do_login)
          {
            std::cout << "Authentication (" << auth.scheme() << ") required." << std::endl;
            std::cout << "Realm : " << auth.realm () << std::endl
                      << "User: " << auth.user() << std::endl
                      << "Password: \t\t\t (Enter password or press enter for default)\rPassword: ";
            char buf[80] = {0};
            u_int n = 0;
            int ch = 0;
            while (n < (sizeof(buf)-1) && (ch = std::cin.get ()) != '\n' && ch != std::char_traits<char>::eof ())
              {
                buf[n++] = char(ch);
              }
            if (n>0)
              auth.password (buf);
          }
        return true;
      }
};

class My_FTP_RequestHandler
  : public ACE::FTP::ClientRequestHandler
{
  public:
    My_FTP_RequestHandler () : read_length_ (0) {}
    virtual ~My_FTP_RequestHandler () {}

  protected:
    virtual void handle_request_error (const ACE::FTP::URL& url)
      {
        std::cout << "ERROR" << std::endl;
        std::cerr << "Failed to handle download for " << url.to_string ().c_str () << std::endl;
      }

    virtual void handle_connection_error (const ACE::FTP::URL& url)
      {
        std::cout << "ERROR" << std::endl;
        std::cerr << "Failed to set up connection for " << url.to_string ().c_str () << std::endl;
      }

    virtual void after_read (const char_type* /*buffer*/, int length_read)
      {
        this->read_length_ += length_read;
        std::cout << "\r [" << this->read_length_ << "/???]";
      }

    virtual void on_eof ()
      {
        ACE::FTP::ClientRequestHandler::on_eof ();
        std::cout << std::endl;
      }

  private:
    int read_length_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  ACE_Auto_Ptr<std::ofstream> fout;
  std::ostream* sout = &std::cout;

  if (!parse_args (argc, argv))
    {
      return 1;
    }

  ACE::INet::URL_INetAuthBase::add_authenticator ("my_auth",
                                                  new My_Authenticator);

  std::cout << "Starting..." << std::endl;

  if (!url.empty ())
    {
      if (!outfile.empty ())
        {
          fout.reset (new std::ofstream (outfile.c_str (), std::ios_base::binary|std::ios_base::out));

          if (!*fout)
            {
              std::cerr << "Failed to open output file : " << outfile.c_str () << std::endl;
              return 1;
            }

          sout = fout.get ();
        }

      ACE::FTP::URL ftp_url;

      std::cout << "Parsing url [" << url.c_str () << "]" << std::endl;

      if (!ftp_url.parse (url))
        {
          std::cerr << "Failed parsing url [" << url << "]" << std::endl;
          std::cerr << "\tresult = " << ftp_url.to_string ().c_str ();
          return 1;
        }

      std::cout << "Opening url..." << std::endl;
      My_FTP_RequestHandler my_rh;
      if (do_active)  my_rh.use_active_mode ();
      ACE::INet::URLStream urlin = ftp_url.open (my_rh);
      if (urlin)
        {
          std::cout << "Saving to: ";
          if (!outfile.empty ())
            std::cout << '\'' << outfile.c_str () << '\'' << std::endl;
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
