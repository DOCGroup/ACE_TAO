// $Id$

#include "tao/Environment.h"

ACE_INLINE ACE_Lock &
TAO_POA_Manager::lock (void)
{
  return this->lock_;
}

ACE_INLINE void
TAO_POA_Manager::activate (CORBA_Environment &ACE_TRY_ENV)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_OBJECT_ADAPTER_GUARD;

  this->activate_i (ACE_TRY_ENV);
}

#if (TAO_HAS_MINIMUM_POA == 0)

ACE_INLINE void
TAO_POA_Manager::hold_requests (CORBA::Boolean wait_for_completion,
                                CORBA_Environment &ACE_TRY_ENV)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_OBJECT_ADAPTER_GUARD;

  this->hold_requests_i (wait_for_completion, ACE_TRY_ENV);
}

ACE_INLINE void
TAO_POA_Manager::discard_requests (CORBA::Boolean wait_for_completion,
                                   CORBA_Environment &ACE_TRY_ENV)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_OBJECT_ADAPTER_GUARD;

  this->discard_requests_i (wait_for_completion, ACE_TRY_ENV);
}

ACE_INLINE void
TAO_POA_Manager::deactivate (CORBA::Boolean etherealize_objects,
                             CORBA::Boolean wait_for_completion,
                             CORBA_Environment &ACE_TRY_ENV)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_OBJECT_ADAPTER_GUARD;

  this->deactivate_i (etherealize_objects,
                      wait_for_completion,
                      ACE_TRY_ENV);
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

ACE_INLINE PortableServer::POAManager::State
TAO_POA_Manager::get_state_i (void)
{
  return this->state_;
}

ACE_INLINE PortableServer::POAManager::State
TAO_POA_Manager::get_state (CORBA::Environment &ACE_TRY_ENV)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_OBJECT_ADAPTER_GUARD_RETURN (this->state_);

  return this->get_state_i ();
}
