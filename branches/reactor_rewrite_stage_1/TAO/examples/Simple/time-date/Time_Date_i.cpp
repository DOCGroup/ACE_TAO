// -*- C++ -*-
// $Id$

#include "Time_Date_i.h"
#include "tao/corba.h"

// Default constructor.

Time_Date_i::Time_Date_i (void)
{
  //no-op
}

// Default destructor.

Time_Date_i::~Time_Date_i (void)
{
  // no-op.
}

// Obtain the time and date in binary format.

void
Time_Date_i::bin_date (CORBA::Long_out time_date
                       TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  time_date = ACE_OS::time (0);
}

// Obtain the time and date in string format.

void
Time_Date_i::str_date (CORBA::String_out time_date
                       TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  const time_t time = ACE_OS::time (0);

  time_date = CORBA::string_dup (ACE_OS::ctime (&time));
}

void
Time_Date_i::orb (CORBA::ORB_ptr orb_ptr)
{
   orb_var_ = CORBA::ORB::_duplicate (orb_ptr);
}
