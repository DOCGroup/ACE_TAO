/* -*- c++ -*- */
// Hey, Emacs!  This is a C++ file!
// $Id$

#if !defined (HTTP_REQUEST_H)
#define HTTP_REQUEST_H

class ACE_Message_Block;

#include "ace/OS.h"
#include "JAWS/server/Parse_Headers.h"

class HTTP_Request 
{
public:
  HTTP_Request (void);
  // Default construction.

  ~HTTP_Request (void);
  // Destructor.

  int parse_request (ACE_Message_Block &mb);
  // parse an incoming request

  void parse_request_line (char * const request_line);
  // the first line of a request is the request line, which
  // is of the form: METHOD URI VERSION

  int init (char * const buffer, int buflen);
  // Initialize the request object.  This will parse
  // the buffer and prepare for the accessors.

public:
  // The Accessors

  const char * method (void) const;
  const char * uri (void) const;
  const char * version (void) const;
  const char * path (void) const;

  int cgi (void) const;
  const char * cgi_args (void) const;
  const char ** cgi_env (void) const;

  const char * query_string (void) const;
  const char * path_info (void) const;

  int type (void) const;

  const Headers & headers (void) const;
  const char * header_strings (int index) const;
  const char * header_values (int index) const;

  // These are accessors I didn't implement, or don't
  // remember implementing, anyway.
  char *data (void);
  int data_length (void);
  int content_length (void);

  int status (void);
  const char *status_string (void);

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

private:
  // Private Accessors which can set values
  const char * method (const char *method_string);
  const char * uri (char *uri_string);
  const char * version (const char *version_string);
  const char * path (const char *uri_string);

  int cgi (char *uri_string);

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

  const char * const * const header_strings_;
  static const char * const static_header_strings_[NUM_HEADER_STRINGS];

  const char * const * const method_strings_;
  static const char * const static_method_strings_[NUM_METHOD_STRINGS];

  char *data_;
  int datalen_;
  int content_length_;
  char *filename_;
  int status_;
  int type_;
};

#endif /* HTTP_REQUEST_H */
