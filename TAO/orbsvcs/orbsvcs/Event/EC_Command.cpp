// $Id$

#ifndef TAO_EC_COMMAND_CPP
#define TAO_EC_COMMAND_CPP

#include "EC_Command.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Command.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Command, "$Id$")

template<class Target, class Object> int
TAO_EC_Connected_Command<Target,Object>::execute (void* arg)
{
  CORBA::Environment *env = &CORBA::default_environment ();
  if (arg != 0)
    env = ACE_dynamic_cast(CORBA::Environment*, arg);

  this->target_->connected_i (this->object_, *env);
  return 0;
}

// ****************************************************************

template<class Target, class Object> int
TAO_EC_Disconnected_Command<Target,Object>::execute (void* arg)
{
  CORBA::Environment *env = &CORBA::default_environment ();
  if (arg != 0)
    env = ACE_dynamic_cast(CORBA::Environment*, arg);

  this->target_->disconnected_i (this->object_, *env);
  return 0;
}

#endif /* TAO_EC_COMMAND_CPP */
