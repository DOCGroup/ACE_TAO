/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Dispatching_Task
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_DISPATCHING_TASK_H
#define TAO_EC_DISPATCHING_TASK_H
#include "ace/pre.h"

#include "EC_ProxySupplier.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventCommC.h"
#include "event_export.h"
#include "ace/Task.h"
#include "ace/Message_Block.h"

class TAO_RTEvent_Export TAO_EC_Queue : public ACE_Message_Queue<ACE_SYNCH>
{
public:
  TAO_EC_Queue (size_t high_water_mark = ACE_Message_Queue_Base::DEFAULT_HWM,
                size_t low_water_mark = ACE_Message_Queue_Base::DEFAULT_LWM,
                ACE_Notification_Strategy * = 0);

protected:
  // = Override the default definition in the Message_Queue, to count
  // the number of messages (and not their size).
  virtual int is_full_i (void);
};

class TAO_RTEvent_Export TAO_EC_Dispatching_Task : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //   Implement the dispatching queues for FIFO and Priority
  //   dispatching.
  //
  // = DESCRIPTION
  //
public:
  TAO_EC_Dispatching_Task (ACE_Thread_Manager* thr_manager = 0);
  // Constructor

  virtual int svc (void);
  // Process the events in the queue.

  virtual void push (TAO_EC_ProxyPushSupplier *proxy,
                     RtecEventComm::PushConsumer_ptr consumer,
                     RtecEventComm::EventSet& event,
                     CORBA::Environment &env);

private:
  ACE_Allocator *allocator_;
  // An per-task allocator

  ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> > data_block_;
  // Helper data structure to minimize memory allocations...

  TAO_EC_Queue the_queue_;
  // The queue
};

// ****************************************************************

class TAO_RTEvent_Export TAO_EC_Dispatch_Command : public ACE_Message_Block
{
public:
  TAO_EC_Dispatch_Command (ACE_Allocator *mb_allocator = 0);
  // Constructor, it will allocate its own data block

  TAO_EC_Dispatch_Command (ACE_Data_Block*,
                           ACE_Allocator *mb_allocator = 0);
  // Constructor, it assumes ownership of the data block

  virtual ~TAO_EC_Dispatch_Command (void);
  // Destructor

  virtual int execute (CORBA::Environment&) = 0;
  // Command callback
};

// ****************************************************************

class TAO_RTEvent_Export TAO_EC_Shutdown_Task_Command : public TAO_EC_Dispatch_Command
{
public:
  TAO_EC_Shutdown_Task_Command (ACE_Allocator *mb_allocator = 0);
  // Constructor

  virtual int execute (CORBA::Environment&);
  // Command callback
};

// ****************************************************************

class TAO_RTEvent_Export TAO_EC_Push_Command : public TAO_EC_Dispatch_Command
{
public:
  TAO_EC_Push_Command (TAO_EC_ProxyPushSupplier* proxy,
                       RtecEventComm::PushConsumer_ptr consumer,
                       RtecEventComm::EventSet& event,
                       ACE_Data_Block* data_block,
                       ACE_Allocator *mb_allocator);
  // Constructor

  virtual ~TAO_EC_Push_Command (void);
  // Destructor

  virtual int execute (CORBA::Environment&);
  // Command callback

private:
  TAO_EC_ProxyPushSupplier* proxy_;
  // The proxy

  RtecEventComm::PushConsumer_var consumer_;
  // The consumer connected to the proxy when the event was pushed.

  RtecEventComm::EventSet event_;
  // The event
};

#if defined (__ACE_INLINE__)
#include "EC_Dispatching_Task.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_DISPATCHING_TASK_H */
