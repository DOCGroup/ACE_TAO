/**
 * @file Send_Task_Stopper.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Send_Task_Stopper.h"

#if !defined(__ACE_INLINE__)
#include "Send_Task_Stopper.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO_PERF_RTEC, Send_Task_Stopper, "$Id$")

Send_Task_Stopper::~Send_Task_Stopper (void)
{
  if (this->task_ == 0)
    return;
  this->task_->stop ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Task_Activator<Send_Task>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Task_Activator<Send_Task>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
