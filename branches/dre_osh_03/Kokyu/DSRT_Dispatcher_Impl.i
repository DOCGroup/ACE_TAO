// $Id$

namespace Kokyu
{

ACE_INLINE
DSRT_Dispatcher_Impl::~DSRT_Dispatcher_Impl ()
{
}

ACE_INLINE
int DSRT_Dispatcher_Impl::init (const DSRT_ConfigInfo& config_info)
{
  return this->init_i (config_info);
}

ACE_INLINE
int DSRT_Dispatcher_Impl::schedule (guid_t guid,
                                    const DSRT_QoSDescriptor& qos)
{
  return this->schedule_i (guid, qos);
}

ACE_INLINE
int DSRT_Dispatcher_Impl::update_schedule (guid_t guid,
                                           const DSRT_QoSDescriptor& qos)
{
  return this->update_schedule_i (guid, qos);
}

ACE_INLINE
int DSRT_Dispatcher_Impl::cancel_schedule (guid_t guid,
                                           const DSRT_QoSDescriptor& qos)
{
  return this->cancel_schedule_i (guid, qos);
}

}
