// $Id$

ACE_INLINE void
TAO_Asynch_Reply_Dispatcher_Base::transport (TAO_Transport *t)
{
  if (this->transport_ != 0) {
    TAO_Transport::release (this->transport_);
  }
  this->transport_ = TAO_Transport::_duplicate (t);
}


