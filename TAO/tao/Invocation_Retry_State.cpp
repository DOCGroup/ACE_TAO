// -*- C++ -*-
//
// $Id$

#include "tao/Invocation_Retry_State.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::Invocation_Retry_State::Invocation_Retry_State (TAO_Stub &stub)
  : stub_(stub)
  , retry_params_(stub.orb_core ()->orb_params ()->invocation_retry_params ())
  , forward_on_exception_limit_used_(false)
{
  this->stub_.invocation_retry_state (this);
  this->ex_count_map_[FOE_OBJECT_NOT_EXIST] = 0;
  this->ex_count_map_[FOE_COMM_FAILURE] = 0;
  this->ex_count_map_[FOE_TRANSIENT] = 0;
  this->ex_count_map_[FOE_INV_OBJREF] = 0;

  for (Invocation_Retry_Params::exception_limit_map_type::const_iterator i =
         this->retry_params_.forward_on_exception_limit_.begin();
       i != this->retry_params_.forward_on_exception_limit_.end(); ++i)
    {
      if (i->second > 0)
        {
          forward_on_exception_limit_used_ = true;
          break;
        }
    }
}

bool
TAO::Invocation_Retry_State::forward_on_exception_limit_used () const
{
  return forward_on_exception_limit_used_;
}

bool
TAO::Invocation_Retry_State::forward_on_exception_increment (const int ef)
{
  if (!this->forward_on_exception_limit_used_)
    return false;

  int count = this->ex_count_map_[ef];
  Invocation_Retry_Params::exception_limit_map_type::const_iterator i =
    this->retry_params_.forward_on_exception_limit_.find (ef);
  int limit = i->second;
  if (count < limit)
    {
      this->ex_count_map_[ef] = count + 1;
      return true;
    }

  return false;
}

void
TAO::Invocation_Retry_State::sleep_before_retry ()
{
  // Avoid delay while iterating through a cycle of profiles.
  if (stub_.at_starting_profile ())
    ACE_OS::sleep (this->retry_params_.init_retry_delay_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
