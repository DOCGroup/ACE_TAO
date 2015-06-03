// -*- C++ -*-
#include "tao/SystemException.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE ACE_Lock &
TAO_POA_Manager::lock (void)
{
  return this->lock_;
}

ACE_INLINE void
TAO_POA_Manager::activate (void)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_OBJECT_ADAPTER_GUARD;

  this->activate_i ();
}

#if (TAO_HAS_MINIMUM_POA == 0)

ACE_INLINE void
TAO_POA_Manager::hold_requests (CORBA::Boolean wait_for_completion)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_OBJECT_ADAPTER_GUARD;

  this->hold_requests_i (wait_for_completion);
}

ACE_INLINE void
TAO_POA_Manager::discard_requests (CORBA::Boolean wait_for_completion)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_OBJECT_ADAPTER_GUARD;

  this->discard_requests_i (wait_for_completion);
}

ACE_INLINE void
TAO_POA_Manager::deactivate (CORBA::Boolean etherealize_objects,
                             CORBA::Boolean wait_for_completion)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_OBJECT_ADAPTER_GUARD;

  this->deactivate_i (etherealize_objects, wait_for_completion);
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

ACE_INLINE PortableServer::POAManager::State
TAO_POA_Manager::get_state_i (void)
{
  return this->state_;
}

ACE_INLINE PortableServer::POAManager::State
TAO_POA_Manager::get_state (void)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_OBJECT_ADAPTER_GUARD_RETURN (this->state_);

  return this->get_state_i ();
}

ACE_INLINE char*
TAO_POA_Manager::generate_manager_id (void) const
{
  // The AdapterManagerId must be unique across all Adapter Managers
  // (e.g. POAManagers) within a given process.  To avoid locking
  // overhead, the address of the POAManager object is used as the
  // AdapterManagerId.  This guarantees that the AdapterManagerId is
  // unique.
  //
  // For 64-bit platforms, only the lower 32 bits are used.  Hopefully
  // that will be enough to ensure uniqueness.

  // This is basically the same trick used in
  // TAO_GIOP_Invocation::generate_request_id().  However, no right
  // shifting of 64 bit addresses is performed since the
  // TAO_POA_Manager object is not large enough to allow that trick.

  CORBA::Long id = 0;

  // Note that we reinterpret_cast to an "unsigned long" instead of
  // CORBA::ULong since we need to first cast to an integer large
  // enough to hold an address to avoid compile-time warnings on some
  // 64-bit platforms.

  if (sizeof (this) == 4)       // 32 bit address
    id = static_cast<CORBA::Long> (reinterpret_cast <ptrdiff_t> (this));

  else if (sizeof (this) == 8)  // 64 bit address -- use lower 32 bits
    id = static_cast<CORBA::Long> (reinterpret_cast <ptrdiff_t> (this) & 0xFFFFFFFFu);

  // @@ If we ever hit a platform where neither of the above cases are
  //    satisfied, we're up the creek!

//   else
//     // Fallback on an atomically incremented variable specific to the
//     // ORB, or perhaps specific to the process.
//     id = ...GENERATE_ID_ATOMICALLY...;  // Fallback

  char* buf = new char [25];
  ACE_OS::sprintf (buf, "POAManager%d", id);
  return buf;
}


#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

ACE_INLINE
CORBA::PolicyList& TAO_POA_Manager::get_policies (void)
{
  return this->policies_;
}

#endif


TAO_END_VERSIONED_NAMESPACE_DECL
