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

template<class SERVANT>
Servant_var<SERVANT>::~Servant_var ()
{
  if (this->ptr_ == 0)
    return;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY {
    this->ptr_->_remove_ref (ACE_TRY_ENV);
    ACE_TRY_CHECK;
  } ACE_CATCHANY {
    // @@ This event should be logged...
  } ACE_ENDTRY;
}

#endif /* CS_SERVANT_VAR_CPP */
