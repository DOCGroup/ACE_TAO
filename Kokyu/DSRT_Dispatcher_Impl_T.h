/* -*- C++ -*- */
/**
 *  @file   DSRT_Dispatcher_Impl.h
 *
 *  $Id$
 *
 */

#ifndef DSRT_DISPATCHER_IMPL_H
#define DSRT_DISPATCHER_IMPL_H
#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
//# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Kokyu_dsrt.h"

namespace Kokyu
{
  /**
   * @class DSRT_Dispatcher
   *
   * @brief Base class for DSRT dispatcher implementations
   *
   * The responsibility of this class is to act as a common base class
   * for different DSRT dispatcher implementations. This is an
   * abstract base class and cannot be instantiated.
   */
  template <class DSRT_Scheduler_Traits>
  class DSRT_Dispatcher_Impl
  {
  public:
    typedef typename DSRT_Scheduler_Traits::Guid_t Guid_t;
    typedef typename DSRT_Scheduler_Traits::QoSDescriptor_t DSRT_QoSDescriptor;

    /// Configure the DSRT dispatcher.
    int init (const DSRT_ConfigInfo&);

    /// Schedule a thread dynamically based on the qos info supplied.
    int schedule (Guid_t guid,
                  const DSRT_QoSDescriptor&);

    /// Update the schedule for a thread. This could alter the current
    /// schedule.
    int update_schedule (Guid_t guid,
                         const DSRT_QoSDescriptor&);

    /// Inform the scheduler that the caller thread is about to
    /// block. This could alter the current schedule.
    int update_schedule (Guid_t guid, Block_Flag_t flag);

    /// Cancel the schedule for a thread. This could alter the current
    /// schedule.
    int cancel_schedule (Guid_t guid);

    /// Shut down the dispatcher. The dispatcher will stop processing
    /// requests.
    int shutdown ();

    virtual ~DSRT_Dispatcher_Impl ();

  private:

    //following an idiom to avoid public virtual functions.
    //instead make them private and use the template method
    //pattern - "Virtually Yours" article in CUJ Experts Forum

    virtual int init_i (const DSRT_ConfigInfo&)=0;
    virtual int schedule_i (Guid_t guid,
                    const DSRT_QoSDescriptor&)=0;
    virtual int update_schedule_i (Guid_t guid,
                           const DSRT_QoSDescriptor&)=0;
    virtual int update_schedule_i (Guid_t guid, Block_Flag_t flag)=0;
    virtual int cancel_schedule_i (Guid_t guid)=0;
    virtual int shutdown_i ()=0;

  };

} //end of namespace

#if defined (__ACE_INLINE__)
#include "DSRT_Dispatcher_Impl_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "DSRT_Dispatcher_Impl_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("DSRT_Dispatcher_Impl_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* DSRT_DISPATCHER_IMPL_H */
