// $Id$

#include "simple_object_impl.h"
#include "tao/corba.h"

ACE_RCSID(ImplRepo, simple_object_impl, "$Id$")

// Constructor

Simple_i::Simple_i (const char *)
: use_ir_ (1)
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
  TAO_ORB_Core_instance ()->orb ()->shutdown ();
  return l * l * l;
}

// Shutdown.

void Simple_i::shutdown (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  ACE_DEBUG ((LM_DEBUG, "%s\n", "Simple_i is shutting down"));

//  if (this->use_ir_ == 1)
 //   TAO_ORB_Core_instance ()->orb ()->shutdown ();
}

void Ping_i::ping (CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, "Pinged!\n"));

  ACE_UNUSED_ARG (env);
  // Does nothing, just returns.
}
