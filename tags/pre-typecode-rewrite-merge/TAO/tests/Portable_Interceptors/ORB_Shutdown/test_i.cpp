#include "test_i.h"


ACE_RCSID (ORB_Shutdown,
           test_i,
           "$Id$")


test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

test_i::~test_i (void)
{
}

void
test_i::hello (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Hello!\n"));
}

void
test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Shutting down ORB.\n"));

  this->orb_->shutdown (0
                        ACE_ENV_ARG_PARAMETER);
}
