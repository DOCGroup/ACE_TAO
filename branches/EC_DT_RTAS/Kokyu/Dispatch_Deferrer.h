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

#if defined KOKYU_HAS_RELEASE_GUARD

#include "ace/Event_Handler.h"
#include "ace/Thread_Mutex.h"
#include "ace/Synch_T.h"
#include "ace/Message_Block.h"
#include "ace/Reactor.h"
#include "ace/Map.h"
#include "ace/Singleton.h"

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
class Kokyu_Export Dispatch_Deferrer : public ACE_Event_Handler
{
 public:

  typedef ACE_Singleton<ACE_Reactor,ACE_Recursive_Thread_Mutex> Singleton_Reactor;

  Dispatch_Deferrer(void);
  //Default constructor

  virtual ~Dispatch_Deferrer(void);
  //Destructor

  int init(const Dispatch_Deferrer_Attributes& attr);

  int dispatch (Dispatch_Queue_Item *qitem, ACE_Time_Value last_release);

  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act = 0);
  //TODO: what if need higher resolution timers?

 private:
  ACE_Deadline_Message_Strategy msg_strat_;

  //Stores timer_ids from the Reactor (longs) using the
  //Dispatch_Queue_Item the timer is for as the key. Used to
  //cancel timers if they expire and are enqueued before the
  //callback happens.
  //Can't use ACE_Hash_Map_Manager because pointers and longs don't have hash!
  typedef ACE_Map_Manager<Dispatch_Queue_Item*,long,ACE_Thread_Mutex> Timer_Map;

  Timer_Map timers_;

  ///Manages timers for the Dispatch_Commands
  //TODO How should I start this reactor's event loop?
  // Perhaps I could somehow get the ORB's Reactor?
  // We could assume that the ACE_Reactor::instance() is the ORB's Reactor
  ACE_Reactor *react_;

  Dispatcher_Task* task_;
};

} //namespace Kokyu

#if defined (__ACE_INLINE__)
#include "Dispatch_Deferrer.i"
#endif /* __ACE_INLINE__ */

#endif //defined KOKYU_HAS_RELEASE_GUARD

#include /**/ "ace/post.h"
#endif //DISPATCH_DEFERRER_H
