// -*- C++ -*-
#include "Manager_i.h"

Manager_i::Manager_i (long retry_count)
  : retry_count_ (retry_count)
{
}

CORBA::Long Manager_i::registerServer ()
{
  if (retry_attempt_ < retry_count_)
  {
    ++retry_attempt_;
    return -1;
  }
  return ++server_instance_;
}

void Manager_i::startRetry (CORBA::Long count)
{
  retry_count_ = count;
  retry_attempt_ = 0;
}

CORBA::Long Manager_i::endRetry ()
{
  return retry_count_ - retry_attempt_;
}
