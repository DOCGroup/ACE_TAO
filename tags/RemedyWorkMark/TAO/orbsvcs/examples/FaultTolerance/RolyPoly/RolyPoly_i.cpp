// file      : RolyPoly/RolyPoly_i.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "RolyPoly_i.h"
#include "StateUpdate.h"
#include "tao/AnyTypeCode/Any.h"

RolyPoly_i::RolyPoly_i (CORBA::ORB_ptr orb)
  : number_ (0)
  , orb_ (CORBA::ORB::_duplicate (orb))
{
}

RolyPoly_i::~RolyPoly_i (void)
{
}

CORBA::Any* RolyPoly_i::
get_state ()
{
  /*
  CORBA::Any_var state (new CORBA::Any);

  *state <<= this->number_;

  return state._retn ();
  */

  return 0;
}


void RolyPoly_i::
set_state (CORBA::Any const& state)
{
  state >>= this->number_;
}


CORBA::Short
RolyPoly_i::number (char *&str)
{
  CORBA::string_free (str);

  str = CORBA::string_dup ("Greetings from RolyPoly.");

  ++this->number_;


  // Preppare state update.
  //

  CORBA::Any a;

  a <<= this->number_;

  associate_state (orb_.in (), a);

  return this->number_;
}

void
RolyPoly_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "Server is shutting down.\n"));

  this->orb_->shutdown (0);
}
