// $Id$

#ifndef TAO_ESF_DELAYED_COMMAND_CPP
#define TAO_ESF_DELAYED_COMMAND_CPP

#include "orbsvcs/ESF/ESF_Delayed_Command.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Delayed_Command.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class Target, class Object> int
TAO_ESF_Connected_Command<Target,Object>::execute (void* /*arg*/)
{
  this->target_->connected_i (this->object_);
  return 0;
}

// ****************************************************************

template<class Target, class Object> int
TAO_ESF_Reconnected_Command<Target,Object>::execute (void* /*arg*/)
{
  this->target_->reconnected_i (this->object_);
  return 0;
}

// ****************************************************************

template<class Target, class Object> int
TAO_ESF_Disconnected_Command<Target,Object>::execute (void* /*arg*/)
{
  this->target_->disconnected_i (this->object_);
  return 0;
}

// ****************************************************************

template<class Target> int
TAO_ESF_Shutdown_Command<Target>::execute (void* /*arg*/)
{
  this->target_->shutdown_i ();
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ESF_DELAYED_COMMAND_CPP */
