// $Id$

#if !defined (HTTPU_PARSE_HTTP_RESPONSE_H)
#define HTTPU_PARSE_HTTP_RESPONSE_H

#include "ace/OS.h"

class Parse_HTTP_Response
{
public:
  Parse_HTTP_Response (const char *response = 0);
  ~Parse_HTTP_Response (void);

  void init (const char *response);

  int code (void) const { return this->code_; }
  const char *code_str (void) const
    { return this->code_str_ ? this->code_str_ : "200"; }

  int major_version (void) const { return this->major_version_; }
  int minor_version (void) const { return this->minor_version_; }

  const char *version (void) const
    { return this->version_ ? this->version_ : "HTTP/0.9"; }

  enum { HTTPU_OK, NO_MEMORY, BAD_RESPONSE };

  int error (void) { return this->error_; }
  // 0 -> ok

private:

  int code_;
  char *code_str_;
  int major_version_;
  int minor_version_;
  char *version_;
  char *response_;
  int error_;

};

#endif /* !defined (HTTPU_PARSE_HTTP_RESPONSE_H) */
