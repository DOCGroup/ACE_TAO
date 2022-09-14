#include "URL.h"


Mem_Map_Stream &
URL::stream ()
{
  return this->stream_;
}

URL::~URL ()
{
}

const URL_Status &
URL::reply_status ()
{
  return this->reply_status_;
}

void
URL::reply_status (const URL_Status &rs)
{
  this->reply_status_ = rs;
}

const ACE_CString &
URL::content_type ()
{
  return this->content_type_;
}

void
URL::content_type (const ACE_CString &ct)
{
  this->content_type_ = ct;
}
