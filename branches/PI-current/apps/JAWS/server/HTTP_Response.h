/* -*- c++ -*- */
// Hey, Emacs!  This is a C++ file!
// $Id$

// ============================================================================
//
// = LIBRARY
//    jaws
//
// = FILENAME
//    HTTP_Response.h
//
// = AUTHOR
//    James Hu
//
// ============================================================================

#ifndef HTTP_RESPONSE_H
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
  HTTP_Response (JAWS_IO &io,
                 HTTP_Request &request);
  HTTP_Response (HTTP_Request &request, JAWS_IO &io);
  ~HTTP_Response (void);

  void process_request (void);
  // This is called by the handler to initiate a response.

  void error_response (int status,
                       const char *log_message);
  // This returns an error response for cases where there is a problem
  // with the request, logging the log_message.

private:

  void normal_response (void);
  // Called by process_request when the request is a normal request.

  void cgi_response (void);
  // Called by process_request when the request is a cgi request.

private:

  static void process_request (HTTP_Response &response);
  // static version of process_request, just in case.

  void build_headers (void);
  // creates the appropriate header information for responses.

private:
  JAWS_IO &io_;
  HTTP_Request &request_;
  // The IO and Request objects associated with this re

  char *HTTP_HEADER;
  char *HTTP_TRAILER;
  int HTTP_HEADER_LENGTH;
  int HTTP_TRAILER_LENGTH;
  // HTTP Headers and trailers.
};

#endif /* HTTP_RESPONSE_H */
