// $Id$

ACE_INLINE void
TAO_Asynch_Reply_Dispatcher_Base::transport (TAO_Transport *t)
{
  if (this->transport_ != 0) {
    this->transport_->remove_reference ();
  }

  this->transport_ = t;
  this->transport_->add_reference ();
}


