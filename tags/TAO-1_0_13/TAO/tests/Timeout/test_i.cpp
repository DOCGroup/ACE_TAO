// $Id$

#include "test_i.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Timeout, test_i, "$Id$")

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{

}

CORBA::Long
Simple_Server_i::echo (CORBA::Long x,
                       CORBA::Long msecs,
                       CORBA::Environment&)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Time_Value tv (msecs / 1000, (msecs % 1000) * 1000);

  ACE_DEBUG ((LM_DEBUG, "server (%P) Sleeping for %d msecs\n",
              tv.msec ()));
  ACE_OS::sleep (tv);

  return x;
}

void
Simple_Server_i::shutdown (CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Received shutdown request from client\n"));
  this->orb_->shutdown (0, ACE_TRY_ENV);
}
