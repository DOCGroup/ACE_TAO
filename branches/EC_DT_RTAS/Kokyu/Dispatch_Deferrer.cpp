// $Id$

#include "Dispatch_Deferrer.h"

#include "Dispatcher_Task.h"
#include "ace/Sched_Params.h"
#include "ace/Reactor.h"

#if defined (ACE_HAS_DSUI)
#include "kokyu_config.h"
#include "kokyu_dsui_families.h"
#include <dsui.h>
#endif // ACE_HAS_DSUI

#if ! defined (__ACE_INLINE__)
#include "Dispatch_Deferrer.i"
#endif /* __ACE_INLINE__ */

#if defined (KOKYU_HAS_RELEASE_GUARD)

ACE_RCSID(Kokyu, Dispatch_Deferrer, "$Id$")

namespace {

  ACE_INLINE
  void print_time_value(ACE_Time_Value tv)
  {
    //For printing, assume a long is equivalent to an ACE_UINT64
    ACE_DEBUG((LM_DEBUG,"%isec %iusec",tv.sec(),tv.usec()));
  }

} //anonymous namespace

namespace Kokyu
{

Dispatch_Deferrer::Dispatch_Deferrer (void)
  : msg_strat_()
  , timers_()
  , react_()
  , task_(0)
{
}

Dispatch_Deferrer::~Dispatch_Deferrer (void)
{
  //remove all timers and delete all qitems
  Timer_Map::iterator iter = this->timers_.begin();
  for(; iter != this->timers_.end(); ++iter)
    {
      Timer_Map::ENTRY entry = *iter;
      ACE_DEBUG((LM_DEBUG,"Dispatch_Deferrer destructor cancelling timer with id %d\n",entry.int_id_));
      this->react_->cancel_timer(entry.int_id_);
      Dispatch_Queue_Item *qitem = entry.ext_id_;
      ACE_Message_Block::release (qitem);
    }
  this->timers_.unbind_all();

  //Don't close reactor since it may still be in use
  //and we don't own it!

  this->timers_.close();
}

int
Dispatch_Deferrer::init(const Dispatch_Deferrer_Attributes& attr)
{
  //We assume that the singleton Reactor
  //has its own thread, so we don't have to run the Reactor's
  //event loop ourselves!
  //this->react_ = ACE_Reactor::instance();
  this->react_ = Singleton_Reactor::instance();

  this->timers_.open();

  this->task_ = attr.task_;

  return 0;
}

int
Dispatch_Deferrer::dispatch (Dispatch_Queue_Item *qitem, ACE_Time_Value last_release)
{
  ACE_ASSERT(qitem != 0);

  int ret = 0; //used to check return values

  //setup timout
  //For now, assume period = deadline
  //Note that qitem deadline is absolute!
  ACE_DEBUG((LM_DEBUG, "Dispatch_Deferrer::dispatch() (%t) at %D: now is "));
  print_time_value(ACE_OS::gettimeofday());
  ACE_DEBUG((LM_DEBUG, "; deadline is "));
  print_time_value(qitem->qos_info().deadline_);
  ACE_DEBUG((LM_DEBUG, "; period is "));
  print_time_value(qitem->qos_info().period_);
  ACE_DEBUG((LM_DEBUG, "\n"));

  ACE_Time_Value next_release = last_release + qitem->qos_info().period_;
  ACE_DEBUG((LM_DEBUG, "Dispatch_Deferrer::dispatch() (%t) : delaying deferred event until "));
  print_time_value(last_release);
  ACE_DEBUG((LM_DEBUG, "\n"));
  //END DEBUG!

  long timer_id = this->react_->schedule_timer(this,
                                               qitem, //pass qitem as arg
                                               next_release //delay
                                               );
  if (timer_id < 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("EC (%P|%t) cannot schedule Release Guard timer.")
                         ACE_TEXT ("ACE_Reactor.schedule_timer() returned -1\n")),
                        -1);
    }
  //else valid timer_id
  ret = this->timers_.rebind(qitem,timer_id);
  if (ret < 0)
    {
      ACE_DEBUG((LM_DEBUG,"Dispatch_Deferrer::dispatch() (%P|%t) cannot store timer ID for later cancellation\n"));
    }

  ACE_DEBUG ((LM_DEBUG, "Dispatch_Deferrer::dispatch() (%t) : event deferred enqueue at %u\n",ACE_OS::gettimeofday().msec()));

#if defined (ACE_HAS_DSUI)
  //@BT INSTRUMENT with event ID: EVENT_DEFERRED_ENQUEUE Measure time
  //between release and enqueue into dispatch queue because of RG
  Object_ID oid = qitem->command()->getID();
  DSTRM_EVENT(DISP_DEFERRER_FAM, EVENT_DEFERRED_ENQUEUE, 0, sizeof(Object_ID), (char*)&oid);
#endif // ACE_HAS_DSUI

  return 0;
} //dispatch()

int
Dispatch_Deferrer::handle_timeout (const ACE_Time_Value &,
                                   const void *const_arg)
{
  ACE_DEBUG ((LM_DEBUG, "Dispatch_Deferrer::handle_timeout() (%t) : ENTER\n"));

  //Need to get rid of const-ness, should be OK since qitem not stored anywhere else
  void *arg = ACE_const_cast(void*,const_arg);
  Dispatch_Queue_Item *qitem = ACE_reinterpret_cast(Dispatch_Queue_Item*,arg);

  if (qitem == 0)
    {
      ACE_Message_Block::release (qitem);
      ACE_DEBUG((LM_DEBUG,"Dispatch_Deferrer (%P|%t) handle_timeout() given null arg\n"));
      return 0;
    }

  //remove timer for each enqueued qitem from reactor
  long timer_id;
  if (this->timers_.find(qitem,timer_id) < 0)
    {
      ACE_DEBUG((LM_DEBUG,"Cannot cancel Release Guard timer: Unknown qitem %@\n",
                 qitem));
    }
  //else got timer_id
  this->react_->cancel_timer(timer_id);
  this->timers_.unbind(qitem);

  ACE_DEBUG ((LM_DEBUG, "Dispatch_Deferrer::handle_timeout() (%t) : event deferred dequeue at %u\n",ACE_OS::gettimeofday().msec()));

#if defined (ACE_HAS_DSUI)
  //@BT INSTRUMENT with event ID: EVENT_DEFERRED_DEQUEUE Measure
  //time between release and enqueue into dispatch queue because
  //of RG
  Object_ID oid = qitem->command()->getID();
  DSTRM_EVENT (DISP_DEFERRER_FAM, EVENT_DEFERRED_DEQUEUE, 0, sizeof(Object_ID), (char*)&oid);
#endif // ACE_HAS_DSUI

  this->task_->enqueue_i(qitem);

  ACE_DEBUG ((LM_DEBUG, "Dispatch_Deferrer::handle_timeout() (%t) : LEAVE\n"));

  return 0;
} //handle_timeout()

} //namespace Kokyu

#endif //defined KOKYU_HAS_RELEASE_GUARD
