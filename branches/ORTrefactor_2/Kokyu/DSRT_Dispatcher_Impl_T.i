// $Id$

namespace Kokyu
{

template <class DSRT_Scheduler_Traits>
ACE_INLINE int
DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>::
init (const DSRT_ConfigInfo& config_info)
{
  return this->init_i (config_info);
}

template <class DSRT_Scheduler_Traits>
ACE_INLINE int
DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>::
schedule (Guid_t guid,
          const DSRT_QoSDescriptor& qos)
{
  return this->schedule_i (guid, qos);
}

template <class DSRT_Scheduler_Traits>
ACE_INLINE int
DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>::
update_schedule (Guid_t guid,
                 const DSRT_QoSDescriptor& qos)
{
  return this->update_schedule_i (guid, qos);
}

template <class DSRT_Scheduler_Traits>
ACE_INLINE int
DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>::
update_schedule (Guid_t guid,
                 Block_Flag_t flag)
{
  return this->update_schedule_i (guid, flag);
}

template <class DSRT_Scheduler_Traits>
ACE_INLINE int
DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>::
cancel_schedule (Guid_t guid)
{
  return this->cancel_schedule_i (guid);
}

template <class DSRT_Scheduler_Traits>
ACE_INLINE int
DSRT_Dispatcher_Impl<DSRT_Scheduler_Traits>::shutdown ()

{
  return this->shutdown_i ();
}

}
