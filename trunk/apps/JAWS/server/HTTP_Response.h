/* -*- c++ -*- */
// Hey, Emacs!  This is a C++ file!
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

#if !defined (HTTP_RESPONSE_H)
#define HTTP_RESPONSE_H

class JAWS_IO;
class HTTP_Request;

class HTTP_Response
  // = TITLE
  //     Abstraction for HTTP responses.
  //
  // = DESCRIPTION
  //     Provides an encapsulation of responses to HTTP requests.
  //     For instance, given an HTTP GET request, it will produce
  //     header and body suitable for returning to the client who made
  //     the request.
{
public:

  HTTP_Response (JAWS_IO &io, HTTP_Request &request);
  HTTP_Response (HTTP_Request &request, JAWS_IO &io);
  ~HTTP_Response (void);

  void process_request (void);
  void error_response (int status, const char *log_message);

private:

  static void process_request (HTTP_Response &response);

  void build_headers (void);

private:

  JAWS_IO &io_;
  HTTP_Request &request_;

  char *HTTP_HEADER;
  char *HTTP_TRAILER;
  int HTTP_HEADER_LENGTH;
  int HTTP_TRAILER_LENGTH;
};

#endif /* HTTP_RESPONSE_H */
