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

void
Time_Date_i::bin_date (CORBA::Long_out time_date,
                       CORBA::Environment &ACE_TRY_ENV)
{
  time_date = CORBA::Long (ACE_OS::time (0));
}

void
Time_Date_i::str_date (CORBA::String_out time_date,
                       CORBA::Environment &ACE_TRY_ENV)
{

}

void
Time_Date_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

void
Time_Date_i::shutdown (CORBA::Environment & )
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "Time_i is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}
