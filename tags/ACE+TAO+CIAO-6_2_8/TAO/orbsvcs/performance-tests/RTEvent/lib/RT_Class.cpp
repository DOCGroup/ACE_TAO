/**
 * @file RT_Class.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "RT_Class.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_errno.h"

#if !defined(__ACE_INLINE__)
#include "RT_Class.inl"
#endif /* __ACE_INLINE__ */

RT_Class::RT_Class (void)
  : prc_sched_class_ (ACE_SCHED_FIFO)
  , thr_sched_class_ (THR_SCHED_FIFO)
{
  int priority =
    (ACE_Sched_Params::priority_min (this->prc_sched_class_)
     + ACE_Sched_Params::priority_max (this->prc_sched_class_)) / 2;

  if (ACE_OS::sched_params (ACE_Sched_Params (this->prc_sched_class_,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "WARNING (%P|%t) user does not have "
                      "permissions to run real-time tests.\n"
                      "The test will run in the time-shared "
                      "class, it may fail or produce unpredictable "
                      "results\n"));
          this->prc_sched_class_ = ACE_SCHED_OTHER;
          this->thr_sched_class_ = THR_SCHED_DEFAULT;
        }
    }
  this->priority_low_ =
    ACE_Sched_Params::priority_min (this->prc_sched_class_);
  this->priority_high_ =
    ACE_Sched_Params::priority_max (this->prc_sched_class_);
  this->priority_process_ =
    (this->priority_low_ + this->priority_high_) / 2;
}
