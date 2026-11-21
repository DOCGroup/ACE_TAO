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

/// Represents an HTTP request read from a client or sent to a server.
class HTTPU_Export HTTP_Request : public HTTP_Base
{
public:
  HTTP_Request ();
  ~HTTP_Request () override;
  HTTP_Request (HTTP_Request const &) = delete;
  HTTP_Request (HTTP_Request &&) = delete;
  HTTP_Request &operator= (HTTP_Request const &) = delete;
  HTTP_Request &operator= (HTTP_Request &&) = delete;

  /// Return the parsed request line.
  Parse_HTTP_Request *request_line ();

  /// Return the parsed request line.
  const Parse_HTTP_Request *request_line () const;

  /// Return the parsed URL.
  HTTP_Parse_URL *url ();

  void dump ();

protected:
  void parse_line () override;
  int espouse_line () override;
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
