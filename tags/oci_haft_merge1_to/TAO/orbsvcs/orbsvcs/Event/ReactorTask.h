// $Id$

#if !defined ACE_ReactorTask_H
#define ACE_ReactorTask_H

// BBM, moved this here from UPSingleProcessorOrb.h
//## begin module.includes preserve=yes
#include "Fast_Reactor.h"
#if defined (ACE_OLD_STYLE_REACTOR)
# define ACE_ORB_REACTOR ACE_ES_Fast_Reactor
#endif /* ACE_OLD_STYLE_REACTOR */
//## end module.includes

// Added these.
#include "ace/Timer_Heap.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Timer_List.h"
#include "ace/Reactor.h"
#include "Local_ESTypes.h"
#include "RT_Task.h"

class TAO_RTOLDEvent_Export ACE_ES_Reactor_Task : public ACE_RT_Task
// = TITLE
//    Event Service Timer Task
//
// = DESCRIPTION
//    An active object that dispatches timers from its own ReactorEx.
{
public:
  // BBM, added this.
#if defined (ACE_OLD_STYLE_REACTOR)
  typedef ACE_ORB_REACTOR Reactor;
#else
  typedef ACE_Reactor Reactor;
#endif /* ACE_OLD_STYLE_REACTOR */

  ACE_ES_Reactor_Task (RtecScheduler::Scheduler_ptr scheduler);
  // Default construction.

  ~ACE_ES_Reactor_Task (void);
  // Destruction.

  virtual int svc_hook(RtecScheduler::OS_Priority);
  // Assume ownership of the reactor_.

  int open_reactor (RtecScheduler::Period_t &period);
  // This is a hack for now.

  virtual int svc_one();
  // Calls reactor_.handle_events until done_ is set.

  void shutdown_task();
  // Sets done_ and notifies the reactor_.

  Reactor &get_reactor();
  // ReactorEx accessor.

  virtual void threads_closed();
  // Deletes this.

private:
  ACE_Timer_List timer_queue_;
  // The timer storage mechanism used by reactor_.

#ifndef ACE_OLD_STYLE_REACTOR
  ACE_ES_Fast_Reactor fast_reactor_;
  // The timer dispatch mechanism.
#endif /* ! ACE_OLD_STYLE_REACTOR */

  Reactor reactor_;
  // "Public" handle to fast_reactor_.

  sig_atomic_t done_;
  // When set, end the event loop.
};

#endif /* ACE_ReactorTask_H */
