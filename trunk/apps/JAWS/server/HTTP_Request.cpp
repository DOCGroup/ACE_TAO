// $Id$

#include "ace/Message_Block.h"
#include "JAWS/server/HTTP_Request.h"

#include "JAWS/server/HTTP_Helpers.h"

const char * const
HTTP_Request::static_header_strings_[HTTP_Request::NUM_HEADER_STRINGS]
= { "Date", "Pragma",
    "Authorization", "From", "If-Modified-Since", "Referrer", "User-Agent",
    "Allow", "Content-Encoding", "Content-Length", "Content-Type",
    "Expires", "Last-Modified" };

const char * const
HTTP_Request::static_method_strings_[HTTP_Request::NUM_METHOD_STRINGS]
= { "GET", "HEAD", "POST", "PUT" };

// for reasons of efficiency, this class expects buffer to be
// null-terminated, and buflen does NOT include the \0

HTTP_Request::HTTP_Request (void)
  : got_request_line_(0),
    method_(0), uri_(0), version_(0),
    header_strings_(HTTP_Request::static_header_strings_),
    method_strings_(HTTP_Request::static_method_strings_)
{
  for (int i = 0; i < HTTP_Request::NUM_HEADER_STRINGS; i++)
    this->headers_.recognize (this->header_strings_[i]);
}

HTTP_Request::~HTTP_Request (void)
{
  ACE_OS::free(this->method_);
  ACE_OS::free(this->uri_);
  ACE_OS::free(this->version_);
}

int
HTTP_Request::parse_request (ACE_Message_Block &mb)
{
  mb.wr_ptr ()[0] = '\0';

  // Note that RFC 822 does not mention the maximum length of a header
  // line.  So in theory, there is no maximum length.

  // In Apache, they assume that each header line should not exceed 8K.

  if (this->headers_.complete_header_line (mb.rd_ptr ())) {
    if (!this->got_request_line ())
      this->parse_request_line (mb.rd_ptr ());
    else do {
      this->headers_.parse_header_line (mb.rd_ptr ());
    } while (this->headers_.complete_header_line (mb.rd_ptr ()));
  }

  mb.wr_ptr (strlen(mb.rd_ptr ()) - mb.length ());

  return (this->headers_.end_of_headers ()
          ? this->init (mb.rd_ptr (), mb.length ())
          : 0);
}

void
HTTP_Request::parse_request_line (char * const request_line)
{
  char *ptr = request_line;
  char *buf = request_line;
  int offset = 1;

  ptr = ACE_OS::strchr (request_line, '\n');
  if (ptr > request_line && ptr[-1] == '\r')
    ptr--, offset++;
  if (ptr == request_line)
    {
      this->status_ = HTTP_Status_Code::STATUS_BAD_REQUEST;
      return;
    }
  *ptr = '\0';
  ptr += offset;

  char *lasts; // for strtok_r

  // Get the request type.
  this->got_request_line_ = 1;

  if (this->method (ACE_OS::strtok_r (buf, " \t", &lasts))
      && this->uri (ACE_OS::strtok_r (NULL, " \t", &lasts)))
    {
      this->type (this->method ());

      if (this->version (ACE_OS::strtok_r (NULL, " \t", &lasts)) == 0)
        {
          if (this->type () != HTTP_Request::GET)
            this->status_ = HTTP_Status_Code::STATUS_NOT_IMPLEMENTED;
        }
    }

  //  this->cgi_arguments (this->uri);
}

int
HTTP_Request::init (char * const buffer, int buflen)
{
  // Initialize these every time.
  content_length_ = -1;
  filename_ = "";
  status_ = OK;
  type_ = NO_TYPE;

  // Extract the data pointer.
  data_ = NULL;
  datalen_ = 0;

  if ((data_ = ACE_OS::strstr (buffer,"\r\n\r\n")) != NULL) 
    data_ += 4;
  else if ((data_ = ACE_OS::strstr (buffer, "\n\n")) != NULL)
    data_ += 2;
  else
    // Keep going even without a header?
    status_ = NO_HEADER;

  // set the datalen
  if (data_ != NULL)
    datalen_ = (buffer + buflen) - data_;
  else
    datalen_ = 0;

  return 1;
}

const char *
HTTP_Request::method (void) const
{
  return this->method_;
}

const char *
HTTP_Request::uri (void) const
{
  return this->uri_;
}

const char *
HTTP_Request::version (void) const
{
  return this->version_;
}

int
HTTP_Request::cgi (void) const
{
  return this->cgi_;
}

const char *
HTTP_Request::cgi_env (void) const
{
  return this->cgi_env_;
}

const char *
HTTP_Request::cgi_args (void) const
{
  return this->cgi_args_;
}

int HTTP_Request::got_request_line (void) const
{
  return this->got_request_line_;
}

int
HTTP_Request::type (void) const
{
  return type_;
}

char *
HTTP_Request::data (void)
{
  return data_;
}

int
HTTP_Request::data_length (void)
{
  return datalen_;
}

char *
HTTP_Request::filename (void)
{
  return filename_;
}

int
HTTP_Request::content_length (void)
{
  return content_length_;
}

int
HTTP_Request::status (void)
{
  return status_;
}

char *
HTTP_Request::status_string (void)
{
  switch (status_) 
    {
    case OK:
      return "All is well";
    case NO_REQUEST:
      return "Request line missing from HTTP request";
    case NO_CONTENT_LENGTH:
      return "Content-length: field missing in the header";
    case NO_FILENAME:
      return "Filename missing in the header";
    case NO_HEADER:
    default:
      return "No header";
    }
}

void
HTTP_Request::dump (void)
{
  switch (this->type ())
    {
    case GET :
      ACE_DEBUG ((LM_DEBUG, "GET command.\n"
		  "filename is %s,"
		  " length of the file is %d,"
		  " data string is %s,"
		  " datalen is %d,"
		  " status is %d, which is %s\n\n", 
		  this->filename () ? this->filename () : "EMPTY",
		  this->content_length (),
		  this->data () ? this->data () : "EMPTY",
		  this->data_length (),
		  this->status (),
		  this->status_string ()));
      break;

    case PUT :
      ACE_DEBUG ((LM_DEBUG, "PUT command.\n"
		  "filename is %s,"
		  " length of the file is %d,"
		  " data string is %s,"
		  " datalen is %d,"
		  " status is %d, which is %s\n\n", 
		  this->filename () ? this->filename () : "EMPTY",
		  this->content_length (),
		  this->data () ? this->data () : "EMPTY",
		  this->data_length (),
		  this->status (),
		  this->status_string ()));
      break;

    case NO_TYPE :
    default:
      break;
    }
}

const char *
HTTP_Request::method (const char *method_string)
{
  if (this->method_) ACE_OS::free (this->method_);

  if (method_string == 0)
    {
      this->status_ = HTTP_Status_Code::STATUS_BAD_REQUEST;
      this->method_ = 0;
    }
  else this->method_ = ACE_OS::strdup (method_string);

  return this->method_;
}

const char *
HTTP_Request::uri (char *uri_string)
{
  if (this->uri_) ACE_OS::free (this->uri_);

  if (uri_string == 0)
    {
      this->status_ = HTTP_Status_Code::STATUS_BAD_REQUEST;
      this->uri_ = 0;
    }
  else
    {
      this->uri_ =  ACE_OS::strdup (uri_string);
      this->cgi (this->uri_);
      HTTP_Helper::HTTP_decode_string (this->uri_);
    }

  return this->uri_;
}

const char *
HTTP_Request::version (const char *version_string)
{
  if (this->version_) ACE_OS::free (this->version_);

  this->version_ = (version_string
                    ? ACE_OS::strdup(version_string)
                    : 0);

  return this->version_;
}

int
HTTP_Request::type (const char *type_string)
{
  this->type_ = HTTP_Request::NO_TYPE;
  if (type_string == 0) return this->type_;

  for (int i = 0; i < HTTP_Request::NUM_METHOD_STRINGS; i++)
    if (ACE_OS::strcmp(type_string, this->method_strings_[i]) == 0)
      {
        this->type_ = i;
        break;
      }

  if (this->type_ == HTTP_Request::NO_TYPE)
    this->status_ = HTTP_Status_Code::STATUS_NOT_IMPLEMENTED;

  return this->type_;
}

int
HTTP_Request::cgi (char *uri_string)
{
  this->cgi_ = 0;
  this->cgi_env_ = 0;
  this->cgi_args_ = 0;

  char *cgi_question = ACE_OS::strchr (uri_string, '?');
  if (cgi_question)
    {
      this->cgi_ = 1;
      *cgi_question++ = '\0';


      if (*cgi_question != '\0')
        {
          if (ACE_OS::strchr (cgi_question, '='))
            this->cgi_env_ = cgi_question;
          else
            this->cgi_args_ = cgi_question;

          HTTP_Helper::HTTP_decode_string (cgi_question);
        }
    }

  return this->cgi_;
}
