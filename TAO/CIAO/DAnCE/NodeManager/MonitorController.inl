// $Id$

#ifndef MONITOR_CONTROLLER_INL
#define MONITOR_CONTROLLER_INL

ACE_INLINE
void MonitorController::terminate ()
{
  // make the terminate flag false
  ACE_GUARD (ACE_SYNCH_MUTEX,
             guard,
             lock_
             );
  ACE_DEBUG ((LM_DEBUG , "WITHIN TERMINATE CALL  ......"));
  terminate_flag_=1;
}

ACE_INLINE
bool MonitorController::terminating ()
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    guard,
                    lock_,
                    0
                    );
  return terminate_flag_;
}

#endif
