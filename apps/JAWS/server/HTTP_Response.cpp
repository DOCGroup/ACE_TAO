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
  switch (this->request_.status ()) {
  case HTTP_Request::OK :

    switch (this->request_.type ()) {
    case HTTP_Request::GET :
      this->build_headers ();
      this->io_.transmit_file (this->request_.filename (), 
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
      // this->io_.spawn_cgi (...);
      break;

    case HTTP_Request::PUT :
      this->io_.receive_file (this->request_.filename (),
                              this->request_.data (),
                              this->request_.data_length (),
                              this->request_.content_length ());
      break;

    default :
      this->error_response (HTTP_Status_Code::STATUS_NOT_IMPLEMENTED,
                            "Requested method is not implemented.");
    }
    break;

  default:
    this->error_response (HTTP_Status_Code::STATUS_BAD_REQUEST, "");
  }
}

void
HTTP_Response::error_response (int status_code, const char *log_message)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) [%s %s %s] %s\n",
              this->request_.type(),
              this->request_.filename(),
              this->request_.version(),
              log_message));

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
    "<h1>Error %d: %s</h1>"
    "The request could not be completed because: %s\n"
    "</body>\n"
    "</html>\n"
    ;

  char buffer[4 * BUFSIZ];

  int length;

  if (ACE_OS::strcmp ("HTTP/0.9", this->request_.version ()) == 0) {
    length =
      sprintf(buffer, error_message,
              status_code, HTTP_Status_Code::instance()[status_code],
              log_message);
  }
  else {
    char message[BUFSIZ];
    length =
      sprintf(buffer, error_header,
              this->request_.version(), status_code,
              HTTP_Status_Code::instance()[status_code],
              sprintf(message, error_message,
                      status_code, HTTP_Status_Code::instance()[status_code],
                      log_message),
              message);
  }
          
  this->io_.send_error_message (buffer, length);
}

void
HTTP_Response::build_headers (void)
{
  // At this point, we should really determine the type of request
  // this is, and build the appropriate header.  For instance, this
  // is unnecessary for CGI files since the header and contents are
  // created and sent by the CGI program.

  // Let's assume this is HTML for now.

  if (ACE_OS::strcmp ("HTTP/0.9", this->request_.version ()) == 0) {
    HTTP_HEADER = "";
    HTTP_HEADER_LENGTH = 0;
  }
  else {
    HTTP_HEADER = new char[BUFSIZ * 4];

    HTTP_HEADER[0] = '\0';
    HTTP_HEADER_LENGTH = 0;

    HTTP_HEADER_LENGTH +=
      sprintf(HTTP_HEADER, "%s %d %s\r\n",
              this->request_.version (),
              this->request_.status (),
              this->request_.status_string ());

    char date[30];
    const char *date_ptr = HTTP_Helper::HTTP_date (date, sizeof(date)-1);

    if (date_ptr != 0) {
      HTTP_HEADER_LENGTH +=
        sprintf(HTTP_HEADER, "Date: %s\r\n",
                date);
    }

    HTTP_HEADER_LENGTH +=
      sprintf(HTTP_HEADER, "Content-type: %s\r\n\r\n",
              "text/html");
  }

  HTTP_TRAILER = "";
  HTTP_TRAILER_LENGTH = 0;
}
