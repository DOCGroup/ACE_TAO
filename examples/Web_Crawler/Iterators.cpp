// $Id$

#include "ace/Truncate.h"
#include "Options.h"
#include "Iterators.h"



URL_Iterator::~URL_Iterator (void)
{
}

int
URL_Iterator::destroy (void)
{
  // Commit suicide.
  delete this;
  return 0;
}

HTML_Body_Iterator::HTML_Body_Iterator (URL &url)
  : url_ (url)
{
}

int
HTML_Body_Iterator::next (ACE_CString &url)
{
  size_t len = BUFSIZ;
  const char *buf;
  ACE_CString buffer;
  int href_index = 0;

  for (buf = this->url_.stream ().recv (len);
       buf > 0;
       buf = this->url_.stream ().recv (len))
    {

      buffer.set (buf, BUFSIZ, 1);

      href_index = ACE_Utils::truncate_cast<int> (buffer.find ("HREF"));

      if (href_index < 0)
        href_index = ACE_Utils::truncate_cast<int> (buffer.find ("href"));

      // Grep fpr " and grab the string until end-"
      if ( href_index > 0)
        {
          // Get back to buffer start location.
          this->url_.stream ().seek (-1 * static_cast<ACE_OFF_T> (len),
                                     SEEK_CUR);

          int start_index =
            ACE_Utils::truncate_cast<int> (
              buffer.find ('\"', href_index));

          if (start_index <= 0)
            break;

          start_index += href_index;

          int end_index =
            ACE_Utils::truncate_cast<int> (
              buffer.find ('\"', start_index + 1));

          if (end_index <= 0)
            break;

          end_index += start_index + 1;

          ssize_t url_len = end_index - (start_index + 1);

          ACE_CString temp = buffer.substring (start_index + 1,
                                               url_len);
          url.set (temp.c_str (), len, 1);

          this->url_.stream ().seek (end_index + 1);

          return url_len;
        }
    }
  return 0;

}

HTTP_Header_Iterator::HTTP_Header_Iterator (URL &url)
  : url_ (url),
    end_of_header_ (0)
{
}

int
HTTP_Header_Iterator::next (ACE_CString &line)
{
  if (this->end_of_header_)
    return 0;
  else
    {
      for (char c;
           (c = this->url_.stream ().get_char ()) != (char)EOF;
           )
        {
          // Check to see if we're at the end of the header line.
          if (c == '\r' && this->url_.stream ().peek_char (0) == '\n')
            {
              line.set (this->url_.stream ().recv (),
                        this->url_.stream ().recv_len () - 1,
                        1);

              // Check to see if we're at the end of the header.
              if (this->url_.stream ().peek_char (1) == '\r'
                  && this->url_.stream ().peek_char (2) == '\n')
                {
                  this->end_of_header_ = 1;
                  // We're at the end of the header section.
                  this->url_.stream ().seek (3);
                }
              else
                // We're at the end of the line.
                this->url_.stream ().seek (1);

              return 1;
            }
          // Handle broken Web servers that use '\n' instead of
          // '\r\n'.
          else if (c == '\n')
            {
              line.set (this->url_.stream ().recv (),
                        (this->url_.stream ().recv_len ()),
                        1);

              // Check to see if we're at the end of the header.
              if (this->url_.stream ().peek_char (0) == '\n')
                {
                  // We're at the end of the header section.
                  this->url_.stream ().seek (1);
                  this->end_of_header_ = 1;
                }

              return 1;
            }
        }

    }
  return 0;
}

URL_Download_Iterator::URL_Download_Iterator (URL &url)
  : url_ (url)
{
}

int
URL_Download_Iterator::next (ACE_CString &buffer)
{
  size_t len = BUFSIZ;

  const char *buf = this->url_.stream ().recv (len);


  if (buf == 0)
    return 0;
  else
    {
      buffer.set (buf, len, 1);
      return 1;
    }
}
