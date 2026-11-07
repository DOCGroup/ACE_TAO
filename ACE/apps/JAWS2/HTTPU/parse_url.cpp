#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Log_Msg.h"
#include "HTTPU/parse_url.h"
#include <cstdlib>
#include <cstring>

HTTP_Parse_URL::HTTP_Parse_URL (const char *url)
  : url_ (nullptr),
    scheme_ (nullptr),
    user_ (nullptr),
    passwd_ (nullptr),
    host_ (nullptr),
    port_ (-1),
    url_path_ (nullptr),
    error_ (URL_ERROR_NONE),
    is_cgi_ (false)
{
  this->init (url);
}

HTTP_Parse_URL::~HTTP_Parse_URL ()
{
  if (this->url_ != nullptr)
    ACE_OS::free (this->url_);
}


void
HTTP_Parse_URL::init( const char *url )
{
   // Should really reset completely and cleanly here before
   // doing anything else!
   if (url == nullptr)
      return;

   if (url_ != nullptr)
      ACE_OS::free( url_ );

   url_ = ACE_OS::strdup( url );
   if (url_ == nullptr)
   {
      error_ = URL_ERROR_STRDUP;
      return;
   }

  if (std::strlen (this->url_) > 3 && std::strstr (this->url_, "://"))
    {
      // Parse it out completely.  Figure out what it is later.
      parse_url();
    }
  else
    {
      this->url_path_ = this->url_;
      this->is_cgi (this->url_path_);
    }
}


void
HTTP_Parse_URL::parse_url ()
{
  char *p = this->url_;

  char *q;
  if ((q = std::strchr (this->url_, '\r'))
      || (q = std::strchr (this->url_, '\n')))
    *q = '\0';

  this->parse_scheme (p);
  if (*p == '\0')
    {
      this->error_ = URL_ERROR_SCHEME;
      return;
    }

  // Parse past "//"
  if (*p != '/' || *(p+1) != '/')
    {
      this->error_ = URL_ERROR_SLASHSLASH;
      return;
    }
  p += 2;

  this->parse_host (p);

  while (*p == '/')
    p++;

  if (*p == '\0')
    return;

  this->url_path_ = p;
  this->is_cgi (this->url_path_);
}

void
HTTP_Parse_URL::parse_scheme (char *&p)
{
  // Parse the scheme.  The scheme is delimited by a ':'.
  if (*p != '\0')
    {
      this->scheme_ = p++;
      for (;;)
        {
          switch (*p)
            {
            case '\0':
              break;
            case ':':
              *p++ = '\0';
              break;
            default:
              p++;
              continue;
            }
          break;
        }
    }
}

void
HTTP_Parse_URL::parse_host (char *&p)
{
  // Parse user, password, host, port
  if (*p == '/' || *p == '\0')
    {
      this->set_port_from_scheme ();
      return;
    }

  char *at = nullptr;
  char *colon1 = nullptr;
  char *colon2 = nullptr;
  char *q = p;
  while (*q != '\0')
    {
      if (*q == '/')
        {
          *q = '\0';
          q++;
          break;
        }
      if (*q == ':')
        {
          if (colon1 == nullptr)
            {
              if (at != nullptr && colon2 == nullptr)
                colon2 = q;
              else
                colon1 = q;
            }
          else
            {
              if (at != nullptr && colon2 == nullptr)
                colon2 = q;
            }
        }
      if (*q == '@')
        {
          if (at == nullptr)
            at = q;
        }
      q++;
    }

  // no user, no port
  if (at == nullptr && colon1 == nullptr)
    {
      if (*p != '\0' && *p != '/')
        this->host_ = p;
    }

  // no user, port
  else if (at == nullptr && colon1 != nullptr)
    {
      if (p != colon1)
        this->host_ = p;
      *colon1++ = '\0';
      this->port_ = std::atoi (colon1);
    }

  // user, no passwd, no port
  else if (at != nullptr && colon1 == nullptr && colon2 == nullptr)
    {
      this->user_ = p;
      *at++ = '\0';
      if (*at != '\0' && *at != '/')
        this->host_ = at;
    }

  // user, no passwd, port
  else if (at != nullptr && colon1 == nullptr && colon2 != nullptr)
    {
      this->user_ = p;
      *at++ = '\0';
      if (at != colon2)
        this->host_ = at;
      *colon2++ = '\0';
      this->port_ = std::atoi (colon2);
    }

  // user, passwd, no port
  else if (at != nullptr && colon1 != nullptr && colon2 == nullptr)
    {
      this->user_ = p;
      *colon1++ = '\0';
      this->passwd_ = colon1;
      *at++ = '\0';
      if (*at != '\0')
        this->host_ = at;
    }

  // user, passwd, and port
  else if (at != nullptr && colon1 != nullptr && colon2 != nullptr)
    {
      this->user_ = p;
      *colon1++ = '\0';
      this->passwd_ = colon1;
      *at++ = '\0';
      if (at != colon2)
        this->host_ = at;
      *colon2++ = '\0';
      this->port_ = std::atoi (colon2);
    }

  // impossible!
  else
    {
      ACE_ERROR ((LM_ERROR, "uh oh!\n"));
      p = q;
      return;
    }

  this->set_port_from_scheme ();
  p = q;
}

void
HTTP_Parse_URL::set_port_from_scheme ()
{
  if (std::strcmp (this->scheme_, "ftp") == 0)
    {
      if (this->port_ == -1)
        this->port_ = 21;
      if (this->user_ == nullptr)
        {
          this->user_ = "anonymous";

          // *** need something better here
          this->passwd_ = "a@b.c";
        }
    }
  else if (std::strcmp (this->scheme_, "http") == 0)
    {
      if (this->port_ == -1)
        this->port_ = 80;
    }
}

const char *
HTTP_Parse_URL::scheme () const
{
  return this->scheme_;
}

const char *
HTTP_Parse_URL::user () const
{
  return this->user_;
}

const char *
HTTP_Parse_URL::passwd () const
{
  return this->passwd_;
}

const char *
HTTP_Parse_URL::host () const
{
  return this->host_;
}

int
HTTP_Parse_URL::port () const
{
  return this->port_;
}

const char *
HTTP_Parse_URL::url_path () const
{
  return this->url_path_ != nullptr ? this->url_path_ : "";
}

void
HTTP_Parse_URL::is_cgi (const char *path)
{
  bool yes;

  yes = (std::strchr (path, '?') != nullptr);
  if (!yes && (std::strlen (path) >= 3))
    yes = (std::strstr (path, "cgi") != nullptr);
  if (!yes)
    yes = (std::strstr (path, "asp") != nullptr);

  this->is_cgi_ = yes;
}

bool
HTTP_Parse_URL::is_cgi () const
{
  return this->is_cgi_;
}
