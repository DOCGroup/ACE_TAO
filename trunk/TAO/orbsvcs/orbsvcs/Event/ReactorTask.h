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

/**
 * @class ACE_ES_Reactor_Task
 *
 * @brief Event Service Timer Task
 *
 * An active object that dispatches timers from its own ReactorEx.
 */
{
class TAO_RTOLDEvent_Export ACE_ES_Reactor_Task : public ACE_RT_Task
public:
  // BBM, added this.
#if defined (ACE_OLD_STYLE_REACTOR)
  typedef ACE_ORB_REACTOR Reactor;
#else
  typedef ACE_Reactor Reactor;
#endif /* ACE_OLD_STYLE_REACTOR */

  /// Default construction.
  ACE_ES_Reactor_Task (RtecScheduler::Scheduler_ptr scheduler);

  /// Destruction.
  ~ACE_ES_Reactor_Task (void);

  /// Assume ownership of the reactor_.
  virtual int svc_hook(RtecScheduler::OS_Priority);

  /// This is a hack for now.
  int open_reactor (RtecScheduler::Period_t &period);

  /// Calls reactor_.handle_events until done_ is set.
  virtual int svc_one();

  /// Sets done_ and notifies the reactor_.
  void shutdown_task();

  /// ReactorEx accessor.
  Reactor &get_reactor();

  /// Deletes this.
  virtual void threads_closed();

private:
  /// The timer storage mechanism used by reactor_.
  ACE_Timer_List timer_queue_;

#ifndef ACE_OLD_STYLE_REACTOR
  /// The timer dispatch mechanism.
  ACE_ES_Fast_Reactor fast_reactor_;
#endif /* ! ACE_OLD_STYLE_REACTOR */

  /// "Public" handle to fast_reactor_.
  Reactor reactor_;

  /// When set, end the event loop.
  sig_atomic_t done_;
};

#endif /* ACE_ReactorTask_H */
