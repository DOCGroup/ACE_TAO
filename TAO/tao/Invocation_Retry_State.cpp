// -*- C++ -*-
//
// $Id$

#include "tao/Invocation_Retry_State.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace
{
  void retry_limit_calc (int ex,
                         TAO::Invocation_Retry_Params &command_line_params,
                         TAO::Invocation_Retry_Params &client_factory_params,
                         TAO::Invocation_Retry_Params &result)
  {
    if (command_line_params.forward_on_exception_limit_[ex] !=
        result.forward_on_exception_limit_[ex])
      result.forward_on_exception_limit_[ex] =
        command_line_params.forward_on_exception_limit_[ex];
    else if (client_factory_params.forward_on_exception_limit_[ex] !=
             result.forward_on_exception_limit_[ex])
      result.forward_on_exception_limit_[ex] =
        client_factory_params.forward_on_exception_limit_[ex];
  }

  /// Calculate the retry parameters by giving a command line parameter
  /// precedence over the corresponding client strategy factory parameter.
  /// result is assumed to be passed with default values
  void retry_params_calc (TAO::Invocation_Retry_Params &command_line_params,
                          TAO::Invocation_Retry_Params &client_factory_params,
                          TAO::Invocation_Retry_Params &result)
  {

  // Retry delay
  if (command_line_params.init_retry_delay_ !=
      result.init_retry_delay_)
    result.init_retry_delay_ = command_line_params.init_retry_delay_;
  else if (client_factory_params.init_retry_delay_ !=
      result.init_retry_delay_)
    result.init_retry_delay_ = client_factory_params.init_retry_delay_;

  // Retry on reply closed limit
  if (command_line_params.forward_on_reply_closed_limit_ !=
      result.forward_on_reply_closed_limit_)
    result.forward_on_reply_closed_limit_ =
      command_line_params.forward_on_reply_closed_limit_;
  else if (client_factory_params.forward_on_reply_closed_limit_ !=
           result.forward_on_reply_closed_limit_)
    result.forward_on_reply_closed_limit_ =
      client_factory_params.forward_on_reply_closed_limit_;

  // Forward on exception limits

  retry_limit_calc (TAO::FOE_OBJECT_NOT_EXIST,
                    command_line_params,
                    client_factory_params,
                    result);

  retry_limit_calc (TAO::FOE_COMM_FAILURE,
                    command_line_params,
                    client_factory_params,
                    result);

  retry_limit_calc (TAO::FOE_TRANSIENT,
                    command_line_params,
                    client_factory_params,
                    result);

  retry_limit_calc (TAO::FOE_INV_OBJREF,
                    command_line_params,
                    client_factory_params,
                    result);

  }

}

TAO::Invocation_Retry_State::Invocation_Retry_State (TAO_Stub &stub)
  : forward_on_reply_closed_count_ (0)
  , forward_on_exception_limit_used_ (false)
{
  this->ex_count_map_[FOE_OBJECT_NOT_EXIST] = 0;
  this->ex_count_map_[FOE_COMM_FAILURE] = 0;
  this->ex_count_map_[FOE_TRANSIENT] = 0;
  this->ex_count_map_[FOE_INV_OBJREF] = 0;

  // Cast away const to avoid tedious iterator operations on the ACE_Array_Map.
  TAO::Invocation_Retry_Params &command_line_params =
    const_cast<TAO::Invocation_Retry_Params &> (stub.orb_core ()
      ->orb_params ()->invocation_retry_params ());
  TAO::Invocation_Retry_Params &client_factory_params =
    const_cast<TAO::Invocation_Retry_Params &> (stub.orb_core ()
      ->client_factory ()->invocation_retry_params ());

  retry_params_calc(command_line_params,
                    client_factory_params,
                    this->retry_params_);

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

TAO::Invocation_Retry_State::~Invocation_Retry_State ()
{
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

bool
TAO::Invocation_Retry_State::forward_on_reply_closed_increment ()
{
  if (this->forward_on_reply_closed_count_ <
      this->retry_params_.forward_on_reply_closed_limit_)
    {
      ++this->forward_on_reply_closed_count_;
      return true;
    }

  return false;
}

void
TAO::Invocation_Retry_State::next_profile_retry (TAO_Stub &stub) const
{
  if (!stub.next_profile_retry ())
    {
      stub.reset_profiles ();
    }

  this->sleep_at_starting_profile (stub);
}

void
TAO::Invocation_Retry_State::sleep_at_starting_profile (TAO_Stub &stub) const
{
  if (stub.at_starting_profile ())
    this->sleep ();
}

void
TAO::Invocation_Retry_State::sleep () const
{
  ACE_OS::sleep (this->retry_params_.init_retry_delay_);
}


TAO_END_VERSIONED_NAMESPACE_DECL
