//
// $Id$
//

#include "Sender_Task.h"
#include "Sender.h"

ACE_RCSID(LongWrites, Sender_Task, "$Id$")

Sender_Task::Sender_Task (Sender *sender)
  : sender_ (sender)
  , event_count_ (0)
  , event_size_ (0)
{
}

int
Sender_Task::run_test (int thread_count,
                       CORBA::Long event_count,
                       CORBA::ULong event_size)
{
  this->event_count_ = event_count;
  this->event_size_ = event_size;

  return this->activate (THR_NEW_LWP | THR_JOINABLE, thread_count, 1);
}

int
Sender_Task::svc (void)
{
  return this->sender_->run_test (this->event_count_,
                                  this->event_size_);
}
