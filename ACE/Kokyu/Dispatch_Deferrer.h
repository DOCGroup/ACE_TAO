/* -*- C++ -*- */
/**
 *  @file   Dispatch_Deferrer.h
 *
 *  $Id$
 *
 *  @author Bryan Thrall (thrall@cse.wustl.edu)
 *
 */

#ifndef DISPATCH_DEFERRER_H
#define DISPATCH_DEFERRER_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "kokyu_export.h"
#include "Kokyu_defs.h"
#include "ace/Event_Handler.h"
#include "ace/Thread_Mutex.h"
#include "ace/Message_Block.h"
#include "ace/Message_Queue.h"
#include "ace/Reactor.h"
#include "ace/Map.h"

namespace Kokyu
{

class Dispatch_Task; //forward decl
class Dispatch_Queue_Item; //forward decl

/**
 * @class Dispatch_Deferrer
 *
 * @brief Part of the Release Guard protocol. When a Dispatch_Command
 * needs to be dispatched later rather than when dispatch_i() is
 * called on the Default_Dispatcher_Impl, it is passed to this
 * object. When the appropriate time to dispatch the Dispatch_Command
 * comes (last release time + period), this object calls enqueue() on
 * the Dispatcher_Task.
 */
class Dispatch_Deferrer : public ACE_Event_Handler
{
 public:
  Dispatch_Deferrer();
  //Default constructor

  ~Dispatch_Deferrer();
  //Destructor

  int init(const Dispatch_Deferrer_Attributes& attr);

  int dispatch (Dispatch_Queue_Item *qitem);

  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act = 0);
  //TODO: what if need higher resolution timers?

 private:
  ACE_Deadline_Message_Strategy msg_strat_;

  ///Stores the Dispatch_Commands in earliest-release-time order,
  ///until they are dispatched. I decided to use an
  ///ACE_Dynamic_Message_Queue because it supports deadline
  ///ordering. This decision is also good because we can simply store
  ///the Dispatch_Queue_Item given to us by the
  ///Default_Dispatcher_Impl rather than allocate some structure to
  ///hold the Dispatch_Command and QoSDescriptor.
  ACE_Dynamic_Message_Queue<ACE_SYNCH> rgq_;

  //Stores timer_ids from the Reactor (longs) using the
  //Dispatch_Queue_Item the timer is for as the key. Used to
  //cancel timers if they expire and are enqueued before the
  //callback happens.
  typedef ACE_Map_Manager<Dispatch_Queue_Item*,long,ACE_Thread_Mutex> Timer_Map;

  Timer_Map timers_;

  ///Manages timers for the Dispatch_Commands
  ACE_Reactor react_;

  Dispatcher_Task* task_;
};

} //namespace Kokyu

#if defined (__ACE_INLINE__)
#include "Dispatch_Deferrer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif //DISPATCH_DEFERRER_H
