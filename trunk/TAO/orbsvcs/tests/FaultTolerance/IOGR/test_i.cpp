// $Id$

#include "test_i.h"

#if !defined(__ACE_INLINE__)
//#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(MT_Client, test_i, "$Id$")

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Simple_Server_i::Simple_Server_i (void)
  : orb_ (0)
{
  // no-op
}

void
Simple_Server_i::remote_call (CORBA::Environment & /*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Print out from process id (%P) hosting the servant \n")));

  return;
}


void
Simple_Server_i::shutdown (CORBA::Environment&)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}
