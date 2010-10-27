// $Id$

#include "URL.h"



Mem_Map_Stream &
URL::stream (void)
{
  return this->stream_;
}

URL::~URL (void)
{
}

const URL_Status &
URL::reply_status (void)
{
  return this->reply_status_;
}

void
URL::reply_status (const URL_Status &rs)
{
  this->reply_status_ = rs;
}

const ACE_CString &
URL::content_type (void)
{
  return this->content_type_;
}

void
URL::content_type (const ACE_CString &ct)
{
  this->content_type_ = ct;
}
