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

#include "kokyu_export.h"
#include "Kokyu.h"

namespace Kokyu
{
  class DSRT_Dispatcher_Impl
  {
  public:
    virtual ~DSRT_Dispatcher_Impl ();
    //    int activate ();
    int init (const DSRT_ConfigInfo&);
    int schedule (guid_t guid,
                    const QoSDescriptor&);
    int update_schedule (guid_t guid,
                           const QoSDescriptor&);
    int cancel_schedule (guid_t guid,
                           const QoSDescriptor&);

  private:
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
  };

} //end of namespace

#if defined (__ACE_INLINE__)
#include "DSRT_Dispatcher_Impl.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* DEFAULT_DISPATCHER_IMPL_H */
