/* -*- C++ -*- */
/**
 *  @file   Dispatcher_Task.h
 *
 *  $Id$
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu),
 * Chris Gill, Carlos O'Ryan and other members of the DOC group.
 */

#ifndef TAO_DISPATCHER_TASK_H
#define TAO_DISPATCHER_TASK_H
#include "ace/pre.h"
#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"

#include "kokyu_export.h"
#include "Kokyu.h"

namespace Kokyu
{

class Dispatch_Queue_Item : public ACE_Message_Block
{
public:
  Dispatch_Queue_Item (const Dispatch_Command* , const QoSDescriptor&);

  Dispatch_Queue_Item::Dispatch_Queue_Item (
        const Dispatch_Command* cmd,
        const QoSDescriptor& qos_info,
        ACE_Allocator* mb_allocator =0);

  Dispatch_Queue_Item::Dispatch_Queue_Item (
        const Dispatch_Command* cmd,
        const QoSDescriptor& qos_info,
        ACE_Data_Block* data_block,
        ACE_Allocator* mb_allocator =0);

  Dispatch_Command* command ();

private:
  void init_i(const QoSDescriptor&);

private:
  const Dispatch_Command* command_;
  QoSDescriptor qos_info_;
};

/**
 * @class Dispatcher_Task
 *
 * @brief Implement the dispatching queues for FIFO and Priority
 * dispatching.
 *
 */
class Dispatcher_Task : public ACE_Task<ACE_SYNCH>
{
public:
  /// Constructor
  Dispatcher_Task (const ConfigInfo& config_info,
                   ACE_Thread_Manager* thr_manager = 0);

  int init();

  int enqueue (const Dispatch_Command* cmd,
           const QoSDescriptor& qos_info);

  /// Process the events in the queue.
  int svc (void);

  const ConfigInfo& get_curr_config_info() const;
  Priority_t preemption_priority() const;

private:
  static int get_native_prio();

private:
  /// An per-task allocator
  ACE_Allocator *allocator_;

  /// Helper data structure to minimize memory allocations...
  ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> > data_block_;

  /// The queue
  ACE_Message_Queue<ACE_SYNCH>* the_queue_;

  ConfigInfo curr_config_info_;
  ACE_Deadline_Message_Strategy deadline_msg_strategy_;
  ACE_Laxity_Message_Strategy laxity_msg_strategy_;
};

} //end of namespace

#if defined (__ACE_INLINE__)
#include "Dispatcher_Task.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* DISPATCHER_TASK_H */
