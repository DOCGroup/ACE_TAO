// $Id$

#include "test_i.h"

ACE_RCSID (Service_Context_Manipulation,
           test_i,
           "$Id$")


Visual_i::Visual_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Visual_i::normal (CORBA::Long arg
                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Visual::normal called with %d\n", arg));
}

void
Visual_i::nothing (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Visual::nothing\n"));
}

void
Visual_i::user (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Test_Interceptors::Silly))
{
  ACE_DEBUG ((LM_DEBUG, "Visual::user, throwing Silly\n"));
  ACE_THROW (Test_Interceptors::Silly ());
}

void
Visual_i::system (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Visual::user, throwing INV_OBJREF\n"));
  ACE_THROW (CORBA::INV_OBJREF ());
}

void
Visual_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
