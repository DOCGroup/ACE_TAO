// $Id$

#include "simple_test_i.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "simple_test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tests, simple_test_i, "$Id$")

CORBA::Boolean
Simple_Server_i::print_status (CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "%N:%l:(%P:%t):Simple_Server_i::print_status called\n"));
  return 0;
}

void
Simple_Server_i::shutdown (CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}
