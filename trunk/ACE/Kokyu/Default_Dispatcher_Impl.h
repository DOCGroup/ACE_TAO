/* -*- C++ -*- */
/**
 *  @file   Default_Dispatcher_Impl.h
 *
 *  $Id$
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu),
 * Chris Gill, Carlos O'Ryan and other members of the DOC group.
 */

#ifndef DEFAULT_DISPATCHER_IMPL_H
#define DEFAULT_DISPATCHER_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/Task.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Auto_Ptr.h"
#include "kokyu_export.h"
#include "Kokyu_defs.h"
#include "Dispatcher_Impl.h"
#include "Dispatcher_Task.h"

namespace Kokyu
{
  /**
   * @class Default_Dispatcher_Impl
   *
   * @brief Default implementation class for EC dispatcher
   * implementations
   *
   */
  class Default_Dispatcher_Impl : public Dispatcher_Impl
  {
  public:
    Default_Dispatcher_Impl ();

  private:
    int activate_i ();
    int init_i (const Dispatcher_Attributes&);
    int dispatch_i (const Dispatch_Command*,
                  const QoSDescriptor&);
    int shutdown_i ();
    Dispatcher_Task* find_task_with_preemption_prio (Priority_t);

  private:
    typedef auto_ptr<Dispatcher_Task> Dispatcher_Task_Auto_Ptr;
    ACE_Auto_Array_Ptr<Dispatcher_Task_Auto_Ptr> tasks_;
    int ntasks_;
    ConfigInfoSet curr_config_info_;
    int activated_;
  };

  class Shutdown_Task_Command : public Dispatch_Command
  {
  public:
    /// Constructor
    Shutdown_Task_Command (ACE_Allocator *mb_allocator = 0);

    /// Command callback
    int execute ();
  };

} //end of namespace

#if defined (__ACE_INLINE__)
#include "Default_Dispatcher_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* DEFAULT_DISPATCHER_IMPL_H */
