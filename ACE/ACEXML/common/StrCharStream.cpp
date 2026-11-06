#include "ACEXML/common/StrCharStream.h"
#include "ACEXML/common/Encoding.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

ACEXML_StrCharStream::ACEXML_StrCharStream ()
  : ptr_ {}, end_ {}
{
}

ACEXML_StrCharStream::~ACEXML_StrCharStream ()
{
  this->close();
}

int
ACEXML_StrCharStream::open (ACEXML_Char const *str, ACEXML_Char const *name)
{
  if (str != nullptr && name != nullptr)
    {
      this->start_.reset (ACE::strnew (str));
      if (this->start_ == nullptr)
        return -1;
      this->name_.reset (ACE::strnew (name));
      if (this->name_ == nullptr)
        return -1;
      this->ptr_ = this->start_.get ();
      this->end_ = this->start_.get () + ACE_OS::strlen (this->start_.get ());
      return this->determine_encoding();
    }
  return -1;                // Invalid string passed.
}

int
ACEXML_StrCharStream::available ()
{
  if (this->start_ != nullptr)
    return static_cast<int> (this->end_ - this->start_.get ()); // @@ Will this work on all platforms?
  return -1;
}

int
ACEXML_StrCharStream::close ()
{
  this->start_.reset ();
  this->encoding_.reset ();
  this->name_.reset ();
  this->ptr_ = this->end_ = nullptr;
  return 0;
}

int
ACEXML_StrCharStream::determine_encoding ()
{
  if (this->start_ == nullptr)
    return -1;
  char input[4] = {0,0,0,0};
  char* sptr  = (char*)this->start_.get ();
  int i = 0;
  for ( ; i < 4 && sptr != (char*)this->end_; ++sptr, ++i)
    input[i] = *sptr;
  const ACEXML_Char* temp = ACEXML_Encoding::get_encoding (input);
  if (temp == nullptr)
    return -1;
  else
    {
      this->encoding_.reset (ACE::strnew (temp));
    }
  return 0;
}

void
ACEXML_StrCharStream::rewind ()
{
  this->ptr_ = this->start_.get ();
  this->determine_encoding();
}

int
ACEXML_StrCharStream::get (ACEXML_Char& ch)
{
  if (this->start_ != nullptr && this->ptr_ != this->end_)
    {
      ch = *this->ptr_++;
      return 0;
    }
  return -1;
}

int
ACEXML_StrCharStream::read (ACEXML_Char *str, size_t len)
{
  if (this->start_ != nullptr &&
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
ACEXML_StrCharStream::peek ()
{
  if (this->start_ != nullptr && this->ptr_ != this->end_)
    return *this->ptr_;
  return -1;
}

ACEXML_Char const *
ACEXML_StrCharStream::getEncoding ()
{
  return this->encoding_.get ();
}

ACEXML_Char const *
ACEXML_StrCharStream::getSystemId()
{
  return this->name_.get ();
}
