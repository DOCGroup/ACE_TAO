// -*- C++ -*-
// $Id$

#include "Time_Date_i.h"
#include "ace/OS_NS_time.h"
#include "ace/Reactor.h"

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

  time_date = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(ACE_OS::ctime (&time)));
}

// Shutdown.
void
Time_Date_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Time_Date_i is shutting down\n")));

  // Instruct the ORB to shutdown.
  this->orb_var_->shutdown ();
  // And shutdown the event loop from started in main.
  ACE_Reactor::end_event_loop ();
}

void
Time_Date_i::orb (CORBA::ORB_ptr orb_ptr)
{
  orb_var_ = CORBA::ORB::_duplicate (orb_ptr);
}
