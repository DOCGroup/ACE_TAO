/* -*- C++ -*- */
#include "URL_Status.h"

URL_Status::URL_Status (STATUS_CODE code)
  : status_ (code)
{
}

URL_Status::STATUS_CODE
URL_Status::status () const
{
  return this->status_;
}

void
URL_Status::status (int s)
{
  this->status_ = URL_Status::STATUS_CODE (s);
}

void
URL_Status::status (URL_Status::STATUS_CODE s)
{
  this->status_ = s;
}

int URL_Status::destroy ()
{
  delete this;
  return 0;
}
