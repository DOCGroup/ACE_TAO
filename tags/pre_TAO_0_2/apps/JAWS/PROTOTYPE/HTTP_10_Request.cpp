// $Id$

#include "JAWS/JAWS.h"

#include "HTTP_10.h"

#define ACCESSOR(T,C,x) \
T C##::##x (void) const { return this->##x##_; }\
void C##::##x (T t) { this->##x##_ = t; }


JAWS_HTTP_10_Request::JAWS_HTTP_10_Request (void)
  : got_request_line_ (0),
    end_of_headers_ (0),
    end_of_line_ (0),
    last_header_type_ (0),
    last_header_value_ (0),
    last_header_length_ (0),
    status_ (0),
    type_ (-1),
    content_length_ (-1),
    datalen_ (0),
    status_msglen_ (0),
    data_ (0),
    method_ (0),
    uri_ (0),
    version_ (0),
    path_ (0)
{
}

JAWS_HTTP_10_Request::~JAWS_HTTP_10_Request (void)
{
  ACE_OS::free (this->method_);
  ACE_OS::free (this->uri_);
  ACE_OS::free (this->version_);
  ACE_OS::free (this->path_);
}

int
JAWS_HTTP_10_Request::reset (char *buffer, int buflen)
{
  // Initialize these every time.
  this->content_length_ = -1;

  // Extract the data pointer.
  this->data_ = buffer;
  this->datalen_ = 0;

  // Set the datalen
  if (this->data_ != NULL)
    this->datalen_ = buflen;
  else
    this->datalen_ = 0;

  // ACE_DEBUG ((LM_DEBUG, " (%t) init has initialized\n"));

  return 1;
}

void
JAWS_HTTP_10_Request::parse_request_line (char *request_line)
{
  char *ptr = request_line;
  char *buf = request_line;
  int offset = 1;

  this->status_ = JAWS_HTTP_10_Request::STATUS_OK;

  ptr = ACE_OS::strchr (request_line, '\n');

  if (ptr > request_line && ptr[-1] == '\r')
    ptr--, offset++;

  if (ptr == request_line)
    {
      this->status_ = JAWS_HTTP_10_Request::STATUS_BAD_REQUEST;
      return;
    }

  *ptr = '\0';
  ptr += offset;

  char *lasts = 0; // for strtok_r

  // Get the request type.
  this->got_request_line_ = 1;

  if (this->method (ACE_OS::strtok_r (buf, " \t", &lasts))
      && this->uri (ACE_OS::strtok_r (NULL, " \t", &lasts)))
    {
      if (this->type () != NO_TYPE)
        {
          if (this->version (ACE_OS::strtok_r (NULL, " \t", &lasts)) == 0
              && this->type () != GET)
            this->status_ = STATUS_NOT_IMPLEMENTED;

          if (this->path (this->uri ()) == 0)
            this->status_ = STATUS_NOT_FOUND;
        }
    }
}

const char *
JAWS_HTTP_10_Request::method (const char *method_string)
{
  if (this->method_)
    ACE_OS::free (this->method_);

  if (method_string == 0)
    {
      this->status_ = STATUS_BAD_REQUEST;
      this->method_ = 0;
    }
  else
    this->method_ = ACE_OS::strdup (method_string);

  return this->method_;
}

const char *
JAWS_HTTP_10_Request::uri (const char *uri_string)
{
  if (this->uri_)
    ACE_OS::free (this->uri_);

  if (uri_string == 0)
    {
      this->status_ = STATUS_BAD_REQUEST;
      this->uri_ = 0;
    }
  else
    {
      this->uri_ =  ACE_OS::strdup (uri_string);
      // this->cgi (this->uri_);
      JAWS_HTTP_10_Helper::HTTP_decode_string (this->uri_);
    }

  return this->uri_;
}

const char *
JAWS_HTTP_10_Request::version (const char *version_string)
{
  if (this->version_)
    ACE_OS::free (this->version_);

  if (version_string)
    this->version_ = ACE_OS::strdup (version_string);
  else
    this->version_ = 0;

  return this->version_;
}

const char *
JAWS_HTTP_10_Request::method (void) const
{
  return this->method_;
}

const char *
JAWS_HTTP_10_Request::uri (void) const
{
  return this->uri_;
}

const char *
JAWS_HTTP_10_Request::version (void) const
{
  return this->version_;
}

const char *
JAWS_HTTP_10_Request::path (void) const
{
  return this->path_;
}

int
JAWS_HTTP_10_Request::type (void)
{
  if (this->type_ == NO_TYPE && this->method () != 0)
    {
      if (ACE_OS::strcmp (this->method (), "GET") == 0)
        this->type_ = GET;
      else if (ACE_OS::strcmp (this->method (), "HEAD") == 0)
        this->type_ = HEAD;
      else if (ACE_OS::strcmp (this->method (), "POST") == 0)
        this->type_ = POST;
      else if (ACE_OS::strcmp (this->method (), "PUT") == 0)
        this->type_ = PUT;
      else
        this->status_ = STATUS_NOT_IMPLEMENTED;
    }

  return this->type_;
}

const char *
JAWS_HTTP_10_Request::path (const char *uri_string)
{
  char const *file_name = uri_string;
  char buf[MAXPATHLEN + 1];
  buf[0] = '\0';

  if (file_name == 0) return 0;

  if (*file_name == '/')
    {
      file_name++;
      if (*file_name == '~')
        {
          char *ptr = buf;

          while (*++file_name && *file_name != '/')
            *ptr++ = *file_name;

          *ptr = '\0';

          if (ptr == buf)
            ACE_OS::strcpy (buf, ACE_OS::getenv ("HOME"));
          else
            {
#if !defined (ACE_WIN32) && !defined (VXWORKS)
              char pw_buf[BUFSIZ];
              struct passwd pw_struct;
              if (ACE_OS::getpwnam_r (buf, &pw_struct, pw_buf, sizeof (pw_buf))
                  == 0)
                return 0;
              ACE_OS::strcpy (buf, pw_struct.pw_dir);
#endif /* NOT ACE_WIN32 AND NOT VXWORKS */
            }

          ACE_OS::strcat (buf, "/");
#if 0
          ACE_OS::strcat (buf, HTTP_Config::instance ()->user_dir ());
#else
          ACE_OS::strcat (buf, ".www-docs");
#endif
          ACE_OS::strcat (buf, file_name);
        }
      else
        {
          // With a starting '/' but no '~'
#if 0
          ACE_OS::strcat (buf, HTTP_Config::instance ()->document_root ());
#else
          ACE_OS::strcat (buf, ".");
#endif
          ACE_OS::strcat (buf, file_name - 1);
        }
    }

  if (*buf != '\0')
    this->path_ = ACE_OS::strdup (buf);

  return this->path_;
}

JAWS_HTTP_10_Headers *
JAWS_HTTP_10_Request::table (void)
{
  return &(this->table_);
}

void
JAWS_HTTP_10_Request::append_last_header_value (char c)
{
  if (this->last_header_value_ == 0)
    return;

  if (this->last_header_length_ < MAX_HEADER_LENGTH-1)
    {
      this->last_header_value_[this->last_header_length_] = c;
      this->last_header_length_++;
      this->last_header_value_[this->last_header_length_] = '\0';
    }
}

void
JAWS_HTTP_10_Request::reduce_last_header_value (void)
{
  if (this->last_header_value_ == 0) return;

  if (this->last_header_length_ > 0)
    {
      this->last_header_length_--;
      this->last_header_value_[this->last_header_length_] = '\0';
    }
}

void
JAWS_HTTP_10_Request::resize_last_header_value (void)
{
  if (this->last_header_value_ == 0)
    return;
  else {
    JAWS_TRACE ("JAWS_HTTP_10_Request::resize_last_header_value");

    char *nhv = ACE_OS::strdup (this->last_header_value_);

    if (nhv == 0)
      {
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "JAWS_HTTP_10_Request::resize_last_header_value"));
        this->table_.remove (this->last_header_type_);
        ACE_OS::free (this->last_header_type_);
      }
    else
      {
        const char **s = this->table_.find (this->last_header_type_);
        *s = nhv;
      }

    this->last_header_type_ = 0;
    this->last_header_value_ = 0;
    this->last_header_length_ = 0;
  }
}

void
JAWS_HTTP_10_Request::create_next_header_value (char *ht, char *hv)
{
  JAWS_TRACE ("JAWS_HTTP_10_Request::create_next_header_value");

  this->resize_last_header_value ();
  this->last_header_type (ht);
  this->last_header_value (hv);
  this->last_header_length (0);
}

char *
JAWS_HTTP_10_Request::header_buf (void)
{
  return this->header_buf_;
}

const ACCESSOR(char *,JAWS_HTTP_10_Request,last_header_type)
const ACCESSOR(char *,JAWS_HTTP_10_Request,last_header_value)
ACCESSOR(int,JAWS_HTTP_10_Request,last_header_length)
ACCESSOR(int,JAWS_HTTP_10_Request,end_of_line)
ACCESSOR(int,JAWS_HTTP_10_Request,got_request_line)
ACCESSOR(int,JAWS_HTTP_10_Request,end_of_headers)
ACCESSOR(int,JAWS_HTTP_10_Request,status)
