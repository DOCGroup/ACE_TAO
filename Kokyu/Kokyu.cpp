// $Id$

#include "Kokyu.h"

#include "Default_Dispatcher_Impl.h"

#if ! defined (__ACE_INLINE__)
#include "Kokyu.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Kokyu, Kokyu, "$Id$")

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
  auto_ptr<Dispatcher_Impl> tmp_impl (impl);
  dispatcher_impl_ = tmp_impl;

  //I couldn't use reset because MSVC++ auto_ptr does not have reset method.
  //So in configurations where the auto_ptr maps to the std::auto_ptr instead
  //of ACE auto_ptr, this would be a problem.
  //dispatcher_impl_.reset (impl);
}

Dispatcher_Auto_Ptr
Dispatcher_Factory::
create_dispatcher(const ConfigInfoSet& config_info_set)
{
  Dispatcher* disp;
  Dispatcher_Auto_Ptr nil_ptr((Dispatcher*)0);
  Dispatcher_Impl* tmp;
  ACE_NEW_RETURN (tmp, Default_Dispatcher_Impl, nil_ptr);
  ACE_NEW_RETURN (disp, Dispatcher, nil_ptr);
  Dispatcher_Auto_Ptr disp_auto_ptr(disp);
  disp->implementation (tmp);
  tmp->init (config_info_set);
  return disp_auto_ptr;
}

}
