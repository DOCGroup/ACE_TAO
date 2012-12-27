// $Id$

#include "test_i.h"
#include "tao/debug.h"
#include "ace/ACE.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

void
Test_i::test_method (CORBA::Long id)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Receiving request from thread <%d> in <%t>\n",
                id));
}

void
Test_i::shutdown (const char *orb_id)
{
  int argc = 0;
  ACE_TCHAR **argv = 0;
  CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        argv,
                                        orb_id);

  orb->shutdown (0);
}
