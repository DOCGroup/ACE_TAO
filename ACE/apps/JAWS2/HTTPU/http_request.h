// There are two kinds of HTTP Requests in a proxy.
// One is the kind you have to read in from the HTTP client.
// The other is the kind you issue to the server.

#ifndef HTTPU_HTTP_REQUEST_HPP
#define HTTPU_HTTP_REQUEST_HPP

#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "JAWS/Parse_Headers.h"
#include "HTTPU/http_export.h"
#include "HTTPU/http_base.h"
#include "HTTPU/parse_http_request.h"
#include "HTTPU/parse_url.h"

class HTTPU_Export HTTP_Request : public HTTP_Base
{
public:
  HTTP_Request ();
  virtual ~HTTP_Request ();

  Parse_HTTP_Request *request_line ();
  // Returns the parsed request line.

  const Parse_HTTP_Request *request_line () const;
  // Returns the parsed request line.

  HTTP_Parse_URL *url ();
  // Returns the parsed url.

  void dump ();

protected:
  virtual void parse_line ();
  virtual int espouse_line ();
  virtual void set_status (int);

private:
  Parse_HTTP_Request request_;
  HTTP_Parse_URL url_;
};

#if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "HTTPU/http_request.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */


#endif /* !defined (HTTPU_HTTP_REQUEST_HPP) */
