// $Id$

#include "ACEXML/common/StrCharStream.h"
#include "ACEXML/common/Encoding.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

ACEXML_StrCharStream::ACEXML_StrCharStream (void)
  : start_ (0), ptr_ (0), end_ (0), encoding_ (0), name_ (0)
{
}


ACEXML_StrCharStream::~ACEXML_StrCharStream (void)
{
  this->close();
}

int
ACEXML_StrCharStream::open (const ACEXML_Char *str, const ACEXML_Char* name)
{

  if (str != 0 && name != 0)
    {
      delete [] this->start_;
      if ((this->start_ = ACE::strnew (str)) == 0)
        return -1;
      delete [] this->name_;
      if ((this->name_ = ACE::strnew (name)) == 0)
        return -1;
      this->ptr_ = this->start_;
      this->end_ = this->start_ + ACE_OS::strlen (this->start_);
      return this->determine_encoding();
    }
  return -1;                // Invalid string passed.
}

int
ACEXML_StrCharStream::available (void)
{
  if (this->start_ != 0)
    return static_cast<int> (this->end_ - this->start_); // @@ Will this work on all platforms?
  return -1;
}

int
ACEXML_StrCharStream::close (void)
{
  delete[] this->start_;
  delete[] this->encoding_;
  this->encoding_ = 0;
  delete[] this->name_;
  this->name_ = 0;
  this->start_ = this->ptr_ = this->end_ = 0;
  return 0;
}

int
ACEXML_StrCharStream::determine_encoding (void)
{
  if (this->start_ == 0)
    return -1;
  char input[4] = {0,0,0,0};
  char* sptr  = (char*)this->start_;
  int i = 0;
  for ( ; i < 4 && sptr != (char*)this->end_; ++sptr, ++i)
    input[i] = *sptr;
  const ACEXML_Char* temp = ACEXML_Encoding::get_encoding (input);
  if (!temp)
    return -1;
  else
    {
      delete [] this->encoding_;
      this->encoding_ = ACE::strnew (temp);
      // ACE_DEBUG ((LM_DEBUG, "String's encoding is %s\n", this->encoding_));
    }
  return 0;
}

void
ACEXML_StrCharStream::rewind (void)
{
  this->ptr_ = this->start_;
  this->determine_encoding();
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
ACEXML_StrCharStream::read (ACEXML_Char *str, size_t len)
{
  if (this->start_ != 0 &&
      this->ptr_ != this->end_)
    {
      if (len * sizeof (ACEXML_Char) > (size_t) (this->end_ - this->ptr_))
        len = this->end_ - this->ptr_;
      ACE_OS::strncpy (str, this->ptr_, len);
      this->ptr_ += len;
      return static_cast<int> (len);
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

const ACEXML_Char*
ACEXML_StrCharStream::getSystemId(void)
{
  return this->name_;
}
