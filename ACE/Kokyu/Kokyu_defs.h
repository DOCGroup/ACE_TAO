/* -*- C++ -*- */
/**
 *  @file   Kokyu_defs.h
 *
 *  $Id$
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
 */

#ifndef KOKYU_DEFS_H
#define KOKYU_DEFS_H
#include /**/ "ace/pre.h"
#include "ace/Containers_T.h"
#include "ace/Time_Value.h"
#include "ace/Auto_Ptr.h"
#include "ace/Message_Block.h"
#include "ace/Sched_Params.h"
#include "ace/Malloc_Allocator.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "kokyu_export.h"

namespace Kokyu
{
  typedef long Priority_t;
  typedef ACE_Time_Value Deadline_t; //absolute deadline
  typedef ACE_Time_Value Execution_Time_t; //execution time
  //typedef int Guid_t;

  enum Dispatching_Type_t
    // Defines the type of prioritization strategy
    // to be used by a dispatching queue
    {
      FIFO_DISPATCHING,
      DEADLINE_DISPATCHING,
      LAXITY_DISPATCHING
    };

  enum Criticality_t
    // Defines the criticality of the operation.
    // For use with Dynamic Scheduler.
    {
      VERY_LOW_CRITICALITY,
      LOW_CRITICALITY,
      MEDIUM_CRITICALITY,
      HIGH_CRITICALITY,
      VERY_HIGH_CRITICALITY
    };

  enum Importance_t
    // Defines the importance of the operation,
    // which can be used by the RtecScheduler as a
    // "tie-breaker" when other scheduling
    // parameters are equal.
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
    unsigned long static_bit_field_mask_;
    unsigned long static_bit_field_shift_;
    unsigned long dynamic_priority_max_;
    unsigned long dynamic_priority_offset_;
  };

  struct Kokyu_Export ConfigInfo
  {
    Priority_t preemption_priority_;

    // OS priority of the dispatching thread associated with the queue
    Priority_t thread_priority_;

    // type of dispatching queue
    Dispatching_Type_t dispatching_type_;

    //allocator to be used for dynamic memory allocation. If each
    //thread gets its own memory pool, contention will be less
    ACE_Allocator *allocator_;

    Reordering_Queue_Attributes reordering_flags_;

    ConfigInfo ();
  };

  typedef ACE_Array<ConfigInfo> ConfigInfoSet;

  class Kokyu_Export Dispatcher_Attributes
  {
  public:
    ConfigInfoSet config_info_set_;
    int immediate_activation_;

  public:
    Dispatcher_Attributes ();
    void sched_policy (int);
    void sched_scope (int);
    int thread_creation_flags () const;

  private:
    int sched_policy_;
    int sched_scope_;
    int base_thread_creation_flags_;
  };


  struct Kokyu_Export QoSDescriptor
  {
    Priority_t preemption_priority_;
    Deadline_t deadline_;
    Execution_Time_t execution_time_;
    Importance_t importance_;
  };

  enum Block_Flag_t {BLOCK, UNBLOCK};

  class Kokyu_Export Dispatch_Command
    {
    public:
      Dispatch_Command(int dont_delete = 0,
                       ACE_Allocator *allocator = 0);
      //dont_delete indicates whether this object needs to be deleted once processed.
      //allocator indicates the ACE_Allocator, if any, from which this object was created.
      //This same allocator has to be used for the deletion also

      /// Command callback
      virtual int execute () = 0;

      int can_be_deleted () const;

      void destroy (void);
    protected:
      /// Destructor
      // only inheritance is possible and object should be on heap,
      // since object could be handed over to a different thread.
      virtual ~Dispatch_Command (void);

    private:
      int dont_delete_;
      ACE_Allocator *allocator_;
      //if this object has to be deleted, then delete it using the allocator
      //if one is present.
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
    //not used currently
    DSRT_Sched_Type_t sched_strategy_;

    ACE_Sched_Params::Policy sched_policy_;
    int sched_scope_;

    //type of implementation
    DSRT_Dispatcher_Impl_t impl_type_;

    DSRT_ConfigInfo ();
  };

} //end of namespace

//to satisfy ACE_Array<ConfigInfo>
ACE_INLINE bool operator != (const Kokyu::ConfigInfo& lhs, const Kokyu::ConfigInfo& rhs);

#if defined (__ACE_INLINE__)
#include "Kokyu_defs.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* KOKYU_DEFS_H */
