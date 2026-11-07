#include "HTTPU/parse_http_response.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdlib.h"
#include <cstring>

Parse_HTTP_Response::Parse_HTTP_Response (const char *response)
  : code_ (200),
    code_str_ (nullptr),
    major_version_ (0),
    minor_version_ (9),
    version_ (nullptr),
    response_ (nullptr),
    error_ (0)
{
  if (response != nullptr)
    this->init (response);
}

Parse_HTTP_Response::~Parse_HTTP_Response ()
{
  if (this->response_ != nullptr)
    ACE_OS::free (this->response_);
}

void
Parse_HTTP_Response::init (const char *response)
{
  this->response_ = ACE_OS::strdup (response);
  if (this->response_ == nullptr)
    {
      this->error_ = NO_MEMORY;
      return;
    }

  int n = ::sscanf (this->response_, "HTTP/%d.%d %d %*s",
                    &(this->major_version_),
                    &(this->minor_version_),
                    &(this->code_));

  if (n == 3)
    {
      char *p = this->response_;

      while (*p == ' ' || *p == '\t')
        p++;

      this->version_ = p++;

      while (*p != ' ' && *p != '\t')
        p++;

      *p++ = '\0';

      while (*p == ' ' || *p == '\t')
        p++;

      this->code_str_ = p;

      while (*p && !std::strchr (" \t\r\n", *p))
        p++;

      *p++ = '\0';
    }
  else
    this->error_ = BAD_RESPONSE;
}

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "HTTPU/parse_http_response.inl"
#endif /* ACE_HAS_INLINED_OSCALLS */
