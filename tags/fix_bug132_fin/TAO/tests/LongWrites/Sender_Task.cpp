//
// $Id$
//

#include "Sender_Task.h"
#include "Sender.h"

ACE_RCSID(LongWrites, Sender_Task, "$Id$")

Sender_Task::Sender_Task (Sender *sender,
                          CORBA::Long event_count,
                          CORBA::ULong event_size,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , sender_ (sender)
  , event_count_ (event_count)
  , event_size_ (event_size)
{
}

int
Sender_Task::svc (void)
{
  return this->sender_->run_test (this->event_count_,
                                  this->event_size_);
}
