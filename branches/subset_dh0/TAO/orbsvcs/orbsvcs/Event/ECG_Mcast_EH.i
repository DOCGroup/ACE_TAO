/* -*- C++ -*- */
// $Id$

ACE_INLINE
TAO_ECG_Mcast_EH::
Observer_Disconnect_Command::Observer_Disconnect_Command (void)
  : handle_ ()
  , ec_ ()
{
}

ACE_INLINE
TAO_ECG_Mcast_EH::
Observer_Disconnect_Command::Observer_Disconnect_Command (
                                 RtecEventChannelAdmin::Observer_Handle handle,
                                 RtecEventChannelAdmin::EventChannel_ptr ec)
  : handle_ (handle)
  , ec_ (RtecEventChannelAdmin::EventChannel::_duplicate (ec))
{
}

ACE_INLINE
TAO_ECG_Mcast_EH::
Observer_Disconnect_Command::Observer_Disconnect_Command (
                          const Observer_Disconnect_Command & rhs)
  : handle_ (rhs.handle_)
  , ec_ (RtecEventChannelAdmin::EventChannel::_duplicate (rhs.ec_.in()))
{
}

ACE_INLINE
TAO_ECG_Mcast_EH::Observer_Disconnect_Command &
TAO_ECG_Mcast_EH::
Observer_Disconnect_Command::operator= (
                         const Observer_Disconnect_Command & rhs)
{
  this->handle_ = rhs.handle_;
  this->ec_ = RtecEventChannelAdmin::EventChannel::_duplicate (rhs.ec_.in());

  return *this;
}

ACE_INLINE void
TAO_ECG_Mcast_EH::
Observer_Disconnect_Command::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->ec_.in ()))
    // We are not connected.
    return;

  RtecEventChannelAdmin::EventChannel_var release_ec =
    this->ec_._retn ();

  release_ec->remove_observer (this->handle_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

//***************************************************************************
