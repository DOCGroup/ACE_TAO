// $Id$

#include "ACEXML/common/StrCharStream.h"
#include "ACEXML/common/Encoding.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"

ACEXML_StrCharStream::ACEXML_StrCharStream (void)
  : start_ (0), ptr_ (0), end_ (0), encoding_ (0)
{
}

ACEXML_StrCharStream::ACEXML_StrCharStream (const ACEXML_Char *str)
  : start_ (0), ptr_ (0), end_ (0), encoding_ (0)
{
  this->open (str);
}


ACEXML_StrCharStream::~ACEXML_StrCharStream (void)
{
  this->close();
}

int
ACEXML_StrCharStream::open (const ACEXML_Char *str)
{
  delete[] this->start_;
  delete[] this->encoding_;

  if (str != 0 && (this->start_ = ACE::strnew (str)) != 0)
    {
      this->ptr_ = this->start_;
      this->end_ = this->start_ + ACE_OS_String::strlen (this->start_);
      this->determine_encoding();
      return 0;
    }

  this->start_ = this->ptr_ = this->end_ = 0;
  return -1;                // Invalid string passed.
}

int
ACEXML_StrCharStream::available (void)
{
  if (this->start_ != 0)
    return (this->end_ - this->start_); // @@ Will this work on all platforms?
  return -1;
}

int
ACEXML_StrCharStream::close (void)
{
  delete[] this->start_;
  delete[] this->encoding_;
  this->start_ = this->ptr_ = this->end_ = 0;
  return 0;
}

int
ACEXML_StrCharStream::determine_encoding (void)
{
  char input[4];
  int retval = 0;
  char* sptr  = (char*)this->start_;
  int i = 0;
  for ( ; i < 4 && sptr != (char*)this->end_; ++sptr, ++i)
    {
      retval = input[i] = *sptr;
    }
  if (i < 4)
    return -1;
  const ACEXML_Char* temp = ACEXML_Encoding::get_encoding (input);
  if (!temp)
    return -1;
  if (ACE_OS::strcmp (temp,
                      ACEXML_Encoding::encoding_names_[ACEXML_Encoding::OTHER]) == 0)
    return -1;
  else
    {
      this->encoding_ = ACE::strnew (temp);
      ACE_DEBUG ((LM_DEBUG, "String's encoding is %s\n", this->encoding_));
    }
  return 0;
}

void
ACEXML_StrCharStream::rewind (void)
{
  this->ptr_ = this->start_;
}

int
ACEXML_StrCharStream::get (ACEXML_Char& ch)
{
  if (this->start_ != 0 && this->ptr_ != this->end_)
    {
      ch = *this->ptr_++;

      return 0;
    }

  return -1;
}

int
ACEXML_StrCharStream::read (ACEXML_Char *str,
                            size_t len)
{
  if (this->start_ != 0 &&
      this->ptr_ != this->end_)
    {
      if ((int) len > this->end_ - this->ptr_)
        len = this->end_ - this->ptr_;

      ACE_OS_String::strncpy (str, this->ptr_, len);
      this->ptr_ += len;
      return ACE_static_cast (int, len);
    }
  return 0;
}

int
ACEXML_StrCharStream::peek (void)
{
  if (this->start_ != 0 && this->ptr_ != this->end_)
    return *this->ptr_;
  return -1;
}

const ACEXML_Char*
ACEXML_StrCharStream::getEncoding (void)
{
  return this->encoding_;
}
