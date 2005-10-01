// $Id$

#include "test_i.h"
#include "tao/debug.h"
#include "ace/ACE.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Latency, test_i, "$Id$")

void
Test_i::test_method (CORBA::Long id
                     ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Receiving request from thread <%d> in <%t>\n",
                id));
}

void
Test_i::shutdown (const char *orb_id
                  ACE_ENV_ARG_DECL_NOT_USED ACE_ENV_SINGLE_ARG_PARAMETER)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int argc = 0;
  CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        0,
                                        orb_id
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  orb->shutdown (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
