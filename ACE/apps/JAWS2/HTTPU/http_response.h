// There are two kinds of HTTP Responses in a proxy.
// One is the kind you have to read in from the HTTP server.
// The other is the kind you issue to the HTTP client.

#ifndef HTTPU_HTTP_RESPONSE_HPP
#define HTTPU_HTTP_RESPONSE_HPP

#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "JAWS/Parse_Headers.h"

#include "HTTPU/http_export.h"
#include "HTTPU/http_base.h"
#include "HTTPU/parse_http_response.h"

class HTTPU_Export HTTP_Response : public HTTP_Base
{
public:
  HTTP_Response ();
  ~HTTP_Response ();

  Parse_HTTP_Response *response_line ();
  // Returns the parsed response line.

  void dump ();

protected:
  virtual void parse_line ();
  virtual int espouse_line ();

private:
  Parse_HTTP_Response response_;
};

#if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "HTTPU/http_response.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */


#endif /* !defined (HTTPU_HTTP_RESPONSE_HPP) */
