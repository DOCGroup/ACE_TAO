// $Id$

// There are two kinds of HTTP Responses in a proxy.
// One is the kind you have to read in from the HTTP server.
// The other is the kind you issue to the HTTP client.

#if !defined (HTTPU_HTTP_RESPONSE_HPP)
#define HTTPU_HTTP_RESPONSE_HPP

#include "ace/Message_Block.h"
#include "JAWS/Parse_Headers.h"

#include "HTTPU/http_base.h"
#include "HTTPU/parse_http_response.h"

class HTTP_Response : public HTTP_Base
{
public:
  HTTP_Response (void) {}
  ~HTTP_Response (void) {}

  Parse_HTTP_Response *response_line (void)
    { return &(this->response_); }
  // Returns the parsed response line.

  void dump (void);

protected:

  virtual void parse_line (void);
  virtual int espouse_line (void);

private:
  Parse_HTTP_Response response_;
};

#endif /* !defined (HTTPU_HTTP_RESPONSE_HPP) */
