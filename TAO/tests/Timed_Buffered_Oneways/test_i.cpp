// $Id$

#include "test_i.h"

ACE_RCSID(Timed_Buffered_Oneways, test_i, "$Id$")

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
test_i::method (CORBA::ULong request_number,
                const test::data &,
                CORBA::ULong work
                TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "server: Iteration %d @ %T\n",
              request_number));

  // Time required to process this request. <work> is time units in
  // milli seconds.
  ACE_Time_Value work_time (0,
                            work * 1000);

  ACE_OS::sleep (work_time);
}

void
test_i::shutdown (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0
                        TAO_ENV_ARG_PARAMETER);
}
