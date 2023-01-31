/**
 * @file Send_Task_Stopper.cpp
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Send_Task_Stopper.h"

#if !defined(__ACE_INLINE__)
#include "Send_Task_Stopper.inl"
#endif /* __ACE_INLINE__ */

Send_Task_Stopper::~Send_Task_Stopper ()
{
  if (this->task_ == 0)
    return;
  this->task_->stop ();
}
