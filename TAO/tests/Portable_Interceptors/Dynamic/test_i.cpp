// $Id$

#include "test_i.h"

ACE_RCSID(Dynamic, test_i, "$Id$")

Visual_i::Visual_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}
  // ctor

void
Visual_i::normal (CORBA::Long arg,
                  CORBA::Environment&)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Visual::normal called with %d\n", arg));
}

CORBA::Long
Visual_i::calculate (CORBA::Long one,
                     CORBA::Long two,
                     CORBA::Environment&)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Visual::calculate\n"));
  return (one + two);
}

void
Visual_i::user (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,Test_Interceptors::Silly))
{
  ACE_DEBUG ((LM_DEBUG, "Visual::user, throwning Silly\n"));
  ACE_THROW (Test_Interceptors::Silly ());
}

void
Visual_i::system (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Visual::user, throwing INV_OBJREF\n"));
  ACE_THROW (CORBA::INV_OBJREF ());
}

void
Visual_i::shutdown (CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown ();
}
