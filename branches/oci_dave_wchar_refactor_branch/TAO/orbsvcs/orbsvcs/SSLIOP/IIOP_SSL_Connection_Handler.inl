// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::Null_SSL_State_Guard::Null_SSL_State_Guard (
  TAO::SSLIOP::Current_ptr current,
  int &result)
  : previous_current_impl_ (0),
    current_ (current), // No need to duplicate
    setup_done_ (false)
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
TAO::Null_SSL_State_Guard::~Null_SSL_State_Guard (void)
{
  // Restore the previous TSS SSL state.
  this->current_->teardown (this->previous_current_impl_,
                            this->setup_done_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
