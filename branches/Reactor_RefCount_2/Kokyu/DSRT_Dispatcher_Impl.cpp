// $Id$

#include "ace/Sched_Params.h"
#include "ace/Arg_Shifter.h"
#include "ace/Dynamic_Service.h"

#include "DSRT_Dispatcher_Impl.h"
#include "DSRT_Schedulers.h"

#if ! defined (__ACE_INLINE__)
#include "DSRT_Dispatcher_Impl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Kokyu, Dispatcher_Impl, "$Id$")

  //using namespace Kokyu;

namespace Kokyu
{
int
DSRT_Dispatcher_Impl::init_i (const DSRT_ConfigInfo& config_info)
{
  curr_config_info_ = config_info;

  this->scheduler_impl_ =
    ACE_Dynamic_Service<DSRT_Scheduler_Impl>::instance ("DSRT_Scheduler_Impl");

  return 0;
}

int
DSRT_Dispatcher_Impl::schedule_i (guid_t guid,
                  const DSRT_QoSDescriptor& qos_info)
{
  ACE_UNUSED_ARG (guid);
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);

  int prio = this->scheduler_impl_->schedule (guid, qos_info);
  ACE_OS::thr_setprio (thr_handle, prio);

  return prio;
}

int
DSRT_Dispatcher_Impl::update_schedule_i (guid_t guid,
                  const DSRT_QoSDescriptor& qos_info)
{
  ACE_UNUSED_ARG (guid);
  ACE_UNUSED_ARG (qos_info);
  return 0;
}

int
DSRT_Dispatcher_Impl::cancel_schedule_i (guid_t guid,
                  const DSRT_QoSDescriptor& qos_info)
{
  return schedule_i (guid, qos_info);
}

}
