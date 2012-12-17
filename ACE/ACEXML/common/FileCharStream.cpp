// $Id$

#include "ACEXML/common/FileCharStream.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/Truncate.h"

#if defined (ACE_USES_WCHAR)
#  include "ace/OS_NS_wchar.h"
#endif /* ACE_USES_WCHAR */

ACEXML_FileCharStream::ACEXML_FileCharStream (void)
  : filename_ (0), encoding_ (0), size_ (0), infile_ (0),
    close_infile_ (true), peek_ (0)
{
}

ACEXML_FileCharStream::~ACEXML_FileCharStream (void)
{
  this->close();
}

int
ACEXML_FileCharStream::use_stream_i (FILE* open_file, const ACEXML_Char *name)
{
  delete[] this->filename_;
  this->filename_ = 0;

  delete[] this->encoding_;
  this->encoding_ = 0;

  this->infile_ = open_file;
  if (this->infile_ == 0)
    return -1;

  ACE_stat statbuf;
  if (ACE_OS::stat (name, &statbuf) < 0)
    return -1;

  this->size_ = ACE_Utils::truncate_cast<ACE_OFF_T> (statbuf.st_size);
  this->filename_ = ACE::strnew (name);
  return this->determine_encoding();
}

int
ACEXML_FileCharStream::use_stream (FILE* open_file, const ACEXML_Char *name)
{
  if (open_file != 0)
    ACE_OS::rewind(open_file);

  this->close_infile_ = false;
  return use_stream_i(open_file, name);
}

int
ACEXML_FileCharStream::open (const ACEXML_Char *name)
{
  this->close_infile_ = true;
  return use_stream_i(ACE_OS::fopen (name, ACE_TEXT ("r")), name);
}

int
ACEXML_FileCharStream::determine_encoding (void)
{
  if (this->infile_ == 0)
    return -1;

  char input[4];
  int retval = 0;
  int i = 0;
  for (; i < 4 && retval != -1; ++i)
    retval = this->getchar_i(input[i]);
  if (i < 4)
    return -1;

  // Rewind the stream
  ACE_OS::rewind (this->infile_);

  const ACEXML_Char* temp = ACEXML_Encoding::get_encoding (input);
  if (!temp)
    return -1;
  else
    {
      delete [] this->encoding_;
      this->encoding_ = ACE::strnew (temp);
//       ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("File's encoding is %s\n"),
//                   this->encoding_));
    }
  // Move over the byte-order-mark if present.
  char ch;
  for (int j = 0; j < 3; ++j)
    {
      if (this->getchar_i (ch) < 0)
        return -1;
      if (ch == '\xFF' || ch == '\xFE' || ch == '\xEF' || ch == '\xBB' ||
          ch == '\xBF')
        continue;
      else
        {
          ACE_OS::ungetc (ch, this->infile_);
          break;
        }
    }
  return 0;
}

void
ACEXML_FileCharStream::rewind()
{
  if (this->infile_ == 0)
    return;
  ACE_OS::rewind (this->infile_);
  this->determine_encoding();
}

int
ACEXML_FileCharStream::available (void)
{
  if (this->infile_ == 0)
    return -1;

  long curr;
  if ((curr = ACE_OS::ftell (this->infile_)) < 0)
    return -1;
  return static_cast<int> (this->size_ - curr);
}

int
ACEXML_FileCharStream::close (void)
{
  if (this->infile_ != 0)
    {
      if (this->close_infile_)
        {
          ACE_OS::fclose (this->infile_);
        }
      this->infile_ = 0;
    }
  delete[] this->filename_;
  this->filename_ = 0;
  delete[] this->encoding_;
  this->encoding_ = 0;
  this->size_ = 0;
  this->peek_ = 0;
  return 0;
}


int
ACEXML_FileCharStream::getchar_i (char& ch)
{
  ch = static_cast<char> (ACE_OS::fgetc (this->infile_));
  return (feof(this->infile_) ? -1 : 0);
}

int
ACEXML_FileCharStream::read (ACEXML_Char *str,
                             size_t len)
{
  if (this->infile_ == 0)
    return -1;

  return static_cast<int> (ACE_OS::fread (str, sizeof (ACEXML_Char), len, this->infile_));
}

int
ACEXML_FileCharStream::get (ACEXML_Char& ch)
{
  if (this->infile_ == 0)
    return -1;
#if defined (ACE_USES_WCHAR)
  return this->get_i (ch);
#else
  ch = (ACEXML_Char) ACE_OS::fgetc (this->infile_);
  return (feof(this->infile_) ? -1 : 0);
#endif /* ACE_USES_WCHAR */
}

int
ACEXML_FileCharStream::peek (void)
{
  if (this->infile_ == 0)
    return -1;
#if defined (ACE_USES_WCHAR)
  return this->peek_i();
#else

  ACEXML_Char ch = static_cast<ACEXML_Char> (ACE_OS::fgetc (this->infile_));
  ACE_OS::ungetc (ch, this->infile_);
  return ch;
#endif /* ACE_USES_WCHAR */
}

#if defined (ACE_USES_WCHAR)
int
ACEXML_FileCharStream::get_i (ACEXML_Char& ch)
{
  if (ACE_OS::strcmp (this->encoding_, ACE_TEXT ("UTF-8")) == 0)
    {
      ch = (ACEXML_Char) ACE_OS::fgetc (this->infile_);
      return (feof(this->infile_) ? -1 : 0);
    }
  // If we have a value in peek_, return it.
  if (this->peek_ != 0)
    {
      ch = this->peek_;
      this->peek_ = 0;
      return 0;
    }

  int BE = (ACE_OS::strcmp (this->encoding_,
                            ACE_TEXT ("UTF-16BE")) == 0) ? 1 : 0;
  ACEXML_Char input[2];
  int i = 0;
  for (; i < 2 && !feof (this->infile_); ++i)
    {
      input[i] = ACE_OS::fgetwc (this->infile_);
    }
  if (i < 2)
    {
      ch = 0;
      return -1;
    }
  ch = BE ? input[0] << 8 | input[1] : input[1] << 8 | input[0];
  return 0;
}

int
ACEXML_FileCharStream::peek_i (void)
{
  // If we are reading a UTF-8 encoded file, just use the plain unget.
  if (ACE_OS::strcmp (this->encoding_, ACE_TEXT ("UTF-8")) == 0)
    {
      ACEXML_Char ch = (ACEXML_Char) ACE_OS::fgetc (this->infile_);
      ACE_OS::ungetc (ch, this->infile_);
      return ch;
    }

  // If somebody had already called peek() and not consumed it, return the
  // value held in this->peek_.
  if (this->peek_ != 0)
    return this->peek_;

  // Peek into the stream. This reads two characters off the stream, keeps
  // it in peek_.
  int BE = (ACE_OS::strcmp (this->encoding_,
                            ACE_TEXT ("UTF-16BE")) == 0) ? 1 : 0;
  ACEXML_Char input[2];
  int i = 0;
  for (; i < 2 && !feof (this->infile_); ++i)
    {
      input[i] = ACE_OS::fgetwc (this->infile_);
    }
  if (i < 2)
    {
      this->peek_ = 0;
      return -1;
    }
  this->peek_ = BE ? input[0] << 8 | input[1] : input[1] << 8 | input[0];
  return this->peek_;
}
#endif /* ACE_USES_WCHAR */

const ACEXML_Char*
ACEXML_FileCharStream::getEncoding (void)
{
  return this->encoding_;
}

const ACEXML_Char*
ACEXML_FileCharStream::getSystemId (void)
{
  return this->filename_;
}
