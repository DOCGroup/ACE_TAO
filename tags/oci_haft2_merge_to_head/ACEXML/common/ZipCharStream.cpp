// $Id$

#ifdef ACEXML_HAS_ZZIPLIB

#include "ACEXML/common/ZipCharStream.h"
#include "ace/ACE.h"


ACEXML_ZipCharStream::ACEXML_ZipCharStream (void)
  : filename_ (0), encoding_ (0), size_ (0), infile_ (0), pos_ (0),
    limit_ (0)
{
}

ACEXML_ZipCharStream::~ACEXML_ZipCharStream (void)
{
  this->close();
}

int
ACEXML_ZipCharStream::open (const ACEXML_Char *name)
{
  delete[] this->filename_;
  this->filename_ = 0;

  delete[] this->encoding_;
  this->encoding_ = 0;

  this->infile_ = zzip_fopen (name, ACE_TEXT ("r"));
  if (this->infile_ == 0)
    return -1;

  this->filename_ = ACE::strnew (ACE::basename (name));
  return this->determine_encoding();
}

int
ACEXML_ZipCharStream::determine_encoding (void)
{
  if (this->infile_ == 0)
    return -1;
  char input[4];
  int i = 0;
  for (; i < 4 && (input[i] = this->peekchar_i(i)) > 0; ++i)
    ;
  if (i < 4)
    return -1;
  const ACEXML_Char* temp = ACEXML_Encoding::get_encoding (input);
  if (!temp)
    return -1;
  else
    {
      this->encoding_ = ACE::strnew (temp);
  //     ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("File's encoding is %s\n"),
//                   this->encoding_));
    }
  // Move over the byte-order-mark if present.
  char ch;
  for (int j = 0; j < 3; ++j)
    {
      if ((ch = this->peekchar_i()) < 0)
        return -1;
      if (ch == '\xFF' || ch == '\xFE' || ch == '\xEF' || ch == '\xBB' ||
          ch == '\xBF')
        this->get(ch);
      else
        break;
    }
  return 0;
}

void
ACEXML_ZipCharStream::rewind()
{
  if (this->infile_ == 0)
    return;
  zzip_rewind (this->infile_);
}

int
ACEXML_ZipCharStream::available (void)
{
  if (this->infile_ == 0)
    return -1;
  long curr;
  if ((curr = zzip_tell (this->infile_)) < 0)
    return -1;
  return (this->size_ - curr);
}

int
ACEXML_ZipCharStream::close (void)
{
  if (this->infile_ != 0)
    {
      zzip_close (this->infile_);
      this->infile_ = 0;
    }
  delete[] this->filename_;
  this->filename_ = 0;
  delete[] this->encoding_;
  this->encoding_ = 0;
  this->size_ = 0;
  this->pos_ = 0;
  this->limit_ = 0;
  return 0;
}


int
ACEXML_ZipCharStream::getchar_i (char& ch)
{
  if (this->infile_ == 0)
    return -1;

  if (this->pos_ < this->limit_)
    {
      ch = this->buf_[this->pos_++];
      return 0;
    }
  this->limit_ = zzip_read (this->infile_, this->buf_, sizeof (this->buf_));
  if (this->limit_ == 0)
    return -1;
  this->pos_ = 0;
  ch = this->buf_[this->pos_++];
  return 0;
}

int
ACEXML_ZipCharStream::peekchar_i (off_t offset)
{
  if (this->infile_ == 0)
    return -1;

  if (offset > (off_t) sizeof (this->buf_))
    return -1;
  if (this->pos_ + offset < this->limit_)
    return this->buf_[this->pos_ + offset];
  int i = 0;
  for (; this->pos_ < this->limit_; ++this->pos_, ++i)
    this->buf_[i] = this->buf_[this->pos_];
  this->limit_ = zzip_read (this->infile_, this->buf_ + i,
                            sizeof (this->buf_) - i);
  this->limit_ += i;
  if (this->limit_ == 0)
    return -1;
  this->pos_ = 0;
  return this->buf_[this->pos_ + offset];
}

int
ACEXML_ZipCharStream::read (ACEXML_Char *str, size_t len)
{
  if (this->infile_ == 0)
    return -1;

  size_t i = 0;
  for (; i < len && this->pos_ < this->limit_; ++i)
    str[i] = this->buf_[this->pos_++];
  if (i == len)
    return len;
  len = len - i;
  this->pos_ = 0;
  this->limit_ = 0;
  int bytes = zzip_fread (str + i, sizeof (ACEXML_Char), len, this->infile_);
  return (bytes + i);
}

int
ACEXML_ZipCharStream::get (ACEXML_Char& ch)
{
#if defined (ACE_USES_WCHAR)
  return this->get_i (ch);
#else
  return this->getchar_i (ch);
#endif /* ACE_USES_WCHAR */
}


int
ACEXML_ZipCharStream::peek (void)
{
#if defined (ACE_USES_WCHAR)
  return this->peek_i();
#else
  return this->peekchar_i();
#endif /* ACE_USES_WCHAR */
}

const ACEXML_Char*
ACEXML_ZipCharStream::getEncoding (void)
{
  return this->encoding_;
}

const ACEXML_Char*
ACEXML_ZipCharStream::getSystemId (void)
{
  return this->filename_;
}

#if defined (ACE_USES_WCHAR)
int
ACEXML_ZipCharStream::get_i (ACEXML_Char& ch)
{
  if (ACE_OS::strcmp (this->encoding_, ACE_TEXT ("UTF-8")) == 0)
    return this->getchar_i (ch);

  int BE = (ACE_OS::strcmp (this->encoding_,
                            ACE_TEXT ("UTF-16BE")) == 0) ? 1 : 0;
  ACEXML_Char input[2];
  int i = 0;
  for (; i < 2 && (this->getchar_i (input[i]) == 0); ++i)
    ;
  if (i < 2)
    {
      ch = 0;
      return -1;
    }
  ch = BE ? input[0] << 8 | input[1] : input[1] << 8 | input[0];
  return 0;
}

int
ACEXML_ZipCharStream::peek_i (void)
{

  // If we are reading a UTF-8 encoded file, just use the plain unget.
  if (ACE_OS::strcmp (this->encoding_, ACE_TEXT ("UTF-8")) == 0)
    return this->peekchar_i();

  // Peek into the stream. This reads two characters off the stream, keeps
  // it in peek_.
  int BE = (ACE_OS::strcmp (this->encoding_,
                            ACE_TEXT ("UTF-16BE")) == 0) ? 1 : 0;

  ACEXML_Char input[2];
  int i = 0;
  for (; i < 2 && (input[i] = this->peekchar_i (i)) > 0; ++i)
    ;
  if (i < 2)
    return -1;
  return (BE ? input[0] << 8 | input[1] : input[1] << 8 | input[0]);
}
#endif /* ACE_USES_WCHAR */

#endif /* ACEXML_HAS_ZZIPLIB */
