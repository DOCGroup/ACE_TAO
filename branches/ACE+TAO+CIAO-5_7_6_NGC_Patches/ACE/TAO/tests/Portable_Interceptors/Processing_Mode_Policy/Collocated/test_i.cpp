// $Id$

#include "test_i.h"
#include "ace/OS_NS_unistd.h"

ACE_RCSID(Dynamic, test_i, "$Id$")

Visual_i::Visual_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}
  // ctor

void
Visual_i::normal (CORBA::Long)
{
  // ACE_DEBUG ((LM_DEBUG, "Visual::normal called with %d\n", arg));
}

CORBA::Long
Visual_i::calculate (CORBA::Long one,
                     CORBA::Long two)
{
  // ACE_DEBUG ((LM_DEBUG, "Visual::calculate\n"));
  return (one + two);
}

void
Visual_i::user (void)
{
  // ACE_DEBUG ((LM_DEBUG, "Visual::user, throwning Silly\n"));
  throw Test_Interceptors::Silly ();
}

void
Visual_i::system (void)
{
  // ACE_DEBUG ((LM_DEBUG, "Visual::user, throwing INV_OBJREF\n"));
  throw CORBA::INV_OBJREF ();
}

void
Visual_i::shutdown (void)
{
  this->_remove_ref ();

  // Give the client thread time to return from the collocated
  // call to this method before shutting down the ORB.  We sleep
  // to avoid BAD_INV_ORDER exceptions on fast dual processor machines.
  ACE_OS::sleep(1);
  this->orb_->shutdown ();
}
