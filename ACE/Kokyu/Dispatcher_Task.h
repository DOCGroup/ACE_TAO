/* -*- C++ -*- */
/**
 *  @file   Dispatcher_Task.h
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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Kokyu_defs.h"
#include <memory>

namespace Kokyu
{
class Dispatch_Queue_Item : public ACE_Message_Block
{
public:
  Dispatch_Queue_Item (Dispatch_Queue_Item const &) = delete;
  Dispatch_Queue_Item (Dispatch_Queue_Item &&) = delete;
  Dispatch_Queue_Item &operator= (Dispatch_Queue_Item const &) = delete;
  Dispatch_Queue_Item &operator= (Dispatch_Queue_Item &&) = delete;

  Dispatch_Queue_Item (
        Dispatch_Command const *cmd,
        QoSDescriptor const &qos_info,
        ACE_Allocator *mb_allocator = nullptr);

  Dispatch_Queue_Item (
        Dispatch_Command const *cmd,
        QoSDescriptor const &qos_info,
        ACE_Data_Block *data_block,
        int flags,
        ACE_Allocator *mb_allocator = nullptr);

  Dispatch_Command *command ();

private:
  void init_i (QoSDescriptor const &);

private:
  // The caller retains ownership when can_be_deleted() is false; otherwise
  // the worker destroys the command after execution.
  Dispatch_Command const *command_;
  QoSDescriptor qos_info_;
};

/**
 * @class Dispatcher_Task
 *
 * @brief Implement the dispatching queues for FIFO and Priority
 * dispatching.
 */
class Dispatcher_Task : public ACE_Task<ACE_SYNCH>
{
public:
  explicit Dispatcher_Task (ConfigInfo const &config_info,
                            ACE_Thread_Manager *thr_manager = nullptr);

  ~Dispatcher_Task () override = default;
  Dispatcher_Task (Dispatcher_Task const &) = delete;
  Dispatcher_Task (Dispatcher_Task &&) = delete;
  Dispatcher_Task &operator= (Dispatcher_Task const &) = delete;
  Dispatcher_Task &operator= (Dispatcher_Task &&) = delete;

  int initialize ();

  int enqueue (Dispatch_Command const *cmd,
               QoSDescriptor const &qos_info);

  /// Process the events in the queue.
  int svc () override;

  ConfigInfo const &get_curr_config_info () const;
  Priority_t preemption_priority () const;

private:
  static constexpr int ALLOC_POOL_CHUNKS = 200;

  ConfigInfo curr_config_info_;

  std::unique_ptr<ACE_Allocator> owned_allocator_;
  ACE_Allocator *allocator_; // Borrowed unless owned_allocator_ owns it.

  /// Helper data structure to minimize memory allocations...
  ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> > data_block_;
  // Destroy the queue before its data block and allocator.
  std::unique_ptr<ACE_Message_Queue<ACE_SYNCH> > owned_queue_;
};
} //end of namespace

#if defined (__ACE_INLINE__)
#include "Dispatcher_Task.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* DISPATCHER_TASK_H */
