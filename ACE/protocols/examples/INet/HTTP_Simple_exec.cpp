#include "ace/Get_Opt.h"
#include "ace/INet/HTTP_URL.h"
#include "ace/INet/HTTP_ClientRequestHandler.h"
#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1
# include "ace/INet/SSL_CallbackManager.h"
# include "ace/INet/HTTPS_Context.h"
#endif
#include "ace/INet/INet_Log.h"
#include <cstddef>
#include <iostream>
#include <fstream>
#include <memory>

struct HTTP_Options
{
  ACE_CString proxy_hostname;
  u_short proxy_port { ACE::HTTP::URL::HTTP_PROXY_PORT };
  ACE_CString url;
  ACE_CString outfile;
#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1
  int ssl_mode { ACE_SSL_Context::SSLv23 };
  bool verify_peer { true };
  bool ignore_verify {};
  ACE_CString certificate;
  ACE_CString private_key;
  ACE_CString ca_location;
#endif
};

void
usage ()
{
  std::cout << "usage: http_simple_wget [options] <url>\n";
  std::cout << "Executes an HTTP GET request and sends the result to STDOUT or file\n";
  std::cout << "\t-H <hostname>   \t\tproxy host to connect to\n";
  std::cout << "\t-p <port>       \t\tproxy port to connect to\n";
  std::cout << "\t-o <filename>   \t\tfile to write output to\n";
#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1
  std::cout << "\t-n              \t\tno peer certificate verification\n";
  std::cout << "\t-i              \t\tignore peer certificate verification failures\n";
  std::cout << "\t-c <filename>   \t\tcertificate file (PEM format)\n";
  std::cout << "\t-k <filename>   \t\tprivate key file (PEM format); requires -c\n";
  std::cout << "\t-C <path>       \t\ttrusted CA file or directory\n";
#endif
}

bool
parse_args (int argc, ACE_TCHAR *argv [], HTTP_Options &options)
{
#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("H:p:o:hv:nic:k:C:"), 0, 0, ACE_Get_Opt::RETURN_IN_ORDER);
#else
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("H:p:o:h"), 0, 0, ACE_Get_Opt::RETURN_IN_ORDER);
#endif

  int c {};
  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 1:
          options.url = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;
        case 'H':
          options.proxy_hostname = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;

        case 'p':
          options.proxy_port = static_cast<u_short> (
            ACE_OS::atoi (ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ())));
          break;

        case 'o':
          options.outfile = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;

#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1
        case 'n':
          options.verify_peer = false;
          break;

        case 'i':
          options.ignore_verify = true;
          break;

        case 'c':
          options.certificate = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;

        case 'k':
          options.private_key = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;

        case 'C':
          options.ca_location = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
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
    My_HTTP_RequestHandler () = default;
    ~My_HTTP_RequestHandler () override = default;

    My_HTTP_RequestHandler (My_HTTP_RequestHandler const &) = delete;
    My_HTTP_RequestHandler (My_HTTP_RequestHandler &&) = delete;
    My_HTTP_RequestHandler &operator= (My_HTTP_RequestHandler const &) = delete;
    My_HTTP_RequestHandler &operator= (My_HTTP_RequestHandler &&) = delete;

  protected:
    void handle_request_error (ACE::HTTP::URL const &url) override
      {
        std::cout << "ERROR" << std::endl;
        std::cerr << "Failed to handle request for " << url.to_string ().c_str () << std::endl;
      }

    void handle_connection_error (ACE::HTTP::URL const &url) override
      {
        std::cout << "ERROR" << std::endl;
        std::cerr << "Failed to set up connection for " << url.to_string ().c_str () << std::endl;
      }

    void after_read (char_type const * /*buffer*/, int length_read) override
      {
        if (this->read_length_ == 0)
          {
            this->in_length_ = this->response ().get_content_length ();
          }
        this->read_length_ += static_cast<size_t> (length_read);
        std::cout << "\r [" << this->read_length_ << '/';
        if (this->in_length_ > 0)
          {
            std::cout << this->in_length_ << "] "
                      << ((this->read_length_ * 100) /
                          static_cast<size_t> (this->in_length_)) << "%";
          }
        else
          std::cout << "???]";
        std::cout.flush ();
      }

    void on_eof () override
      {
        ACE::HTTP::ClientRequestHandler::on_eof ();
        std::cout << std::endl;
        this->read_length_ = 0;
      }

  private:
    int in_length_ {};
    size_t read_length_ {};
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  HTTP_Options options {};
  std::ofstream fout;
  std::ostream *sout = &std::cout;

  if (!parse_args (argc, argv, options))
    {
      return 1;
    }

#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1
  ACE::HTTPS::Context::set_default_ssl_mode (options.ssl_mode);
  ACE::HTTPS::Context::set_default_verify_mode (options.verify_peer);
  ACE::HTTPS::Context::instance ().use_default_ca ();
  if (!options.private_key.empty ())
    {
      if (options.certificate.empty ())
        {
          std::cerr << "ERROR: private key file [" << options.private_key
                    << "] requires certificate file to be specified." << std::endl;
          return 1;
        }
      if (!ACE::HTTPS::Context::instance ().set_key_files (
            options.private_key.c_str (), options.certificate.c_str ()))
        {
          std::cerr << "ERROR: failed to set private key [" << options.private_key << "]." << std::endl;
          return 1;
        }
    }
  if (!options.ca_location.empty ())
    {
      INET_DEBUG (6, (LM_INFO, DLINFO ACE_TEXT ("loading trusted CA [%C]\n"),
                      options.ca_location.c_str ()));
      if (!ACE::HTTPS::Context::instance ().load_trusted_ca (options.ca_location.c_str ()))
        {
          std::cerr << "ERROR: failed to load trusted CA from [" << options.ca_location << "]." << std::endl;
          return 1;
        }
      INET_DEBUG (6, (LM_INFO, DLINFO ACE_TEXT ("loaded [%d] trusted CA\n"),
                      ACE::HTTPS::Context::instance ().has_trusted_ca ()));
    }
  if (options.ignore_verify)
    ACE::INet::SSL_CallbackManager::instance ()->set_certificate_callback (new ACE::INet::SSL_CertificateAcceptor);
#endif

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

      std::cout << "Parsing url [" << options.url.c_str () << "]" << std::endl;

      std::unique_ptr<ACE::INet::URL_Base> url_safe (
        ACE::INet::URL_Base::create_from_string (options.url));

      if (!url_safe || options.url != url_safe->to_string ())
        {
          std::cerr << "Failed parsing url [" << options.url << "]" << std::endl;
          std::cerr << "\tresult = "
                    << (url_safe ? url_safe->to_string ().c_str () : "(null)") << std::endl;
          return 1;
        }

      ACE::HTTP::URL * const http_url = dynamic_cast<ACE::HTTP::URL *> (url_safe.get ());
      if (http_url == nullptr)
        {
          std::cerr << "URL is not an HTTP URL: " << options.url << std::endl;
          return 1;
        }

      if (!options.proxy_hostname.empty ())
        {
          std::cout << "Setting proxy: " << options.proxy_hostname.c_str ()
                    << ':' << options.proxy_port << std::endl;
          http_url->set_proxy (options.proxy_hostname, options.proxy_port);
        }

      std::cout << "Opening url...";
      My_HTTP_RequestHandler my_rh;
      ACE::INet::URLStream urlin = http_url->open (my_rh);
      if (urlin)
        {
          std::cout << "Received response "
                    << static_cast<int> (my_rh.response ().get_status ().get_status ())
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
