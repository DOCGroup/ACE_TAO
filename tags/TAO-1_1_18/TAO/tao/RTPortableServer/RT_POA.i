// $Id$

// Exception macros
#include "tao/PortableServer/poa_macros.h"
#include "tao/Environment.h"

ACE_INLINE CORBA::Object_ptr
TAO_RT_POA::create_reference_with_priority (const char * intf,
                                         RTCORBA::Priority priority,
                                         CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  // Check that this method can be used based on this POA's policies.
  this->validate_policies (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  this->valid_priority (priority, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->create_reference_i (intf,
                                   priority,
                                   ACE_TRY_ENV);
}

ACE_INLINE CORBA::Object_ptr
TAO_RT_POA::create_reference_with_id_and_priority (const PortableServer::ObjectId & oid,
                                                const char * intf,
                                                RTCORBA::Priority priority,
                                                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::WrongPolicy))
{
  // Check that this method can be used based on this POA's policies.
  this->validate_policies (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  this->valid_priority (priority, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->create_reference_with_id_i (oid,
                                           intf,
                                           priority,
                                           ACE_TRY_ENV);
}

ACE_INLINE PortableServer::ObjectId *
TAO_RT_POA::activate_object_with_priority (PortableServer::Servant servant,
                                        RTCORBA::Priority priority,
                                        CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::WrongPolicy))
{
  // Check that this method can be used based on this POA's policies.
  this->validate_policies (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  this->valid_priority (priority, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->activate_object_i (servant,
                                  priority,
                                  ACE_TRY_ENV);
}

ACE_INLINE void
TAO_RT_POA::activate_object_with_id_and_priority (const PortableServer::ObjectId & oid,
                                               PortableServer::Servant servant,
                                               RTCORBA::Priority priority,
                                               CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::POA::ServantAlreadyActive,
                   PortableServer::POA::ObjectAlreadyActive,
                   PortableServer::POA::WrongPolicy))
{
  // Check that this method can be used based on this POA's policies.
  this->validate_policies (ACE_TRY_ENV);
  ACE_CHECK;

  this->valid_priority (priority, ACE_TRY_ENV);
  ACE_CHECK;

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->activate_object_with_id_i (oid,
                                   servant,
                                   priority,
                                   ACE_TRY_ENV);
}

