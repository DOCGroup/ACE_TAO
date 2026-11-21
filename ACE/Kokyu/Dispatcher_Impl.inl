namespace Kokyu
{

ACE_INLINE
int Dispatcher_Impl::init (Dispatcher_Attributes const &attr)
{
  return init_i (attr);
}

ACE_INLINE
int Dispatcher_Impl::dispatch (Dispatch_Command const *cmd,
                               QoSDescriptor const &qos_info)
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
