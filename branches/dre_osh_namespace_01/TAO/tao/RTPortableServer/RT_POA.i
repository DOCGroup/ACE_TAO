// $Id$

// Exception macros
#include "tao/PortableServer/poa_macros.h"
#include "tao/Environment.h"

ACE_INLINE CORBA::Object_ptr
TAO_RT_POA::create_reference_with_priority (const char * intf,
                                            RTCORBA::Priority priority
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  // Check that this method can be used based on this POA's policies.
  this->validate_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  this->validate_priority (priority ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->create_reference_i (intf,
                                   priority
                                   ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE CORBA::Object_ptr
TAO_RT_POA::create_reference_with_id_and_priority (const PortableServer::ObjectId & oid,
                                                   const char * intf,
                                                   RTCORBA::Priority priority
                                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  // Check that this method can be used based on this POA's policies.
  this->validate_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  this->validate_priority (priority ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->create_reference_with_id_i (oid,
                                           intf,
                                           priority
                                           ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE PortableServer::ObjectId *
TAO_RT_POA::activate_object_with_priority (PortableServer::Servant servant,
                                           RTCORBA::Priority priority
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::WrongPolicy))
{
  // Check that this method can be used based on this POA's policies.
  this->validate_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  this->validate_priority (priority ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  while (1)
    {
      int wait_occurred_restart_call = 0;      

      // Lock access for the duration of this transaction.
      TAO_POA_GUARD_RETURN (0);
      
      PortableServer::ObjectId *result =
        this->activate_object_i (servant,
                                 priority,
                                 wait_occurred_restart_call
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // If we ended up waiting on a condition variable, the POA state
      // may have changed while we are waiting.  Therefore, we need to
      // restart this call.
      if (wait_occurred_restart_call)
        continue;
      else
        return result;
    }
}

ACE_INLINE void
TAO_RT_POA::activate_object_with_id_and_priority (const PortableServer::ObjectId & oid,
                                                  PortableServer::Servant servant,
                                                  RTCORBA::Priority priority
                                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::ObjectAlreadyActive,
                   PortableServer::POA::WrongPolicy))
{
  // Check that this method can be used based on this POA's policies.
  this->validate_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->validate_priority (priority ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  while (1)
    {
      int wait_occurred_restart_call = 0;      
      
      // Lock access for the duration of this transaction.
      TAO_POA_GUARD;
      
      this->activate_object_with_id_i (oid,
                                       servant,
                                       priority,
                                       wait_occurred_restart_call
                                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // If we ended up waiting on a condition variable, the POA state
      // may have changed while we are waiting.  Therefore, we need to
      // restart this call.
      if (wait_occurred_restart_call)
        continue;
      else
        return;
    }
}

ACE_INLINE void *
TAO_RT_POA::thread_pool (void) const
{
  return this->thread_pool_;
}
