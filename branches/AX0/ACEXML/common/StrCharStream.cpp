// $Id$

#include "common/StrCharStream.h"
#include "ace/ACE.h"

ACEXML_StrCharStream::ACEXML_StrCharStream (void)
  : start_ (0),
    ptr_ (0),
    end_ (0)
{
}

ACEXML_StrCharStream::ACEXML_StrCharStream (const ACEXML_Char *str)
  : start_ (0),
    ptr_ (0),
    end_ (0)
{
  this->open (str);
}


ACEXML_StrCharStream::~ACEXML_StrCharStream (void)
{
  delete this->start_;
}

int
ACEXML_StrCharStream::open (const ACEXML_Char *str)
{
  delete this->start_;

  if (str != 0 &&
      (this->start_ = ACE::strnew (str)) != 0)
    {
      this->ptr_ = this->start_;
      this->end_ = this->start_ + ACE_OS_String::strlen (this->start_);
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
  delete this->start_;
  this->start_ = this->ptr_ = this->end_ = 0;
  return 0;
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
      if (len > this->end_ - this->ptr_)
        len = this->end_ - this->ptr_;

      ACE_OS_String::strncpy (str, this->ptr_, len);
      this->ptr_ += len;
      return len;
    }
  return 0;
}

int
ACEXML_StrCharStream::peek (void)
{
  if (this->start_ !=0 &&
      this->ptr_ != this->end_)
    return *this->ptr_;
  return -1;
}
