// $Id$

#include "test_i.h"

ACE_RCSID(MProfile_Connection_Timeout,
          test_i,
          "$Id$")

// part of the famous caesium 133 atom transition for a
// second. Cannot represent the last part since the value gets out of
// range.
const CORBA::ULong freq = 919263;

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    val_ (1)
{
}

Simple_Server_i::Simple_Server_i (void)
  : orb_ (0)
{
  // no-op
}

CORBA::ULongLong
Simple_Server_i::remote_call (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Print out from process id (%P) hosting the servant \n"));

  return freq;
}


void
Simple_Server_i::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}
