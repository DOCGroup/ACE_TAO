/* -*- C++ -*- */
/**
 *  @file   Kokyu_defs.h
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 */

#ifndef KOKYU_DEFS_H
#define KOKYU_DEFS_H
#include /**/ "ace/pre.h"
#include "ace/Containers_T.h"
#include "ace/Time_Value.h"
#include "ace/Message_Block.h"
#include "ace/Sched_Params.h"
#include "ace/Malloc_Allocator.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "kokyu_export.h"

namespace Kokyu
{
  using Priority_t = long;
  /// Absolute deadline.
  using Deadline_t = ACE_Time_Value;
  /// Execution time.
  using Execution_Time_t = ACE_Time_Value;

  /// Type of prioritization strategy to use for a dispatching queue.
  enum Dispatching_Type_t
    {
      FIFO_DISPATCHING,
      DEADLINE_DISPATCHING,
      LAXITY_DISPATCHING
    };

  /// Criticality of an operation for dynamic scheduling.
  enum Criticality_t
    {
      VERY_LOW_CRITICALITY,
      LOW_CRITICALITY,
      MEDIUM_CRITICALITY,
      HIGH_CRITICALITY,
      VERY_HIGH_CRITICALITY
    };

  /// Importance used by RtecScheduler as a tie-breaker when other scheduling
  /// parameters are equal.
  enum Importance_t
    {
      VERY_LOW_IMPORTANCE,
      LOW_IMPORTANCE,
      MEDIUM_IMPORTANCE,
      HIGH_IMPORTANCE,
      VERY_HIGH_IMPORTANCE
    };

  struct Kokyu_Export Reordering_Queue_Attributes
  {
    Reordering_Queue_Attributes ();
    unsigned long static_bit_field_mask_ {};
    unsigned long static_bit_field_shift_ {};
    unsigned long dynamic_priority_max_ {0x7FFFFFFFUL};
    unsigned long dynamic_priority_offset_ {0x08000000UL};
  };

  struct Kokyu_Export ConfigInfo
  {
    Priority_t preemption_priority_ {};

    /// OS priority of the dispatching thread associated with the queue.
    Priority_t thread_priority_ {};

    /// Type of dispatching queue.
    Dispatching_Type_t dispatching_type_ {FIFO_DISPATCHING};

    /// Borrowed allocator for dynamic memory; it must outlive tasks using this configuration.
    /// A separate allocator per thread can reduce contention.
    ACE_Allocator *allocator_ {};

    Reordering_Queue_Attributes reordering_flags_;

    ConfigInfo ();
  };

  using ConfigInfoSet = ACE_Array<ConfigInfo>;

  class Kokyu_Export Dispatcher_Attributes
  {
  public:
    ConfigInfoSet config_info_set_;
    bool immediate_activation_ {};

  public:
    Dispatcher_Attributes ();
    void sched_policy (int);
    void sched_scope (int);
    int thread_creation_flags () const;

  private:
    int sched_policy_ {ACE_SCHED_FIFO};
    int sched_scope_ {ACE_SCOPE_THREAD};
    int base_thread_creation_flags_ {THR_NEW_LWP | THR_BOUND | THR_JOINABLE};
  };

  struct Kokyu_Export QoSDescriptor
  {
    Priority_t preemption_priority_ {};
    Deadline_t deadline_ {};
    Execution_Time_t execution_time_ {};
    Importance_t importance_ {};
  };

  enum Block_Flag_t {BLOCK, UNBLOCK};

  class Kokyu_Export Dispatch_Command
  {
  public:
    Dispatch_Command (Dispatch_Command const &) = delete;
    Dispatch_Command (Dispatch_Command &&) = delete;
    Dispatch_Command &operator= (Dispatch_Command const &) = delete;
    Dispatch_Command &operator= (Dispatch_Command &&) = delete;

    /// @param dont_delete Keeps this object from being deleted after processing.
    /// @param allocator Allocator used to create and delete this object.
    explicit Dispatch_Command (bool dont_delete = false,
                               ACE_Allocator *allocator = nullptr);

    /// Command callback
    virtual int execute () = 0;

    bool can_be_deleted () const;

    void destroy ();
  protected:
    /// Commands are heap allocated because they can be handed to another thread.
    virtual ~Dispatch_Command ();

  private:
    bool dont_delete_;
    /// Allocator used by destroy() to deallocate this command.
    ACE_Allocator *allocator_;
  };

  enum DSRT_Sched_Type_t
    {
      DSRT_FP,
      DSRT_MUF,
      DSRT_MIF
    };

  enum DSRT_Dispatcher_Impl_t
    {
      DSRT_CV_BASED,
      DSRT_OS_BASED
    };

  struct Kokyu_Export DSRT_ConfigInfo
  {
    /// Reserved; currently unused.
    DSRT_Sched_Type_t sched_strategy_ {DSRT_FP};

    ACE_Sched_Params::Policy sched_policy_ {ACE_SCHED_RR};
    int sched_scope_ {ACE_SCOPE_THREAD};

    /// Type of implementation.
    DSRT_Dispatcher_Impl_t impl_type_ {DSRT_CV_BASED};

    DSRT_ConfigInfo ();
  };
}

//to satisfy ACE_Array<ConfigInfo>
ACE_INLINE bool operator != (Kokyu::ConfigInfo const &lhs,
                             Kokyu::ConfigInfo const &rhs);

#if defined (__ACE_INLINE__)
#include "Kokyu_defs.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* KOKYU_DEFS_H */
