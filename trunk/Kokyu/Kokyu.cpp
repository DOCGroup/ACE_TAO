// $Id$

#include "Kokyu.h"
#include "Default_Dispatcher_Impl.h"

#if ! defined (__ACE_INLINE__)
#include "Kokyu.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Kokyu, Dispatcher_Impl, "$Id$")

namespace Kokyu
{

int Dispatcher::dispatch (const Dispatch_Command* cmd, const QoSDescriptor& qos)
{
  return dispatcher_impl_->dispatch (cmd, qos);
}

int Dispatcher::shutdown ()
{
  return dispatcher_impl_->shutdown ();
}


void Dispatcher::implementation (Dispatcher_Impl* impl)
{
  dispatcher_impl_.reset (impl);
}


Dispatcher*
Dispatcher_Factory::
create_dispatcher (const ConfigInfoSet& config_info_set)
{
  Dispatcher_Impl* tmp;
  ACE_NEW_RETURN (tmp, Default_Dispatcher_Impl, (Dispatcher*)0);
  Dispatcher* disp;
  ACE_NEW_RETURN (disp, Dispatcher, (Dispatcher*)0);
  disp->implementation (tmp);
  tmp->init (config_info_set);
  return disp;
}

ACE_INLINE void
DSRT_Dispatcher::implementation (DSRT_Dispatcher_Impl* impl)
{
  dispatcher_impl_ = impl;
}

int
DSRT_Dispatcher::schedule (guid_t guid, const QoSDescriptor& qos)
{
  return dispatcher_impl_->schedule (guid, qos);
}

ACE_INLINE int
DSRT_Dispatcher::update_schedule (guid_t guid, const QoSDescriptor& qos)
{
  return dispatcher_impl_->update_schedule (guid, qos);
}

ACE_INLINE int
DSRT_Dispatcher::cancel_schedule (guid_t guid, const QoSDescriptor& qos)
{
  return dispatcher_impl_->cancel_schedule (guid, qos);
}

DSRT_Dispatcher*
Dispatcher_Factory::
create_DSRT_dispatcher (const DSRT_ConfigInfo& config_info)
{
  DSRT_Dispatcher_Impl* tmp;
  ACE_NEW_RETURN (tmp, DSRT_Dispatcher_Impl, (DSRT_Dispatcher*)0);
  DSRT_Dispatcher* disp;
  ACE_NEW_RETURN (disp, DSRT_Dispatcher, (DSRT_Dispatcher*)0);
  disp->implementation (tmp);
  tmp->init (config_info);
  return disp;
}

}
