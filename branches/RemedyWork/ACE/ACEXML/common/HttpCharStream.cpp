// $Id$

#include "ace/ACE.h"
#include "ace/ace_wchar.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ACEXML/common/HttpCharStream.h"
#include "ACEXML/common/Encoding.h"



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
    size_(0),
    data_offset_ (0),
    encoding_ (0)
{

}

ACEXML_HttpCharStream::~ACEXML_HttpCharStream (void)
{
  this->close ();
}

int
ACEXML_HttpCharStream::open (const ACEXML_Char *url)
{
  this->url_ = ACE::strnew (url);

  ACE_NEW_RETURN (this->url_addr_, ACEXML_URL_Addr, -1);
  ACE_NEW_RETURN (this->stream_, ACEXML_Mem_Map_Stream, -1);

  if (this->url_addr_->string_to_addr (this->url_) == -1) {
    this->close();
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "cannot convert URL"), -1);
  }

  ACE_NEW_RETURN (this->connector_,
                  Connector (0, ACE_NONBLOCK),
                  -1);

  if (this->stream_->open (this->connector_, *this->url_addr_) == -1) {
    this->close();
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "cannot open backing store"), -1);
  }

  int result = this->send_request();
  if (result == -1) {
    this->close();
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send_request"), -1);
  }

  size_t len = 0;
  result = this->get_url(len);
  if (result == -1) {
    this->close();
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_url"), -1);
  }
  if (result != 200) {
    this->close();
    ACE_ERROR_RETURN ((LM_ERROR, "Server returned status %d : %s\n",
                       result,
                       "Refer HTTP/1.0 error code for details"), -1);
  }

  this->size_ = static_cast<ACE_OFF_T> (len);
  return this->determine_encoding();
}

// The FSM was taken from the implementation of http_get and that falls
// under the following license:
//
// Copyrigh (c) 2000 by Jef Poskanzer <jef@acme.com>.  All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.


int
ACEXML_HttpCharStream::get_url (size_t& len)
{
  if (this->stream_ == 0)
    {
      return -1;
    }

  int header_state = HDST_LINE1_PROTOCOL;
  int status = 0;
  size_t b = 0;
  char const * buf = 0;
  size_t buflen = BUFSIZ;

  for (;;)
    {
      buf = this->stream_->recv (buflen);

      if (buf == 0)
        {
          if (buflen == 0)
            {
              break;
            }
          else
            {
              continue;
            }
        }

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

  if (b == 0)
    {
      return -1;
    }

  ++b;
  // Store the address of the beginning of data. We will use it to seek to
  // beginning of the data in the URL.
  char const * const data_beg = buf + b;
  buflen = BUFSIZ;

  // Get all of the data. Since this is backed by file store, we won't lose
  // any of the data.
  while ((buf = this->stream_->recv (buflen)) != 0)
    ;

  // Length of data in the URL.
  len = this->stream_->recv() - data_beg;

  // Move the pointer to the beginning of the file store.
  this->stream_->rewind();

  this->data_offset_ =
    ACE_Utils::truncate_cast<ACE_OFF_T> (data_beg - this->stream_->recv());

  // Forward to the beginning of data.
  if (this->stream_->seek (this->data_offset_, SEEK_SET) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: %m",
                         "Error in seeking to beginning of data"),
                        -1);
    }

  return status;
}


int
ACEXML_HttpCharStream::send_request (void)
{
  char* path = ACE::strnew (ACE_TEXT_ALWAYS_CHAR (this->url_addr_->get_path_name()));
  ACE_Auto_Basic_Array_Ptr<char> path_ptr (path);
  size_t commandsize = ACE_OS::strlen (path)
                       + ACE_OS::strlen (this->url_addr_->get_host_name ())
                       + 20     // Extra
                       + 1      // NUL byte
                       + 16 ;   // Protocol filler...

  char* command;
  ACE_NEW_RETURN (command, char[commandsize], -1);

  // Ensure that the <command> memory is deallocated.
  ACE_Auto_Basic_Array_Ptr<char> cmd_ptr (command);

  int bytes = ACE_OS::sprintf (command, "GET %s HTTP/1.0\r\n", path);
  bytes += ACE_OS::sprintf (&command[bytes], "Host: %s\r\n",
                            this->url_addr_->get_host_name ());
  bytes += ACE_OS::sprintf (&command[bytes], "\r\n");

  ACE_Time_Value tv (ACE_DEFAULT_TIMEOUT);

  // Send the command to the connected server.
  int retval = static_cast<int> (this->stream_->send_n (command, bytes, &tv));
  if (retval <= 0)
    return -1;
  return retval;
}


int
ACEXML_HttpCharStream::available (void)
{
  if (this->stream_ == 0)
    return -1;
  return static_cast<int> (this->stream_->available());
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
  this->data_offset_ = 0;

  delete[] this->encoding_;
  this->encoding_ = 0;

  return 0;
}

int
ACEXML_HttpCharStream::determine_encoding (void)
{
  if (this->stream_ == 0)
    return -1;

  char input[] = {0, 0, 0, 0};
  size_t const len = sizeof (input) / sizeof (input[0]);

  size_t i = 0;
  for (; i < len && input[i] != static_cast<char> (EOF); ++i)
    input[i] = this->stream_->peek_char (i);

  if (i < len)
    return -1;

  ACEXML_Char const * const temp = ACEXML_Encoding::get_encoding (input);

  if (!temp)
    return -1;
  else
    {
      if (this->encoding_)
        delete [] this->encoding_;

      this->encoding_ = ACE::strnew (temp);
      //   ACE_DEBUG ((LM_DEBUG, "URI's encoding is %s\n", this->encoding_));
    }

  // Move over the byte-order-mark if present.
  for (size_t j = 0; j < len; ++j)
    {
      if (input[j] == '\xFF' || input[j] == '\xFE' || input[j] == '\xEF' ||
          input[j] == '\xBB' || input[j] == '\xBF')
        {
          this->stream_->get_char();
          continue;
        }
      break;
    }

  return 0;
}

void
ACEXML_HttpCharStream::rewind (void)
{
  if (this->stream_ == 0)
    return;
  this->stream_->rewind();

  // Forward to the beginning of data.
  if (this->stream_->seek (this->data_offset_, SEEK_SET) == -1)
    ACE_ERROR ((LM_ERROR, "%s: %m", "Error in seeking to beginning of data"));
  this->determine_encoding();
}

const ACEXML_Char*
ACEXML_HttpCharStream::getEncoding (void)
{
  return this->encoding_;
}

const ACEXML_Char*
ACEXML_HttpCharStream::getSystemId (void)
{
  return this->url_;
}


int
ACEXML_HttpCharStream::read (ACEXML_Char *str,
                             size_t len)
{
  if (this->stream_ == 0)
    return -1;
  len = len * sizeof (ACEXML_Char);
  const char* temp = this->stream_->recv (len);
  if (temp == 0)
    return -1;
  ACE_OS::strncpy (str, ACE_TEXT_CHAR_TO_TCHAR (temp), len);
  return static_cast<int> (len);
}


int
ACEXML_HttpCharStream::get (ACEXML_Char& ch)
{
  if (this->stream_ == 0)
    return -1;
#if defined (ACE_USES_WCHAR)
  return this->get_i (ch);
#else
  ch = (ACEXML_Char) this->stream_->get_char();
  return (ch == (ACEXML_Char)EOF ? -1 :0);
#endif /* ACE_USES_WCHAR */
}

int
ACEXML_HttpCharStream::peek (void)
{
  if (this->stream_ == 0)
    return -1;

#if defined (ACE_USES_WCHAR)
  return this->peek_i();
#else
  return this->stream_->peek_char (0);
#endif /* ACE_USES_WCHAR */
}


#if defined (ACE_USES_WCHAR)
int
ACEXML_HttpCharStream::get_i (ACEXML_Char& ch)
{
  if (ACE_OS::strcmp (this->encoding_, ACE_TEXT ("UTF-8")) == 0)
    {
      ch = (ACEXML_Char) this->stream_->get_char();
      return (ch == (ACEXML_Char)EOF ? -1 : 0);
    }
  int BE = (ACE_OS::strcmp (this->encoding_,
                            ACE_TEXT ("UTF-16BE")) == 0) ? 1 : 0;
  ACEXML_Char input[2] = {0};
  int i = 0;
  for (; i < 2 && (input[i] = this->stream_->get_char()) > 0; ++i)
    ;
  if (i < 2)
    {
      ch = 0;
      return input[i];
    }
  ch = BE ? input[0] << 8 | input[1] : input[1] << 8 | input[0];
  return 0;
}

int
ACEXML_HttpCharStream::peek_i (void)
{
  // If we are reading a UTF-8 encoded file, just use the plain unget.
  if (ACE_OS::strcmp (this->encoding_, ACE_TEXT ("UTF-8")) == 0)
    {
      ACEXML_Char ch = (ACEXML_Char) this->stream_->peek_char (0);
      return ch;
    }

  int BE = (ACE_OS::strcmp (this->encoding_,
                            ACE_TEXT ("UTF-16BE")) == 0) ? 1 : 0;
  // Peek into the stream.
  ACEXML_Char input[2];
  int i = 0;
  for (; i < 2 && (input[i] = this->stream_->peek_char (i)) > 0; ++i)
    ;
  if (i < 2)
    return -1;
  return (BE ? input[0] << 8 | input[1] : input[1] << 8 | input[0]);
}
#endif /* ACE_USES_WCHAR */
