// $Id$

#include "ace/Process.h"
#include "ace/Mem_Map.h"

#include "HTTP_Response.h"
#include "HTTP_Request.h"
#include "HTTP_Helpers.h"
#include "HTTP_Config.h"
#include "IO.h"

static char * const EMPTY_HEADER = (char *)"";

HTTP_Response::HTTP_Response (JAWS_IO &io, HTTP_Request &request)
  : io_(io), request_(request)
{
}

HTTP_Response::HTTP_Response (HTTP_Request &request, JAWS_IO &io)
  : io_(io), request_(request)
{
}

HTTP_Response::~HTTP_Response (void)
{
  //  if (this->HTTP_HEADER != EMPTY_HEADER)
  //    delete [] this->HTTP_HEADER;
  // The [] is important.  Without it, there was a huge memory leak!
}

void
HTTP_Response::process_request(HTTP_Response &response)
{
  response.process_request();
}

void
HTTP_Response::process_request (void)
{
  ACE_DEBUG ((LM_DEBUG, "  (%t) processing request: %s\n",
              this->request_.status_string ()));

 switch (this->request_.status ())
    {
    case HTTP_Status_Code::STATUS_OK :

      if (this->request_.cgi ())
        {
          this->cgi_response ();
        }
      else
        {
          this->normal_response ();
        }

      break;

    default:
      this->error_response (this->request_.status (),
                            this->request_.status_string ());
    }
}

void
HTTP_Response::error_response (int status_code, const char *log_message)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) [%s %s %s] %s\n",
              this->request_.method () ? this->request_.method () : "-",
              this->request_.uri () ? this->request_.uri () : "-",
              this->request_.version() ? this->request_.version () : "-",
              log_message ? log_message : "-"));

  static char const error_header1[] =
    "%s %d %s\r\n"
    "Server: JAWS/1.0prebeta\r\n"
    "Content-type: text/html\r\n"
    "Content-length: %d\r\n"
    "\r\n"
    "%s"
    ;

  static char const error_header2[] =
    "%s %d %s\r\n"
    "Server: JAWS/1.0prebeta\r\n"
    "WWW-Authenticate: Basic realm=\"JAWS_authorization\"\r\n"
    "Content-type: text/html\r\n"
    "Content-length: %d\r\n"
    "\r\n"
    "%s"
    ;

  static char const error_message[] =
    "<html>\n"
    "<head><title>Server error message</title></head>\n"
    "<body>\n"
    "<h1>Error %d: %s</h1>\n"
    "The request could not be completed because:\n %s\n"
    "</body>\n"
    "</html>\n"
    ;


  char *buf;
  char buf1[4 * BUFSIZ];
  char buf2[BUFSIZ];

  int length;
  const char *error_header = error_header1;

  if (status_code == HTTP_Status_Code::STATUS_UNAUTHORIZED)
    error_header = error_header2;

  length =
    ACE_OS::sprintf (buf2, error_message,
                     status_code, HTTP_Status_Code::instance ()[status_code],
                     log_message);

  if (this->request_.version () == 0
      || ACE_OS::strcmp ("HTTP/0.9", this->request_.version ()) == 0)
    buf = buf2;
  else
    {
      length =
        ACE_OS::sprintf (buf1, error_header,
                         this->request_.version(), status_code,
                         HTTP_Status_Code::instance()[status_code],
                         length,
                         buf2);
      buf = buf1;
    }
          
  this->io_.send_error_message (buf, length);
}

void
HTTP_Response::normal_response (void)
{
  const char *hv = 0;;

  ACE_DEBUG ((LM_DEBUG, " (%t) %s request for %s [%s], version %s\n",
              request_.method (), request_.uri (), request_.path (),
              (request_.version () ? request_.version () : "HTTP/0.9")));

  switch (this->request_.type ())
    {
    case HTTP_Request::GET :

      this->build_headers ();
      this->io_.transmit_file (this->request_.path (), 
                               this->HTTP_HEADER,
                               this->HTTP_HEADER_LENGTH,
                               this->HTTP_TRAILER, 
                               this->HTTP_TRAILER_LENGTH);
      break;

    case HTTP_Request::HEAD :
      this->build_headers ();
      this->io_.send_confirmation_message (this->HTTP_HEADER,
                                           this->HTTP_HEADER_LENGTH);
      break;

    case HTTP_Request::POST :
      // What to do here?
      // Standard says this is implementation dependent.
      // Examples: annotations, page updates, etc.
      // This may be a good place to stick CORBA stuff,
      // and mobile code.
      this->error_response (HTTP_Status_Code::STATUS_NOT_IMPLEMENTED,
                            "Requested method is not implemented.");
      break;

    case HTTP_Request::PUT :
      // Only commit to this if we can authenticate it

      // if there is no Authentication: header on the incoming request,
      // deny it
      hv = this->request_.headers ()["Authorization"];
      if (hv == 0 || *hv == '\0')
        this->error_response (HTTP_Status_Code::STATUS_UNAUTHORIZED,
                              "Unauthorized to use PUT method");
      else if (ACE_OS::strncmp (hv, "Basic ", 6) != 0)
        this->error_response (HTTP_Status_Code::STATUS_UNAUTHORIZED,
                              "Unknown authroization method");
      else
        {
          ACE_Mem_Map mmapfile;
	  // James, please document where the value 6 comes from (up above too).
          const char *hvv = hv + 6;
          char *buf = new char [ACE_OS::strlen (hv)];
          char *auth
            = HTTP_Helper::HTTP_decode_base64 (ACE_OS::strcpy (buf, hvv));

          if (mmapfile.map ("jaws.auth") != -1 
	      && auth != 0
              && ACE_OS::strstr((const char *) mmapfile.addr (), auth) != 0)
            this->io_.receive_file (this->request_.path (),
                                    this->request_.data (),
                                    this->request_.data_length (),
                                    this->request_.content_length ());
          else
            this->error_response (HTTP_Status_Code::STATUS_UNAUTHORIZED,
                                  "Invalid authorization attempt");
          delete buf;
        }
      break;

    default :
      this->error_response (HTTP_Status_Code::STATUS_NOT_IMPLEMENTED,
                            "Requested method is not implemented.");
    }
}


void
HTTP_Response::cgi_response (void)
{
  ACE_Process_Options cgi_options;

  if (this->request_.cgi_args ())
    cgi_options.command_line ("%s %s",
			      this->request_.path (),
			      this->request_.cgi_args ());
  else
    cgi_options.command_line ("%s", this->request_.path ());

  // Build environment variables
  cgi_options.setenv ("SERVER_SOFTWARE", "%s", "JAWS/1.0");
  cgi_options.setenv ("SERVER_NAME", "%s", "localhost");
  cgi_options.setenv ("GATEWAY_INTERFACE", "%s", "CGI/1.1");

  cgi_options.setenv ("SERVER_PROTOCOL", "%s",
                      this->request_.version ()
                      ? this->request_.version ()
                      : "HTTP/0.9");
  cgi_options.setenv ("SERVER_PORT", "%d", 5432);

  cgi_options.setenv ("REQUEST_METHOD", "%s", this->request_.method ());

  if (this->request_.path_info ())
    {
      cgi_options.setenv ("PATH_INFO", "%s",
			  this->request_.path_info ());
      cgi_options.setenv ("PATH_TRANSLATED",
			  "%s/%s",
                          HTTP_Config::instance ()->document_root (),
                          this->request_.path_info ());
    }

  cgi_options.setenv ("SCRIPT_NAME",
		      "%s",
		      this->request_.uri ());

  if (this->request_.query_string ())
    cgi_options.setenv ("QUERY_STRING",
			"%s",
			this->request_.query_string ());

  if (this->request_.cgi_env ())
    for (size_t i = 0; this->request_.cgi_env ()[i]; i += 2)
      cgi_options.setenv (this->request_.cgi_env ()[i],
			  "%s",
			  this->request_.cgi_env ()[i+1]);

  char buf[BUFSIZ];
  char *p, *q;
  ACE_OS::strcpy (buf, "HTTP_");
  p = q = buf + ACE_OS::strlen (buf);

  for (size_t i = 0; i < HTTP_Request::NUM_HEADER_STRINGS; i++)
    {
      int j = 0;

      for (char c; (c = this->request_.header_strings (i)[j++]) != '\0'; )
	if (isalpha (c))
	  *q++ = toupper (c);
	else if (c == '-')
	  *q++ = '_';
	else
	  *q++ = c;

      *q = '\0';

      const char *hv = this->request_.header_values (i);

      if (hv && *hv)
        cgi_options.setenv (buf, "%s", hv);
      q = p;
    }

  cgi_options.set_handles (this->io_.handle (),
                           this->io_.handle (),
                           this->io_.handle ());

  this->build_headers ();
  this->io_.send_confirmation_message (this->HTTP_HEADER,
                                       this->HTTP_HEADER_LENGTH);
  //  ACE::send (this->io_.handle (),
  //  this->HTTP_HEADER, this->HTTP_HEADER_LENGTH);

  // Exec the CGI program.
  ACE_Process cgi_process;
  cgi_process.spawn (cgi_options);
  //  cgi_process.wait ();
}

void
HTTP_Response::build_headers (void)
{
  // At this point, we should really determine the type of request
  // this is, and build the appropriate header.

  // Let's assume this is HTML for now.  Unless the request is CGI,
  // then do not include content-* headers.

  if (this->request_.version () == 0
      || ACE_OS::strcmp ("HTTP/0.9", this->request_.version ()) == 0)
    {
      HTTP_HEADER = (char *) EMPTY_HEADER;
      HTTP_HEADER_LENGTH = 0;
    }
  else
    {
      HTTP_HEADER = new char[BUFSIZ * 4];
      // We assume that at this point everything is OK

      if (! this->request_.cgi ())
        HTTP_HEADER = "HTTP/1.0 200 OK\r\n"
          "Content-type: text/html\r\n\r\n";
      else
        HTTP_HEADER = "HTTP/1.0 200 OK\r\n";

      HTTP_HEADER_LENGTH = ACE_OS::strlen (HTTP_HEADER);

#if 0
      const char *date_ptr = HTTP_Helper::HTTP_date ();

      if (date_ptr != 0)
	HTTP_HEADER_LENGTH +=
	  ACE_OS::sprintf(HTTP_HEADER+HTTP_HEADER_LENGTH,
                            "Date: %s\r\n", date_ptr);

      if (! this->request_.cgi ())
	HTTP_HEADER_LENGTH +=
	  ACE_OS::sprintf(HTTP_HEADER+HTTP_HEADER_LENGTH, 
			  "Content-type: %s\r\n\r\n",
			  "text/html");
#endif
    }

  HTTP_TRAILER = "";
  HTTP_TRAILER_LENGTH = 0;
}
