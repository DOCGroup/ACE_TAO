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

}
