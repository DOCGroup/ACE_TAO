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
#include "ace/pre.h"
#include "ace/Array.h"
#include "ace/Time_Value.h"
#include "ace/Auto_Ptr.h"
#include "ace/Message_Block.h"
#include "ace/Sched_Params.h"

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

  struct ConfigInfo
  {
    Priority_t preemption_priority_;

    // OS priority of the dispatching thread associated with the queue
    Priority_t thread_priority_;

    // type of dispatching queue
    Dispatching_Type_t dispatching_type_;
  };

  typedef ACE_Array<ConfigInfo> ConfigInfoSet;

  struct QoSDescriptor
  {
    Priority_t preemption_priority_;
    Deadline_t deadline_;
    Execution_Time_t execution_time_;
    Importance_t importance_;
  };

  enum Block_Flag_t {BLOCK, UNBLOCK};

  /*
  struct DSRT_QoSDescriptor
  {
    long importance_;
    long criticality_;
    Priority_t priority_;
    Deadline_t deadline_;
    Execution_Time_t exec_time_;
  };
  */

  class Kokyu_Export Dispatch_Command
    {
    public:
      Dispatch_Command(int dont_delete = 0);

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
    };

  class non_copyable
  {
  protected:
    non_copyable (void);
    ~non_copyable (void);
  private:
    non_copyable (const non_copyable&);
    const non_copyable& operator= (const non_copyable&);
  };

} //end of namespace

#if defined (__ACE_INLINE__)
#include "Kokyu_defs.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* KOKYU_DEFS_H */
