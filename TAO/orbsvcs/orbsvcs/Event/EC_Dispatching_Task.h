// -*- C++ -*-

/**
 *  @file   EC_Dispatching_Task.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_DISPATCHING_TASK_H
#define TAO_EC_DISPATCHING_TASK_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Event/EC_ProxySupplier.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventCommC.h"
#include /**/ "orbsvcs/Event/event_serv_export.h"
#include "ace/Task.h"
#include "ace/Message_Block.h"
#include "ace/Lock_Adapter_T.h"
#include "ace/Service_Config.h"
#include "ace/Global_Macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decl
class TAO_EC_Dispatching_Task;

class TAO_RTEvent_Serv_Export TAO_EC_Queue_Full_Service_Object : public ACE_Service_Object
{
public:
  enum QueueFullActionReturnValue
  {
    WAIT_TO_EMPTY = 0,
    SILENTLY_DISCARD = -1
  };

  // Called when
  virtual int queue_full_action (TAO_EC_Dispatching_Task *task,
                                 TAO_EC_ProxyPushSupplier *proxy,
                                 RtecEventComm::PushConsumer_ptr consumer,
                                 RtecEventComm::EventSet& event) = 0;
};

class TAO_RTEvent_Serv_Export TAO_EC_Simple_Queue_Full_Action :
  public TAO_EC_Queue_Full_Service_Object
{
public:
  TAO_EC_Simple_Queue_Full_Action ();

  /// Helper function to register the default action into the service
  /// configurator.
  static int init_svcs (void);

  // = The Service_Object entry points
  virtual int init (int argc, ACE_TCHAR* argv[]);
  virtual int fini (void);

  virtual int queue_full_action (TAO_EC_Dispatching_Task *task,
                                 TAO_EC_ProxyPushSupplier *proxy,
                                 RtecEventComm::PushConsumer_ptr consumer,
                                 RtecEventComm::EventSet& event);

protected:
  int queue_full_action_return_value_;
};

class TAO_RTEvent_Serv_Export TAO_EC_Queue : public ACE_Message_Queue<ACE_SYNCH>
{
public:
  TAO_EC_Queue (size_t high_water_mark = ACE_Message_Queue_Base::DEFAULT_HWM,
                size_t low_water_mark = ACE_Message_Queue_Base::DEFAULT_LWM,
                ACE_Notification_Strategy * = 0);

protected:
  // = Override the default definition in the Message_Queue, to count
  // the number of messages (and not their size).
  virtual bool is_full_i (void);
};

/**
 * @class TAO_EC_Dispatching_Task
 *
 * @brief Implement the dispatching queues for FIFO and Priority
 * dispatching.
 */
class TAO_RTEvent_Serv_Export TAO_EC_Dispatching_Task : public ACE_Task<ACE_SYNCH>
{
public:

#ifdef ACE_WIN32_VC14
  // Work around for Connect #2869042 Bugzilla #4215
  TAO_EC_Dispatching_Task (ACE_Thread_Manager *thr_manager, TAO_EC_Queue_Full_Service_Object *queue_full_service_object = 0);
  TAO_EC_Dispatching_Task ();
#else
  TAO_EC_Dispatching_Task (ACE_Thread_Manager *thr_manager = 0, TAO_EC_Queue_Full_Service_Object *queue_full_service_object = 0);
#endif

  /// Process the events in the queue.
  virtual int svc (void);

  virtual void push (TAO_EC_ProxyPushSupplier *proxy,
                     RtecEventComm::PushConsumer_ptr consumer,
                     RtecEventComm::EventSet& event);

private:
  /// An per-task allocator
  ACE_Allocator *allocator_;

  /// Helper data structure to minimize memory allocations...
  ACE_Locked_Data_Block<ACE_Lock_Adapter<TAO_SYNCH_MUTEX> > data_block_;

  /// The queue
  TAO_EC_Queue the_queue_;

  TAO_EC_Queue_Full_Service_Object* queue_full_service_object_;
};

// ****************************************************************

class TAO_RTEvent_Serv_Export TAO_EC_Dispatch_Command : public ACE_Message_Block
{
public:
  /// Constructor, it will allocate its own data block
  TAO_EC_Dispatch_Command (ACE_Allocator *mb_allocator = 0);

  /// Constructor, it assumes ownership of the data block
  TAO_EC_Dispatch_Command (ACE_Data_Block*,
                           ACE_Allocator *mb_allocator = 0);

  /// Destructor
  virtual ~TAO_EC_Dispatch_Command (void);

  /// Command callback
  virtual int execute (void) = 0;
};

// ****************************************************************

class TAO_RTEvent_Serv_Export TAO_EC_Shutdown_Task_Command : public TAO_EC_Dispatch_Command
{
public:
  /// Constructor
  TAO_EC_Shutdown_Task_Command (ACE_Allocator *mb_allocator = 0);

  /// Command callback
  virtual int execute (void);
};

// ****************************************************************

class TAO_RTEvent_Serv_Export TAO_EC_Push_Command : public TAO_EC_Dispatch_Command
{
public:
  /// Constructor
  TAO_EC_Push_Command (TAO_EC_ProxyPushSupplier* proxy,
                       RtecEventComm::PushConsumer_ptr consumer,
                       RtecEventComm::EventSet& event,
                       ACE_Data_Block* data_block,
                       ACE_Allocator *mb_allocator);

  /// Destructor
  virtual ~TAO_EC_Push_Command (void);

  /// Command callback
  virtual int execute (void);

private:
  /// The proxy
  TAO_EC_ProxyPushSupplier* proxy_;

  /// The consumer connected to the proxy when the event was pushed.
  RtecEventComm::PushConsumer_var consumer_;

  /// The event
  RtecEventComm::EventSet event_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Dispatching_Task.inl"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_EC_Simple_Queue_Full_Action)
ACE_FACTORY_DECLARE (TAO_RTEvent_Serv, TAO_EC_Simple_Queue_Full_Action)

#include /**/ "ace/post.h"

#endif  /* TAO_EC_DISPATCHING_TASK_H */
