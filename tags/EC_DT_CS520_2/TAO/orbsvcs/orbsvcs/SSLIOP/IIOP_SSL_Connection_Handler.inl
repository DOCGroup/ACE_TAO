// -*- C++ -*-
//
// $Id$

ACE_INLINE
TAO_Null_SSL_State_Guard::TAO_Null_SSL_State_Guard (
  TAO_SSLIOP_Current_ptr current,
  int &result)
  : previous_current_impl_ (0),
    current_ (current), // No need to duplicate
    setup_done_ (0)
{
  // Invalidate the TSS SSL session state to make sure that SSL state
  // from a previous SSL connection is not confused with this non-SSL
  // connection.
  current->setup (this->previous_current_impl_,
                  0, // Null SSL state
                  this->setup_done_);

  result = 0;
}

ACE_INLINE
TAO_Null_SSL_State_Guard::~TAO_Null_SSL_State_Guard (void)
{
  // Restore the previous TSS SSL state.
  this->current_->teardown (this->previous_current_impl_,
                            this->setup_done_);
}
