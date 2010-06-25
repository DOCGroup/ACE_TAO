// $Id$

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_errno.h"
#include "HTTP_URL.h"
#include "HTTP_ClientRequestHandler.h"
#include <iostream>
#include <fstream>

ACE_CString proxy_hostname;
u_short proxy_port = ACE::HTTP::URL::HTTP_PROXY_PORT;
ACE_CString url;
ACE_CString outfile;

void
usage (void)
{
  std::cout << "usage: http_simple_wget [options] <url>\n";
  std::cout << "Executes an HTTP GET request and sends the result to STDOUT or file\n";
  std::cout << "\t-H <hostname>\t\tproxy host to connect to\n";
  std::cout << "\t-p <port>\t\tproxy port to connect to\n";
  std::cout << "\t-o <filename>\t\tfile to write output to\n";
}

bool
parse_args (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("H:p:o:h"), 0, 0, ACE_Get_Opt::RETURN_IN_ORDER);

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
    virtual void handle_response (const ACE::HTTP::URL& /*url*/, const ACE::HTTP::Response& response)
      {
        std::cout << "Received response "
                  << (int)response.get_status ().get_status ()
                  << " "
                  << response.get_status ().get_reason ().c_str ()
                  << std::endl;
        if (response.get_status ().is_ok ())
          {
            std::cout << "Length: ";
            if (response.get_content_length () != ACE::HTTP::Response::UNKNOWN_CONTENT_LENGTH)
              std::cout << response.get_content_length () << " [";
            else
              std::cout << "(unknown) [";
            if (response.get_content_type () != ACE::HTTP::Response::UNKNOWN_CONTENT_TYPE)
              std::cout << response.get_content_type ().c_str ();
            else
              std::cout << "(unknown)";
            std::cout  << "]" << std::endl;
            this->in_length_ = response.get_content_length ();
            this->read_length_ = 0;
          }
      }

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

      ACE::HTTP::URL http_url;

      std::cout << "Parsing url [" << url.c_str () << "]" << std::endl;

      if (!http_url.parse (url))
        {
          std::cerr << "Failed parsing url [" << url << "]" << std::endl;
          std::cerr << "\tresult = " << http_url.to_string ().c_str ();
          return 1;
        }

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
