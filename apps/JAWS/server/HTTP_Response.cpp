// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    HTTP_Response
//
// = AUTHOR
//    James Hu
// 
// ============================================================================


#include "JAWS/server/HTTP_Response.h"
#include "JAWS/server/HTTP_Request.h"
#include "JAWS/server/HTTP_Helpers.h"
#include "JAWS/server/IO.h"

HTTP_Response::HTTP_Response (JAWS_IO &io, HTTP_Request &request)
  : io_(io), request_(request)
{
}

HTTP_Response::HTTP_Response (HTTP_Request &request, JAWS_IO &io)
  : io_(io), request_(request)
{
  delete this->HTTP_HEADER;
}

HTTP_Response::~HTTP_Response (void)
{
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

  static char const error_header[] =
    "%s %d %s\r\n"
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

  length =
    ACE_OS::sprintf (buf2, error_message,
                     status_code, HTTP_Status_Code::instance ()[status_code],
                     log_message);

  if (this->request_.version () == 0
      || ACE_OS::strcmp ("HTTP/0.9", this->request_.version ()) == 0)
    {
      buf = buf2;
    }
  else
    {
      length +=
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
  ACE_DEBUG ((LM_DEBUG, " (%t) %s request for %s, version %s\n",
              request_.method (), request_.uri (),
              (request_.version () ? request_.version () : "HTTP/0.9")));

  switch (this->request_.type ())
    {
    case HTTP_Request::GET :

      this->build_headers ();
      this->io_.transmit_file (this->request_.uri (), 
                               this->HTTP_HEADER
                               ? this->HTTP_HEADER
                               : "",
                               this->HTTP_HEADER_LENGTH,
                               this->HTTP_TRAILER, 
                               this->HTTP_TRAILER_LENGTH);

      break;

    case HTTP_Request::HEAD :
      this->build_headers ();
      this->io_.send_confirmation_message (this->HTTP_HEADER
                                           ? this->HTTP_HEADER
                                           : "",
                                           this->HTTP_HEADER_LENGTH);
      break;

    case HTTP_Request::POST :
      // What to do here?
      // Standard says this is implementation dependent.
      // Examples: annotations, page updates, etc.
      // They may be a good place to stick CORBA stuff,
      // and mobile code.
      this->error_response (HTTP_Status_Code::STATUS_NOT_IMPLEMENTED,
                            "Requested method is not implemented.");
      break;

    case HTTP_Request::PUT :
      this->io_.receive_file (this->request_.uri (),
                              this->request_.data (),
                              this->request_.data_length (),
                              this->request_.content_length ());
      break;

    default :
      this->error_response (HTTP_Status_Code::STATUS_NOT_IMPLEMENTED,
                            "Requested method is not implemented.");
    }
}


void
HTTP_Response::cgi_response (void)
{
  // Build command line if any
  // Build environment variables
  // Exec the cgi program
}

void
HTTP_Response::build_headers (void)
{
  // At this point, we should really determine the type of request
  // this is, and build the appropriate header.  For instance, this
  // is unnecessary for CGI files since the header and contents are
  // created and sent by the CGI program.

  // Let's assume this is HTML for now.

  if (this->request_.version () == 0
      || ACE_OS::strcmp ("HTTP/0.9", this->request_.version ()) == 0)
    {
      HTTP_HEADER = 0;
      HTTP_HEADER_LENGTH = 0;
    }
  else
    {
      HTTP_HEADER = new char[BUFSIZ * 4];

      HTTP_HEADER[0] = '\0';
      HTTP_HEADER_LENGTH = 0;

      HTTP_HEADER_LENGTH +=
        ACE_OS::sprintf(HTTP_HEADER, "%s %d %s\r\n",
                        this->request_.version (),
                        this->request_.status (),
                        this->request_.status_string ());

      char date[40];
      const char *date_ptr = HTTP_Helper::HTTP_date (date, sizeof(date)-1);

      if (date_ptr != 0)
        {
          HTTP_HEADER_LENGTH +=
            ACE_OS::sprintf(HTTP_HEADER+HTTP_HEADER_LENGTH,
                            "Date: %s\r\n", date_ptr);
        }

      HTTP_HEADER_LENGTH +=
        ACE_OS::sprintf(HTTP_HEADER+HTTP_HEADER_LENGTH, 
                        "Content-type: %s\r\n\r\n",
                        "text/html");
    }

  HTTP_TRAILER = "";
  HTTP_TRAILER_LENGTH = 0;
}
