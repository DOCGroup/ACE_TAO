/* -*- c++ -*- */
// Hey, Emacs!  This is a C++ file!
// $Id$

// ============================================================================
//
// = LIBRARY
//   jaws
//
// = FILENAME
//    HTTP_Request.h
//
// = AUTHOR
//    James Hu
//
// ============================================================================

#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

class ACE_Message_Block;

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Parse_Headers.h"

class HTTP_Request
  // = TITLE
  //     This parses the client request of an HTTP transaction.
  //
  // = DESCRIPTION
{
public:
  HTTP_Request (void);
  // Default construction.

  ~HTTP_Request (void);
  // Destructor.

  int parse_request (ACE_Message_Block &mb);
  // parse an incoming request

  void parse_request_line (char *const request_line);
  // the first line of a request is the request line, which is of the
  // form: METHOD URI VERSION.

  int init (char *const buffer,
            int buflen);
  // Initialize the request object.  This will parse the buffer and
  // prepare for the accessors.

public:
  // = The Accessors.

  const char *method (void) const;
  // HTTP request method

  const char *uri (void) const;
  // HTTP request uri

  const char *version (void) const;
  // HTTP request version

  const char *path (void) const;
  // The HTTP request uri translated into a server filename path

  int cgi (void) const;
  // TRUE of the request is a cgi request

  const char *cgi_args (void) const;
  // The arguments to the cgi request

  const char **cgi_env (void) const;
  // The environment variables passed to the CGI request

  const char *query_string (void) const;
  // The cgi request query string

  const char *path_info (void) const;
  // The cgi request path information

  int type (void) const;
  // The type of the HTTP request

  const Headers &headers (void) const;
  // The headers that were parsed from the request

  const char *header_strings (int index) const;
  // Header strings stored

  const char *header_values (int index) const;
  // Values associated with the header strings

  char *data (void);
  // The buffer into which request data is read

  int data_length (void);
  // The length of the request data

  int content_length (void);
  // The length of incoming content if any

  int status (void);
  // Current status of the incoming request

  const char *status_string (void);
  // A string describing the state of the incoming request

  void dump (void);
  // Dump the state of the request.

  enum
  {
    NO_TYPE = -1,
    GET = 0,
    HEAD,
    POST,
    PUT,
    NUM_METHOD_STRINGS
  };
  // Values for request type

  enum
  {
    DATE = 0,
    PRAGMA,
    AUTHORIZATION,
    FROM,
    IF_MODIFIED_SINCE,
    REFERRER,
    USER_AGENT,
    ALLOW,
    CONTENT_ENCODING,
    CONTENT_LENGTH,
    CONTENT_TYPE,
    EXPIRES,
    LAST_MODIFIED,
    NUM_HEADER_STRINGS
  };
  // Header strings

private:
  // = Private Accessors which can set values
  const char *method (const char *method_string);
  const char *uri (char *uri_string);
  const char *version (const char *version_string);
  const char *path (const char *uri_string);

  int cgi (char *uri_string);
  // determine if the given URI is a CGI program.

  int cgi_in_path (char *uri_string, char *&extra_path_info);
  // determine if the given URI resides in a cgi-bin directory

  int cgi_in_extension (char *uri_string, char *&extra_path_info);
  // determine if the given URI contains a cgi extension

  void cgi_args_and_env (char *&extra_path_info);
  // set the arguments and environment for the cgi program

  int type (const char *type_string);

private:
  int got_request_line (void) const;

private:
  int got_request_line_;
  Headers headers_;

  char *method_;
  char *uri_;
  char *version_;
  char *path_;

  int cgi_;
  char **cgi_env_;
  char *cgi_args_;

  char *query_string_;
  char *path_info_;

  const char * const *const header_strings_;
  static const char *const static_header_strings_[NUM_HEADER_STRINGS];

  const char * const *const method_strings_;
  static const char *const static_method_strings_[NUM_METHOD_STRINGS];

  char *data_;
  int datalen_;
  int content_length_;
  char *filename_;
  int status_;
  int type_;
};

#endif /* HTTP_REQUEST_H */
