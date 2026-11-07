#ifndef HTTPU_PARSE_HTTP_RESPONSE_H
#define HTTPU_PARSE_HTTP_RESPONSE_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "HTTPU/http_export.h"

class HTTPU_Export Parse_HTTP_Response
{
public:
  explicit Parse_HTTP_Response (char const *response = nullptr);
  ~Parse_HTTP_Response ();
  Parse_HTTP_Response (Parse_HTTP_Response const &) = delete;
  Parse_HTTP_Response (Parse_HTTP_Response &&) = delete;
  Parse_HTTP_Response &operator= (Parse_HTTP_Response const &) = delete;
  Parse_HTTP_Response &operator= (Parse_HTTP_Response &&) = delete;

  void init (const char *response);

  int code () const;
  const char *code_str () const;

  int major_version () const;
  int minor_version () const;

  const char *version () const;

  enum { HTTPU_OK, NO_MEMORY, BAD_RESPONSE };

  int error () const;
  // 0 -> ok

private:
  int code_;
  char *code_str_;
  int major_version_;
  int minor_version_;
  char *version_;
  char *response_; // Owns the strdup buffer; code_str_ and version_ point into it.
  int error_;
};

#if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "HTTPU/parse_http_response.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */


#endif /* !defined (HTTPU_PARSE_HTTP_RESPONSE_H) */
