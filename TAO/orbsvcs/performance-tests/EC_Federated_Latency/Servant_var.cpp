/**
 * @file Servant_var.cpp
 *
 * $Id$
 *
 * @author Jody Hagins <jody@atdesk.com>
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#ifndef CS_SERVANT_VAR_CPP
#define CS_SERVANT_VAR_CPP

#include "Servant_var.h"

#if !defined(__ACE_INLINE__)
#include "Servant_var.inl"
#endif /* __ACE_INLINE__ */

template<typename SERVANT>
Servant_var<SERVANT>::~Servant_var ()
{
  if (this->ptr_ == 0)
    return;

  try {
    this->ptr_->_remove_ref ();
  } catch (...) {
    // @@ This event should be logged...
  }
}

#endif /* CS_SERVANT_VAR_CPP */
