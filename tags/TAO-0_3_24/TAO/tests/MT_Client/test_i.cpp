// $Id$

#include "test_i.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(MT_Client, test_i, "$Id$")

void
Simple_Server_i::test_method (CORBA::Environment&)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "%N:%l:(%P:%t):Simple_Server_i::test_method called\n"));
}

CORBA::Long
Simple_Server_i::get_number (CORBA::Environment&)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "%N:%l:(%P:%t):Simple_Server_i::get_number called\n"));
  return (CORBA::ULong) 931232;
}

void
Simple_Server_i::shutdown (CORBA::Environment&)
{
  this->orb_->shutdown (0);
}
