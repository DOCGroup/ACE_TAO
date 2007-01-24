/**
 * @file ORB_Task.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "ORB_Task.h"

#if !defined(__ACE_INLINE__)
#include "ORB_Task.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (TAO_PERF_RTEC,
           ORB_Task,
           "$Id$")

ORB_Task::ORB_Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
ORB_Task::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_->run ();
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}
