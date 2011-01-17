// -*- C++ -*-
//
// $Id$

// Exception macros
#include "tao/PortableServer/poa_macros.h"
#include "tao/PortableServer/POA_Guard.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::Object_ptr
TAO_RT_POA::create_reference_with_priority (const char * intf,
                                            RTCORBA::Priority priority)
{
  // Check that this method can be used based on this POA's policies.
  this->validate_policies ();

  this->validate_priority (priority);

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->create_reference_i (intf, priority);
}

#if !defined (CORBA_E_MICRO)
ACE_INLINE CORBA::Object_ptr
TAO_RT_POA::create_reference_with_id_and_priority (const PortableServer::ObjectId & oid,
                                                   const char * intf,
                                                   RTCORBA::Priority priority)
{
  // Check that this method can be used based on this POA's policies.
  this->validate_policies ();

  this->validate_priority (priority);

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->create_reference_with_id_i (oid, intf, priority);
}
#endif

ACE_INLINE PortableServer::ObjectId *
TAO_RT_POA::activate_object_with_priority (PortableServer::Servant servant,
                                           RTCORBA::Priority priority)
{
  // Check that this method can be used based on this POA's policies.
  this->validate_policies ();

  this->validate_priority (priority);

  while (true)
    {
      bool wait_occurred_restart_call = false;

      // Lock access for the duration of this transaction.
      TAO_POA_GUARD_RETURN (0);

      PortableServer::ObjectId *result =
        this->activate_object_i (servant, priority, wait_occurred_restart_call);

      // If we ended up waiting on a condition variable, the POA state
      // may have changed while we are waiting.  Therefore, we need to
      // restart this call.
      if (wait_occurred_restart_call)
        continue;
      else
        return result;
    }
}

#if !defined (CORBA_E_MICRO)
ACE_INLINE void
TAO_RT_POA::activate_object_with_id_and_priority (const PortableServer::ObjectId & oid,
                                                  PortableServer::Servant servant,
                                                  RTCORBA::Priority priority)
{
  // Check that this method can be used based on this POA's policies.
  this->validate_policies ();

  this->validate_priority (priority);

  while (true)
    {
      bool wait_occurred_restart_call = false;

      // Lock access for the duration of this transaction.
      TAO_POA_GUARD;

      this->activate_object_with_id_i (oid,
                                       servant,
                                       priority,
                                       wait_occurred_restart_call);

      // If we ended up waiting on a condition variable, the POA state
      // may have changed while we are waiting.  Therefore, we need to
      // restart this call.
      if (wait_occurred_restart_call)
        continue;
      else
        return;
    }
}
#endif

ACE_INLINE void *
TAO_RT_POA::thread_pool (void) const
{
  return this->thread_pool_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
