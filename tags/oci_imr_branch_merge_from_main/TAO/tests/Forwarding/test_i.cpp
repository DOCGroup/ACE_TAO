// $Id$

#include "test_i.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Forwarding, test_i, "$Id$")

CORBA::Boolean
Simple_Server_i::test_is_a (const char * /* type */
                            ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) test_is_a %s\n", type));
  return 0;
}

void
Simple_Server_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
