// $Id$

#include "test_i.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(MT_Server, test_i, "test_i.cpp,v 1.6 2002/01/29 20:21:08 okellogg Exp")

CORBA::Long
Simple_Server_i::test_method (CORBA::Long x ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Request in thread %t\n"));
  ACE_Time_Value tv (0, (x % 1000));
  ACE_OS::sleep (tv);
  return x;
}

void
Simple_Server_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
