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

#ifndef DISPATCHER_TASK_H
#define DISPATCHER_TASK_H
#include /**/ "ace/pre.h"
#include "ace/Task.h"
#include "ace/Lock_Adapter_T.h"

#ifdef KOKYU_HAS_RELEASE_GUARD
#include "ace/Map.h"
#include "Dispatch_Deferrer.h"
#endif //KOKYU_HAS_RELEASE_GUARD

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Kokyu_defs.h"

namespace Kokyu
{

class Dispatch_Queue_Item : public ACE_Message_Block
{
public:
  Dispatch_Queue_Item (
        const Dispatch_Command* cmd,
        const QoSDescriptor& qos_info,
        ACE_Allocator* mb_allocator =0);

  Dispatch_Queue_Item (
        const Dispatch_Command* cmd,
        const QoSDescriptor& qos_info,
        ACE_Data_Block* data_block,
        int flags,
        ACE_Allocator* mb_allocator =0);

  Dispatch_Command* command ();

  //Any reason why this shouldn't be visible?
  const QoSDescriptor& qos_info() const;

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


  ~Dispatcher_Task ();
  int initialize();

  int enqueue (const Dispatch_Command* cmd,
           const QoSDescriptor& qos_info);

  int enqueue (Dispatch_Queue_Item *qitem);

  /// Process the events in the queue.
  int svc (void);

  const ConfigInfo& get_curr_config_info() const;
  Priority_t preemption_priority() const;

private:
  static int get_native_prio();

private:
  ConfigInfo curr_config_info_;

  ACE_Allocator *allocator_;
  int own_allocator_;

  /// Helper data structure to minimize memory allocations...
  ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> > data_block_;

  /// The queue
  ACE_Message_Queue<ACE_SYNCH>* the_queue_;

  ACE_Deadline_Message_Strategy deadline_msg_strategy_;
  ACE_Laxity_Message_Strategy laxity_msg_strategy_;

#ifdef KOKYU_HAS_RELEASE_GUARD
  //TODO: What's the best way to identify periodic events?
  //For now, use QoSDescriptor equivalence.
  //Maps QoSDescriptors to last release time of events for that
  //QoSDescriptor.  Seems kind of wasteful to store the whole
  //QoSDescriptor as the key, but I don't see any other way since each
  //Dispatch_Queue_Item has its own instance of the QoSDescriptor.
  //The release time is stored as a long as if from
  //ACE_Time_Value.msec().
  typedef ACE_Map_Manager<QoSDescriptor,long,ACE_SYNCH_NULL_MUTEX> Release_Time_Map;

  Release_Time_Map releases_;

  //For delaying dispatch until required by RG:
  Dispatch_Deferrer_Attributes deferrer_attr_;
  Dispatch_Deferrer deferrer_;
#endif //KOKYU_HAS_RELEASE_GUARD
};

} //end of namespace

#if defined (__ACE_INLINE__)
#include "Dispatcher_Task.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* DISPATCHER_TASK_H */
