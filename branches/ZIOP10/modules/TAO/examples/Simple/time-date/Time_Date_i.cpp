// -*- C++ -*-
// $Id$

#include "Time_Date_i.h"
#include "ace/OS_NS_time.h"

// Obtain the time and date in binary format.

void
Time_Date_i::bin_date (CORBA::Long_out time_date)
{
  time_date = static_cast<CORBA::Long> (ACE_OS::time (0));
}

// Obtain the time and date in string format.

void
Time_Date_i::str_date (CORBA::String_out time_date)
{
  const time_t time = ACE_OS::time (0);

  time_date = CORBA::string_dup (ACE_OS::ctime (&time));
}

void
Time_Date_i::orb (CORBA::ORB_ptr orb_ptr)
{
   orb_var_ = CORBA::ORB::_duplicate (orb_ptr);
}
