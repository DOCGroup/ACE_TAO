// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::SSLIOP::State_Guard::State_Guard (
  TAO::SSLIOP::Connection_Handler *handler,
  int &result)
  : handler_ (handler),
    previous_current_impl_ (0),
    current_impl_ (),
    setup_done_ (false)
{
  // Set up the SSLIOP::Current object.
  result = this->handler_->setup_ssl_state (this->previous_current_impl_,
                                            &(this->current_impl_),
                                            this->setup_done_);
}

ACE_INLINE
TAO::SSLIOP::State_Guard::~State_Guard (void)
{
  this->handler_->teardown_ssl_state (this->previous_current_impl_,
                                      this->setup_done_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
