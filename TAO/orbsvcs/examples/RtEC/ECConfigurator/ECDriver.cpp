// $Id$

#include "ECDriver.h"

ECDriver::ECDriver (void)
{
}

ECDriver::~ECDriver (void)
{
}

ECTestDriver::ECTestDriver (void)
{
}

ECTestDriver::~ECTestDriver (void)
{
}

bool
ECTestDriver::get_time_master (void) const
{
  return this->time_master;
}

void
ECTestDriver::set_time_master (bool tm)
{
  this->time_master = tm;
}
