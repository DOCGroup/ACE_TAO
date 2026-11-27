#include "URL.h"

Mem_Map_Stream &
URL::stream ()
{
  return this->stream_;
}

URL::~URL () = default;

URL_Status const &
URL::reply_status ()
{
  return this->reply_status_;
}

void
URL::reply_status (URL_Status const &rs)
{
  this->reply_status_ = rs;
}

ACE_CString const &
URL::content_type ()
{
  return this->content_type_;
}

void
URL::content_type (ACE_CString const &ct)
{
  this->content_type_ = ct;
}
