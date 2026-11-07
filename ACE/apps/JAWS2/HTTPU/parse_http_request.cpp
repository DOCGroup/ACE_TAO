#include "HTTPU/parse_http_request.h"
#include <cstring>

Parse_HTTP_Request::Parse_HTTP_Request (const char *request)
  : method_ (nullptr),
    major_version_ (-1),
    minor_version_ (-1),
    version_ (nullptr),
    url_ (nullptr),
    request_ (nullptr),
    error_ (0)
{
  if (request != nullptr)
    this->init (request);
}

Parse_HTTP_Request::~Parse_HTTP_Request ()
{
  if (this->request_ != nullptr)
    ACE_OS::free (this->request_);
}

void
Parse_HTTP_Request::dump ()
{
  ACE_DEBUG ((LM_DEBUG, "%s %s %s\n",
              this->method_str (), this->url (), this->version ()));
}

void
Parse_HTTP_Request::init (const char *request)
{
  char *method;

  this->request_ = ACE_OS::strdup (request);
  if (this->request_ == nullptr)
    {
      this->error_ = NO_MEMORY;
      return;
    }

  char buf[BUFSIZ];
  int n = ::sscanf (this->request_, "%s %*s HTTP/%d.%d",
                    buf,
                    &(this->major_version_),
                    &(this->minor_version_));

  if (n == 1 || n == 3)
    {
      char *p = this->request_;

      while (*p == ' ' || *p == '\t')
        p++;

      method = p++;

      while (*p != ' ' && *p != '\t')
        p++;

      *p++ = '\0';

      while (*p == ' ' || *p == '\t')
        p++;

      this->url_ = p;

      while (*p && !std::strchr (" \t\r\n", *p))
        p++;

      *p++ = '\0';

      if (n == 1)
        {
          this->major_version_ = 0;
          this->minor_version_ = 9;
        }
      else
        {
          while (*p == ' ' || *p == '\t')
            p++;

          this->version_ = p;

          while (*p && !std::strchr (" \t\r\n", *p))
            p++;

          *p++ = '\0';
        }

      if (std::strcmp (method, "GET") == 0)
        this->method_ = &GET;
      else if (std::strcmp (method, "HEAD") == 0)
        this->method_ = &HEAD;
      else if (std::strcmp (method, "POST") == 0)
        this->method_ = &POST;
      else if (std::strcmp (method, "PUT") == 0)
        this->method_ = &PUT;
      else if (std::strcmp (method, "QUIT") == 0)
        this->method_ = &QUIT;
      else
        {
          this->method_ = &DUNNO;
          this->error_ = NOT_IMPLEMENTED;
        }
    }
  else
    this->error_ = BAD_REQUEST;
}

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "HTTPU/parse_http_request.inl"
#endif /* ACE_HAS_INLINED_OSCALLS */
