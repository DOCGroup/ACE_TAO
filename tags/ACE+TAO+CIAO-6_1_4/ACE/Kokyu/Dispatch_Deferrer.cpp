// $Id$

#include "ace/Sched_Params.h"
#include "Dispatch_Deferrer.h"
#include "Dispatcher_Task.h"

#if ! defined (__ACE_INLINE__)
#include "Dispatch_Deferrer.inl"
#endif /* __ACE_INLINE__ */

#include "kokyu_config.h"
#include "kokyu_dsui_families.h"
#include <dsui.h>

namespace Kokyu
{

int
Dispatch_Deferrer::init(const Dispatch_Deferrer_Attributes& attr)
{
  //set up reactor for timeouts
  this->react_.open(0);
  //Don't need any handles, since we're only using it for timeouts

  this->timers_.open();

  this->task_ = attr.task_;

  return 0;
}

int
Dispatch_Deferrer::dispatch (Dispatch_Queue_Item *qitem)
{
  ACE_ASSERT(qitem != 0);

  //setup timout
  //For now, assume period = deadline
  ACE_Time_Value tv;
  tv = ACE_OS::gettimeofday() + qitem->qos_info().deadline_;
  long timer_id = this->react_.schedule_timer(this,
                                              0, //NULL arg
                                              tv);
  if (timer_id == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                  ACE_TEXT ("EC (%P|%t) cannot schedule Release Guard timer.")
                  ACE_TEXT ("ACE_Reactor.schedule_timer() returned -1\n")),
                  -1);
    }
  //else valid timer_id
  this->timers_.bind(qitem,timer_id);

  //@BT INSTRUMENT with event ID: EVENT_DEFERRED_ENQUEUE Measure time
  //between release and enqueue into dispatch queue because of RG
  DSUI_EVENT_LOG(DISP_DEFERRER_FAM, EVENT_DEFERRED_ENQUEUE, timer_id, 0, 0);

  //buffer until timer expires
  return this->rgq_.enqueue_deadline(qitem,&tv);
}


int
Dispatch_Deferrer::handle_timeout (const ACE_Time_Value &,
                                   const void *)
{
  //get all expired Dispatch_Queue_Items
  ACE_Message_Block *begin,*end;
  this->rgq_.remove_messages(begin,end,
                             (u_int) (ACE_Dynamic_Message_Strategy::LATE |
                                      ACE_Dynamic_Message_Strategy::BEYOND_LATE));

  //dispatch them back to Dispatcher_Impl
  while (begin <= end)
    {
      Dispatch_Queue_Item *qitem =
        dynamic_cast<Dispatch_Queue_Item*> (begin);

      if (qitem == 0)
        {
          ACE_Message_Block::release (begin);
          continue;
        }


      //remove timer for each enqueued qitem from reactor
      long timer_id;
      if (this->timers_.find(qitem,timer_id) < 0)
        {
            ACE_ERROR_RETURN ((LM_ERROR,
             ACE_TEXT ("Could not cancel Release Guard timer.")
             ACE_TEXT ("Unknown timer ID\n")),
             -1);
        }
      //else got timer_id
      this->react_.cancel_timer(timer_id);

      //@BT INSTRUMENT with event ID: EVENT_DEFERRED_DEQUEUE Measure
      //time between release and enqueue into dispatch queue because
      //of RG
      DSUI_EVENT_LOG (DISP_DEFERRER_FAM, EVENT_DEFERRED_DEQUEUE, timer_id, 0, 0);

      this->task_->enqueue(qitem);

      ++begin;
    }

  return 0;
}

} //namespace Kokyu
