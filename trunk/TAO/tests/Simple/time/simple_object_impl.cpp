// $Id$

#include "simple_object_impl.h"
#include "tao/corba.h"

ACE_RCSID(Simple, simple_object_impl, "$Id$")

// Constructor

Simple_Impl::Simple_Impl (const char *)
{
}

// Destructor

Simple_Impl::~Simple_Impl (void)
{
}

// Return the current date/time on the server.

CORBA::Long
Simple_Impl::time (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  return long (ACE_OS::time (0));
}

// Shutdown.

void Simple_Impl::shutdown (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "Simple_Impl is shutting down"));
  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}

