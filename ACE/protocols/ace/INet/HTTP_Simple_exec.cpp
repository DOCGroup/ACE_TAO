// $Id$

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_errno.h"
#include "ace/INet/HTTP_URL.h"
#include "ace/INet/HTTP_ClientRequestHandler.h"
#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1
# include "ace/INet/SSL_CallbackManager.h"
# include "ace/INet/HTTPS_Context.h"
#endif
#include "ace/INet/INet_Log.h"
#include <iostream>
#include <fstream>

ACE_CString proxy_hostname;
u_short proxy_port = ACE::HTTP::URL::HTTP_PROXY_PORT;
ACE_CString url;
ACE_CString outfile;
#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1
int ssl_mode = ACE_SSL_Context::SSLv3;
bool verify_peer = true;
bool ignore_verify = false;
ACE_CString certificate;
ACE_CString private_key;
ACE_CString ca_location;
#endif

void
usage (void)
{
  std::cout << "usage: http_simple_wget [options] <url>\n";
  std::cout << "Executes an HTTP GET request and sends the result to STDOUT or file\n";
  std::cout << "\t-H <hostname>   \t\tproxy host to connect to\n";
  std::cout << "\t-p <port>       \t\tproxy port to connect to\n";
  std::cout << "\t-o <filename>   \t\tfile to write output to\n";
#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1
  std::cout << "\t-v <ssl version>\t\tSSL version to use: ";
#if !defined (OPENSSL_NO_SSL2)
  std::cout << "2, ";
#endif /* OPENSSL_NO_SSL2 */
  std::cout << "23, 3\n";
  std::cout << "\t-n              \t\tno peer certificate verification\n";
  std::cout << "\t-i              \t\tignore peer certificate verification failures\n";
  std::cout << "\t-c <filename>   \t\tcertificate file (PEM format)\n";
  std::cout << "\t-k <filename>   \t\tprivate key file (PEM format); requires -c\n";
  std::cout << "\t-C <path>       \t\ttrusted CA file or directory\n";
#endif
}

bool
parse_args (int argc, ACE_TCHAR *argv [])
{
#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("H:p:o:hv:nic:k:C:"), 0, 0, ACE_Get_Opt::RETURN_IN_ORDER);
#else
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("H:p:o:h"), 0, 0, ACE_Get_Opt::RETURN_IN_ORDER);
#endif

  int c;
  ACE_CString s;
  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 1:
          url = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;
        case 'H':
          proxy_hostname = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;

        case 'p':
          proxy_port = (u_short)ACE_OS::atoi (ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ()));
          break;

        case 'o':
          outfile = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;

#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1
        case 'v':
          {
            ACE_CString ver = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
            if (ver == "23")
              ssl_mode = ACE_SSL_Context::SSLv23;
#if !defined (OPENSSL_NO_SSL2)
            else if (ver == "2")
              ssl_mode = ACE_SSL_Context::SSLv2;
#endif /* ! OPENSSL_NO_SSL2*/
            else if (ver != "3") // default mode
              {
                std::cerr << "ERROR: Invalid SSL mode [" << ver << "] specfied!" << std::endl;
                return false;
              }
          }
          break;

        case 'n':
          verify_peer = false;
          break;

        case 'i':
          ignore_verify = true;
          break;

        case 'c':
          certificate = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;

        case 'k':
          private_key = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;

        case 'C':
          ca_location = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;
#endif
        case 'h':
        default:
          usage ();
          return false;
        }
    }

  return true;
}

class My_HTTP_RequestHandler
  : public ACE::HTTP::ClientRequestHandler
{
  public:
    My_HTTP_RequestHandler () : in_length_ (0), read_length_ (0) {}
    virtual ~My_HTTP_RequestHandler () {}

  protected:
    virtual void handle_request_error (const ACE::HTTP::URL& url)
      {
        std::cout << "ERROR" << std::endl;
        std::cerr << "Failed to handle request for " << url.to_string ().c_str () << std::endl;
      }

    virtual void handle_connection_error (const ACE::HTTP::URL& url)
      {
        std::cout << "ERROR" << std::endl;
        std::cerr << "Failed to set up connection for " << url.to_string ().c_str () << std::endl;
      }

    virtual void after_read (const char_type* /*buffer*/, int length_read)
      {
        if (this->read_length_ == 0)
          {
            this->in_length_ = this->response ().get_content_length ();
          }
        this->read_length_ += length_read;
        std::cout << "\r [" << this->read_length_ << '/';
        if (this->in_length_ != ACE::HTTP::Response::UNKNOWN_CONTENT_LENGTH)
          {
            std::cout << this->in_length_ << "] " << ((this->read_length_ * 100) / this->in_length_) << "%";
          }
        else
          std::cout << "???]";
        std::cout.flush ();
      }

    virtual void on_eof ()
      {
        ACE::HTTP::ClientRequestHandler::on_eof ();
        std::cout << std::endl;
        this->read_length_ = 0;
      }

  private:
    int in_length_;
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

#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1
  ACE::HTTPS::Context::set_default_ssl_mode (ssl_mode);
  ACE::HTTPS::Context::set_default_verify_mode (verify_peer);
  ACE::HTTPS::Context::instance ().use_default_ca ();
  if (!private_key.empty ())
    {
      if (certificate.empty ())
        {
          std::cerr << "ERROR: private key file [" << private_key << "] requires certificate file to be specified." << std::endl;
          return 1;
        }
      if (!ACE::HTTPS::Context::instance ().set_key_files (private_key.c_str (), certificate.c_str ()))
        {
          std::cerr << "ERROR: failed to set private key [" << private_key << "]." << std::endl;
          return 1;
        }
    }
  if (!ca_location.empty ())
    {
      INET_DEBUG (6, (LM_INFO, DLINFO ACE_TEXT ("loading trusted CA [%C]\n"), ca_location.c_str ()));
      if (!ACE::HTTPS::Context::instance ().load_trusted_ca (ca_location.c_str ()))
        {
          std::cerr << "ERROR: failed to load trusted CA from [" << ca_location << "]." << std::endl;
          return 1;
        }
      INET_DEBUG (6, (LM_INFO, DLINFO ACE_TEXT ("loaded [%d] trusted CA\n"), ACE::HTTPS::Context::instance ().has_trusted_ca ()));
    }
  if (ignore_verify)
    ACE::INet::SSL_CallbackManager::instance ()->set_certificate_callback (new ACE::INet::SSL_CertificateAcceptor);
#endif

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

      std::cout << "Parsing url [" << url.c_str () << "]" << std::endl;

      ACE_Auto_Ptr<ACE::INet::URL_Base> url_safe (ACE::INet::URL_Base::create_from_string (url));

      if (url_safe.get () == 0 || url != url_safe->to_string ())
        {
          std::cerr << "Failed parsing url [" << url << "]" << std::endl;
          std::cerr << "\tresult = " << (url_safe.get () == 0 ? "(null)" : url_safe->to_string ().c_str ()) << std::endl;
          return 1;
        }

      ACE::HTTP::URL& http_url = *dynamic_cast<ACE::HTTP::URL*> (url_safe.get ());

      if (!proxy_hostname.empty ())
        {
          std::cout << "Setting proxy: " << proxy_hostname.c_str () << ':' << proxy_port << std::endl;
          http_url.set_proxy (proxy_hostname, proxy_port);
        }

      std::cout << "Opening url...";
      My_HTTP_RequestHandler my_rh;
      ACE::INet::URLStream urlin = http_url.open (my_rh);
      if (urlin)
        {
          std::cout << "Received response "
                    << (int)my_rh.response ().get_status ().get_status ()
                    << " "
                    << my_rh.response ().get_status ().get_reason ().c_str ()
                    << std::endl;
          if (my_rh.response ().get_status ().is_ok ())
            {
              std::cout << "Length: ";
              if (my_rh.response ().get_content_length () != ACE::HTTP::Response::UNKNOWN_CONTENT_LENGTH)
                std::cout << my_rh.response ().get_content_length () << " [";
              else
                std::cout << "(unknown) [";
              if (my_rh.response ().get_content_type () != ACE::HTTP::Response::UNKNOWN_CONTENT_TYPE)
                std::cout << my_rh.response ().get_content_type ().c_str ();
              else
                std::cout << "(unknown)";
              std::cout  << "]" << std::endl;
            }

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
