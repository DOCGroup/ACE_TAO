/* -*- C++ -*- */
/**
 *  @file   Kokyu.h
 *
 *  $Id$
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu),
 * Chris Gill, Carlos O'Ryan and other members of the DOC group.
 */

#ifndef KOKYU_H
#define KOKYU_H
#include /**/ "ace/pre.h"
#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Array.h"
#include "ace/Time_Value.h"
#include "ace/Auto_Ptr.h"
#include "ace/Message_Block.h"
#include "ace/Sched_Params.h"

#include "kokyu_export.h"

namespace Kokyu
{
  typedef long Priority_t;
  typedef ACE_Time_Value Deadline_t; //absolute deadline
  typedef ACE_Time_Value Execution_Time_t; //execution time

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

  struct DSRT_QoSDescriptor
  {
    short importance_;
  };

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

  class Dispatcher_Impl;
  class DSRT_Dispatcher_Impl;

  struct DSRT_ConfigInfo
  {
  };

  typedef int guid_t;
  class Kokyu_Export DSRT_Dispatcher
  {
  public:
    int schedule (guid_t guid, const DSRT_QoSDescriptor&);
    int update_schedule (guid_t guid, const DSRT_QoSDescriptor&);
    int cancel_schedule (guid_t guid, const DSRT_QoSDescriptor&);
    void implementation (DSRT_Dispatcher_Impl*);

  private:
    DSRT_Dispatcher_Impl* dispatcher_impl_;
  };

  class Kokyu_Export Dispatcher
  {
  public:
    int dispatch (const Dispatch_Command*, const QoSDescriptor&);
    int shutdown ();
    void implementation (Dispatcher_Impl*);
  private:
    auto_ptr<Dispatcher_Impl> dispatcher_impl_;
  };

  class Kokyu_Export Dispatcher_Factory
    {
    public:
      //@@ Should we return auto_ptr<Dispatcher> instead?
      static Dispatcher* create_dispatcher (const ConfigInfoSet&);
      static DSRT_Dispatcher* create_DSRT_dispatcher (const DSRT_ConfigInfo&);
    };
} //end of namespace

#if defined (__ACE_INLINE__)
#include "Kokyu.i"
#endif /* __ACE_INLINE__ */

#include "Dispatcher_Impl.h"
#include "DSRT_Dispatcher_Impl.h"

#include /**/ "ace/post.h"
#endif /* KOKYU_H */
