// $Id$

#include "ace/ACE.h"
#include "ace/Auto_Ptr.h"

#include "ACEXML/common/HttpCharStream.h"

/* Header FSM states. */
static const int HDST_LINE1_PROTOCOL = 0;
static const int HDST_LINE1_WHITESPACE = 1;
static const int HDST_LINE1_STATUS = 2;
static const int HDST_BOL = 10;
static const int HDST_TEXT = 11;
static const int HDST_LF = 12;
static const int HDST_CR = 13;
static const int HDST_CRLF = 14;
static const int HDST_CRLFCR = 15;

ACEXML_HttpCharStream::ACEXML_HttpCharStream (void)
  : url_(0),
    url_addr_(0),
    stream_(0),
    connector_(0),
    size_(0)
{

}

ACEXML_HttpCharStream::ACEXML_HttpCharStream (const ACEXML_Char *url)
  : url_(0),
    url_addr_(0),
    stream_(0),
    connector_(0),
    size_(0)
{
  this->open (url);
}


ACEXML_HttpCharStream::~ACEXML_HttpCharStream (void)
{
  this->close ();
}

int
ACEXML_HttpCharStream::open (const ACEXML_Char *url)
{
  delete[] this->url_;
  this->url_ = 0;

  this->url_ = ACE::strnew (url);

  ACE_NEW_RETURN (this->url_addr_, ACEXML_URL_Addr, -1);
  ACE_NEW_RETURN (this->stream_, ACEXML_Mem_Map_Stream, -1);

  if (this->url_addr_->string_to_addr (this->url_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "cannot convert URL"), -1);

  ACE_NEW_RETURN (this->connector_,
                  ACEXML_Mem_Map_Stream::Connector (0, ACE_NONBLOCK),
                  -1);

  if (this->stream_->open (this->connector_, *this->url_addr_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "cannot open backing store"), -1);

  int result = this->send_request();
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send_request"), -1);

  size_t len = 0;
  result = this->get_url(len);
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_url"), -1);
  if (result != 200)
    ACE_ERROR_RETURN ((LM_ERROR, "Server returned status %d : %s",
                       result,
                       "Refer HTTP/1.0 for details"), -1);

  this->size_ = len;
  return 0;
}


int
ACEXML_HttpCharStream::get_url (size_t& len)
{
  int header_state = HDST_LINE1_PROTOCOL;
  int status = 0;
  size_t b;
  ACEXML_Char* buf = 0;
  size_t buflen = BUFSIZ;
  for (;;)
    {
      if ((buf = ACE_const_cast (ACEXML_Char*,
                                 this->stream_->recv (buflen))) == 0)
        if (buflen <= 0)
          break;

      for (b = 0; b < buflen; ++b)
        {
          switch ( header_state )
            {
            case HDST_LINE1_PROTOCOL:
              switch ( buf[b] )
                {
                case ' ': case '\t':
                  header_state = HDST_LINE1_WHITESPACE; break;
                case '\n': header_state = HDST_LF ; break;
                case '\r': header_state = HDST_CR; break;
                }
              break;
            case HDST_LINE1_WHITESPACE:
              switch ( buf[b] )
                {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                  status = buf[b] - '0';
                  header_state = HDST_LINE1_STATUS;
                  break;
                case '\n': header_state = HDST_LF ; break;
                case '\r': header_state = HDST_CR; break;
                default: header_state = HDST_TEXT; break;
                }
              break;
            case HDST_LINE1_STATUS:
              switch ( buf[b] )
                {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                  status = status * 10 + buf[b] - '0';
                  break;
                case '\n': header_state = HDST_LF ; break;
                case '\r': header_state = HDST_CR; break;
                default: header_state = HDST_TEXT; break;
                }
              break;
            case HDST_BOL:
              switch ( buf[b] )
                {
                case '\n': header_state = HDST_LF; break;
                case '\r': header_state = HDST_CR; break;
                default: header_state = HDST_TEXT; break;
                }
              break;
            case HDST_TEXT:
              switch ( buf[b] )
                {
                case '\n': header_state = HDST_LF; break;
                case '\r': header_state = HDST_CR; break;
                }
              break;

            case HDST_LF:
              switch ( buf[b] )
                {
                case '\n': goto end_of_headers;
                case '\r': header_state = HDST_CR; break;
                default: header_state = HDST_TEXT; break;
                }
              break;

            case HDST_CR:
              switch ( buf[b] )
                {
                case '\n': header_state = HDST_CRLF; break;
                case '\r': goto end_of_headers;
                default: header_state = HDST_TEXT; break;
                }
              break;

            case HDST_CRLF:
              switch ( buf[b] )
                {
                case '\n': goto end_of_headers;
                case '\r': header_state = HDST_CRLFCR; break;
                default: header_state = HDST_TEXT; break;
                }
              break;

            case HDST_CRLFCR:
              switch ( buf[b] )
                {
                case '\n': case '\r': goto end_of_headers;
                default: header_state = HDST_TEXT; break;
                }
              break;
            }
        }
    }
 end_of_headers:
  ++b;
  // Store the address of the beginning of data. We will use it to seek to
  // beginning of the data in the URL.
  ACEXML_Char* data_beg = buf + b;
  buflen = BUFSIZ;

  // Get all of the data. Since this is backed by file store, we won't lose
  // any of the data.
  while (( buf = ACE_const_cast (ACEXML_Char*,
                                 this->stream_->recv (buflen))) != 0)
    ;

  // Length of data in the URL.
  len = this->stream_->recv() - data_beg;

  // Move the pointer to the beginning of the file store.
  this->stream_->rewind();

  off_t data_offset = data_beg - this->stream_->recv();
  // Forward to the beginning of data.
  if (this->stream_->seek (data_offset, SEEK_SET) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%s: %m",
                       "Error in seeking to beginning of data"), -1);
  return status;
}


int
ACEXML_HttpCharStream::send_request (void)
{
  int commandsize = ACE_OS::strlen (this->url_addr_->get_path_name ())
    + ACE_OS::strlen (this->url_addr_->get_host_name ())
    + 20    // Extra
    + 1     // NUL byte
    + 16;   // Protocol filler...

  ACEXML_Char* command;
  ACE_NEW_RETURN (command, ACEXML_Char[commandsize], -1);

  // Ensure that the <command> memory is deallocated.
  ACE_Auto_Basic_Array_Ptr<ACEXML_Char> cmd_ptr (command);

  int bytes = ACE_OS::sprintf (command,
                               "GET %s HTTP/1.0\r\n",
                               this->url_addr_->get_path_name ());
  bytes += ACE_OS::sprintf (&command[bytes],
                            "Host: %s\r\n",
                            this->url_addr_->get_host_name());
  bytes += ACE_OS::sprintf (&command[bytes], "\r\n");

  ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);

  // Send the command to the connected server.
  int retval = this->stream_->send_n (command, bytes, &tv);
  if (retval <= 0)
    return -1;
  return retval;
}


int
ACEXML_HttpCharStream::available (void)
{
  return this->stream_->available();
}

int
ACEXML_HttpCharStream::close (void)
{
  delete[] this->url_;
  this->url_ = 0;

  delete this->url_addr_;
  this->url_addr_ = 0;

  delete this->stream_;
  this->stream_ = 0;

  delete this->connector_;
  this->connector_ = 0;

  this->size_ = 0;
  return 0;
}

int
ACEXML_HttpCharStream::get (ACEXML_Char& ch)
{
  ch = (unsigned char) this->stream_->get_char();
  return (((char) ch) == EOF ? -1 :0);
}

int
ACEXML_HttpCharStream::read (ACEXML_Char *str,
                             size_t len)
{
  str = ACE_const_cast (ACEXML_Char*, this->stream_->recv (len));
  if (str == 0)
    return -1;
  return len;
}

int
ACEXML_HttpCharStream::peek (void)
{
  return this->stream_->peek_char (1);
}
