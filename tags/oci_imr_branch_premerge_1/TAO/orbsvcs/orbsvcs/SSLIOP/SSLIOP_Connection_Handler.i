// -*- C++ -*-
//
// $Id$


ACE_INLINE
TAO_SSL_State_Guard::TAO_SSL_State_Guard (
  TAO_SSLIOP_Connection_Handler *handler,
  int &result)
  : handler_ (handler),
    previous_current_impl_ (0),
    current_impl_ (),
    setup_done_ (0)
{
  // Set up the SSLIOP::Current object.
  result = this->handler_->setup_ssl_state (this->previous_current_impl_,
                                            &(this->current_impl_),
                                            this->setup_done_);
}

ACE_INLINE
TAO_SSL_State_Guard::~TAO_SSL_State_Guard (void)
{
  this->handler_->teardown_ssl_state (this->previous_current_impl_,
                                      this->setup_done_);
}
