// $Id$

#include "simple_object_impl.h"
#include "tao/corba.h"

ACE_RCSID(ImplRepo, simple_object_impl, "$Id$")

// Constructor

Simple_i::Simple_i (const char *)
{
}


// Destructor

Simple_i::~Simple_i (void)
{
}

// Cube a long

CORBA::Long
Simple_i::simple_method (CORBA::Long l, CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, "Simple Method\n"));
  ACE_UNUSED_ARG (env);
  return l * l * l;
}

// Shutdown.

void Simple_i::shutdown (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  ACE_DEBUG ((LM_DEBUG, "%s\n", "Simple_i is shutting down"));

  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}

