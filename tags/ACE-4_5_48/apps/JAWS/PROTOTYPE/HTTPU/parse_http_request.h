// $Id$

#if !defined (HTTPU_PARSE_HTTP_REQUEST_H)
#define HTTPU_PARSE_HTTP_REQUEST_H

#include "ace/OS.h"
#include "HTTPU/http_headers.h"

class Parse_HTTP_Request : public HTTP_HCodes
{
public:
  Parse_HTTP_Request (const char *request = 0);
  ~Parse_HTTP_Request (void);

  void init (const char *request);

  int method (void) const { return (int) *this->method_; }
  const char *method_str (void) const { return (const char *) *this->method_; }

  int major_version (void) const { return this->major_version_; }
  int minor_version (void) const { return this->minor_version_; }

  const char *version (void) const
    { return this->version_ ? this->version_ : "HTTP/0.9"; }

  const char *url (void) const { return this->url_; }

  enum { HTTPU_OK, NO_MEMORY, BAD_REQUEST, NOT_IMPLEMENTED };

  int error (void) { return this->error_; }
  // 0 -> ok

  void dump (void);

private:

  HTTP_Hdr_Node *method_;
  int major_version_;
  int minor_version_;
  char *version_;
  char *url_;
  char *request_;
  int error_;

};

#endif /* !defined (HTTPU_PARSE_HTTP_REQUEST_H) */
