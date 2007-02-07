// $Id$

namespace Kokyu
{

ACE_INLINE
int Dispatcher_Impl::init (const Dispatcher_Attributes& attr)
{
  return init_i (attr);
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

ACE_INLINE
int Dispatcher_Impl::activate ()
{
  return activate_i ();
}

}
