/* -*- C++ -*- */
/**
 *  @file  DSRT_Schedulers.h
 *
 *  $Id$
 *
 *  @author Venkita Subramonian (venkita@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu),
 * Chris Gill, Carlos O'Ryan and other members of the DOC group.
 */

#ifndef DSRT_SCHEDULERS_H
#define DSRT_SCHEDULERS_H
#include "ace/pre.h"
#include "ace/OS.h"

#include "Kokyu.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Kokyu
{
  class DSRT_Scheduler_Impl : public ACE_Service_Object
  {
  public:
    DSRT_Scheduler_Impl ();
    int schedule (guid_t, const DSRT_QoSDescriptor&);

  private:
    virtual Priority_t schedule_i (guid_t, const DSRT_QoSDescriptor&) = 0;

  protected:
    ACE_Sched_Params::Policy sched_policy_;
    int sched_scope_;
    Priority_t min_prio_;
    Priority_t max_prio_;
    Priority_t prio_range_;
  };

  class MIF_Scheduler_Impl : public DSRT_Scheduler_Impl
  {
  public:
    MIF_Scheduler_Impl ();
    virtual int init (int argc, ACE_TCHAR* argv[]);
    virtual int fini (void);
    static void init_svcs (void);

  private:
    virtual Priority_t schedule_i (guid_t, const DSRT_QoSDescriptor&);

  private:
    short min_importance_;
    short max_importance_;
    short importance_range_;
  };
}

//could not use SVC declare macros here because of Kokyu namespace
//need to revisit later
extern ACE_Static_Svc_Descriptor ace_svc_desc_MIF_Scheduler_Impl;

ACE_FACTORY_DECLARE (Kokyu, MIF_Scheduler_Impl)

#if defined (__ACE_INLINE__)
//#include "DSRT_Schedulers.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* DSRT_SCHEDULERS_H */
