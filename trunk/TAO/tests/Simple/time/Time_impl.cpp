// $Id$

#include "Time_impl.h"
#include "tao/corba.h"

ACE_RCSID(Time, Time_impl, "$Id$")

// Constructor

Time_Impl::Time_Impl (const char *)
{
}

// Destructor

Time_Impl::~Time_Impl (void)
{
}

// Return the current date/time on the server.

CORBA::Long
Time_Impl::time (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  return long (ACE_OS::time (0));
}

// Shutdown.

void Time_Impl::shutdown (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "Time_Impl is shutting down"));
  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}

