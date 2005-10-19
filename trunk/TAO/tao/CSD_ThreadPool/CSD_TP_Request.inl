// $Id$


ACE_INLINE
TAO::CSD::TP_Request::TP_Request(PortableServer::Servant servant,
                                 TP_Servant_State*       servant_state)
  : prev_(0),
    next_(0),
    servant_ (servant),
    servant_state_(servant_state, false)
{
  // This try-catch block is not really necessary for current implementation 
  // since the _add_ref does not throw exception, but we have to add it to 
  // satisfy the non-exception builds. If _add_ref really throws an exception 
  // then this constructor needs deal with the exception.
  ACE_TRY_NEW_ENV
  {
    this->servant_->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHALL
  {
  }
  ACE_ENDTRY;
}


ACE_INLINE
void
TAO::CSD::TP_Request::prepare_for_queue()
{
  this->prepare_for_queue_i();
}


ACE_INLINE
PortableServer::Servant
TAO::CSD::TP_Request::servant()
{
  // Used for chaining so we do not return a new "copy".
  return this->servant_.in();
}


ACE_INLINE
bool
TAO::CSD::TP_Request::is_ready() const
{
  return !this->servant_state_->busy_flag();
}


ACE_INLINE
void
TAO::CSD::TP_Request::mark_as_busy()
{
  this->servant_state_->busy_flag(true);
}


ACE_INLINE
void
TAO::CSD::TP_Request::mark_as_ready()
{
  this->servant_state_->busy_flag(false);
}


ACE_INLINE
bool 
TAO::CSD::TP_Request::is_target(PortableServer::Servant servant)
{
  // Compare pointers.  Return true only if these are the exact same object.
  return (servant == this->servant_.in());
}


ACE_INLINE
void
TAO::CSD::TP_Request::dispatch()
{
  this->dispatch_i();
  
}


ACE_INLINE
void
TAO::CSD::TP_Request::cancel()
{
  this->cancel_i();
}
