// $Id$

namespace Kokyu
{

ACE_INLINE
int Dispatcher_Impl::init (const ConfigInfoSet& config_info)
{
  return init_i (config_info);
}

ACE_INLINE
int Dispatcher_Impl::dispatch (const Dispatch_Command* cmd,
                               const QoSDescriptor& qos_info)
{
  return dispatch_i (cmd, qos_info);
}

ACE_INLINE
int Dispatcher_Impl::shutdown ()
{
  return shutdown_i ();
}

}
