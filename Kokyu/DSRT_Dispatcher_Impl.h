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
#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Message_Block.h"
#include "ace/Auto_Ptr.h"
#include "ace/Service_Config.h"

#include "ace/Sched_Params.h"

#include "kokyu_export.h"
#include "Kokyu.h"

namespace Kokyu
{
  class DSRT_Scheduler_Impl;

  class DSRT_Dispatcher_Impl : public ACE_Service_Object
  {
  public:

    static int init_svcs (void);

    virtual ~DSRT_Dispatcher_Impl ();
    //    int activate ();

    // = The Service_Object entry points
    virtual int init (int argc, ACE_TCHAR* argv[]);
    virtual int fini (void);

    int init (const DSRT_ConfigInfo&);
    int schedule (guid_t guid,
                    const QoSDescriptor&);
    int update_schedule (guid_t guid,
                           const QoSDescriptor&);
    int cancel_schedule (guid_t guid,
                           const QoSDescriptor&);

  private:
    DSRT_Scheduler_Impl* create_scheduler (DSRT_Sched_t sched_type,
                                           ACE_Sched_Params::Policy sched_policy,
                                           int sched_scope);

    int init_i (const DSRT_ConfigInfo&);
    int schedule_i (guid_t guid,
                    const QoSDescriptor&);
    int update_schedule_i (guid_t guid,
                           const QoSDescriptor&);
    int cancel_schedule_i (guid_t guid,
                           const QoSDescriptor&);

  private:
    //    typedef auto_ptr<DSRT_Dispatcher_Task> DSRT_Dispatcher_Task_Auto_Ptr;
    //    DSRT_Dispatcher_Task_Auto_Ptr task_;
    DSRT_ConfigInfo curr_config_info_;
    DSRT_Sched_t sched_type_;
    DSRT_Scheduler_Impl* scheduler_impl_;
  };

} //end of namespace

extern ACE_Static_Svc_Descriptor ace_svc_desc_DSRT_Dispatcher_Impl;

//ACE_STATIC_SVC_DECLARE (DSRT_Dispatcher_Impl)
ACE_FACTORY_DECLARE (Kokyu, DSRT_Dispatcher_Impl)

#if defined (__ACE_INLINE__)
#include "DSRT_Dispatcher_Impl.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* DEFAULT_DISPATCHER_IMPL_H */
