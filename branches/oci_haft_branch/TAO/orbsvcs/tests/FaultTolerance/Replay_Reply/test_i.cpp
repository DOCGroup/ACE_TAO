// -*- C++ -*-
#include "test_i.h"

ACE_RCSID (ForwardRequest,
           test_i,
           "$Id$")

test_i::test_i (CORBA::ORB_ptr orb)
  : number_ (0)
  , orb_ (CORBA::ORB::_duplicate (orb))
{
}

test_i::~test_i (void)
{
}

CORBA::Short
test_i::number (char *&str
                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::string_free (str);

  str = CORBA::string_dup ("Testing_Replay_Reply");

  return ++this->number_;
}

void
test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Server is shutting down via object %d.\n",
              this->number_));

  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
