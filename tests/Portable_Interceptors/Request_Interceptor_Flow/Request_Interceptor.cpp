// -*- C++ -*-
// $Id$

#include "Request_Interceptor.h"

Request_Interceptor::Request_Interceptor (const char *name)
  : name_ (name),
    starting_interception_point_count_ (0),
    ending_interception_point_count_ (0)
{
}

char *
Request_Interceptor::name ()
{
  return CORBA::string_dup (this->name_.in ());
}

void
Request_Interceptor::destroy (void)
{

  CORBA::ULong s_count =
    this->starting_interception_point_count_.value ();

  CORBA::ULong e_count =
    this->ending_interception_point_count_.value ();

  if (s_count == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P) ERROR: Interceptor %d \"%C\"\n"
                  "(%P) The number of starting interception point calls "
                  "is zero\n", s_count,
                  this->name_.in ()));

      throw CORBA::INTERNAL ();
    }

  if (e_count == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P) ERROR: Interceptor %d \"%C\"\n"
                  "(%P) The number of ending interception point calls"
                  "is zero\n",
                  e_count, this->name_.in ()));

      throw CORBA::INTERNAL ();
    }

  if (s_count != e_count)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P) ERROR: Interceptor \"%C\"\n"
                  "(%P) The number of starting interception point "
                  "calls (%u)\n"
                  "(%P) did not equal the number of ending "
                  "interception\n"
                  "(%P) point calls (%u).\n\n",
                  this->name_.in (),
                  s_count,
                  e_count));

      throw CORBA::INTERNAL ();
    }
}
