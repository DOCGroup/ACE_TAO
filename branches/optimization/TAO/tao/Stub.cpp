// $Id$

// @ (#)iiopobj.cpp     1.9 95/11/04
// Copyright 1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// XXXX Bridge:         CORBA::Object operations
//
// Some CORBA::Object and other operations are specific to this STUB
// based implementation, and can neither be used by other kinds of
// objref nor have a default implementation.

#include "Endpoint.h"
#include "Stub.h"
#include "Sequence.h"
#include "Object.h"
#include "Invocation.h"
#include "Asynch_Invocation.h"
#include "ORB_Core.h"
#include "Client_Strategy_Factory.h"
#include "Sync_Strategies.h"
#include "Buffering_Constraint_Policy.h"
#include "Messaging_Policy_i.h"
#include "Client_Priority_Policy.h"
#include "debug.h"

#if (TAO_HAS_RT_CORBA == 1)
# include "RT_Policy_i.h"
#endif /* TAO_HAS_RT_CORBA == 1 */

#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
# include "Stub.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, TAO_Stub, "$Id$")

TAO_Stub::TAO_Stub (const char *repository_id,
                    const TAO_MProfile &profiles,
                    TAO_ORB_Core *orb_core)
  : type_id (repository_id),
#if (TAO_HAS_RT_CORBA == 1)
    priority_model_policy_ (0),
    priority_banded_connection_policy_ (0),
    client_protocol_policy_ (0),
    are_policies_parsed_ (0),
#endif /* TAO_HAS_RT_CORBA == 1 */
    base_profiles_ ((CORBA::ULong) 0),
    forward_profiles_ (0),
    profile_in_use_ (0),
    profile_lock_ptr_ (0),
    profile_success_ (0),
    refcount_lock_ (),
    refcount_ (1),
    orb_core_ (orb_core),
    orb_ (),
    servant_orb_ (),
#if (TAO_HAS_CORBA_MESSAGING == 1)
    policies_ (0),
#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
    addressing_mode_ (0)
{
  if (this->orb_core_ == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO: (%P|%t) TAO_Stub created with default ")
                      ACE_TEXT ("ORB core\n")));
        }
      this->orb_core_ = TAO_ORB_Core_instance ();
    }

  // Duplicate the ORB_Core, otherwise the allocators and other
  // resources that this class references (directly or indirectly)
  // could be destroyed before it is time.
  (void) this->orb_core_->_incr_refcnt ();

  // Cache the ORB pointer to respond faster to certain queries.
  this->orb_ = CORBA::ORB::_duplicate (this->orb_core_->orb ());

  this->profile_lock_ptr_ =
    this->orb_core_->client_factory ()->create_profile_lock ();

  this->base_profiles (profiles);
  TAO_Profile *profile = 0;
  for (CORBA::ULong i = 0; i < this->base_profiles_.profile_count (); ++i)
    {
      // Get the ith profile
      profile = this->base_profiles_.get_profile (i);
      profile->the_stub (this);
    }
}

TAO_Stub::~TAO_Stub (void)
{
  ACE_ASSERT (this->refcount_ == 0);

  if (this->forward_profiles_)
    reset_profiles ();

  if (this->profile_in_use_ != 0)
    {
      if (this->orb_->orb_core () != 0)
        {
          // The hint in the profile is a hint for a client connection
          // handler.  If the ORB core doesn't exist, perhaps due to
          // it being destroy()ed, then no connectors exist so do not
          // reset the hint in case it points to non-existent
          // connection handler.
          this->profile_in_use_->endpoint ()->reset_hint ();
        }

      // decrease reference count on profile
      this->profile_in_use_->_decr_refcnt ();
      this->profile_in_use_ = 0;
    }

  delete this->profile_lock_ptr_;

#if (TAO_HAS_CORBA_MESSAGING == 1)

  delete this->policies_;

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  this->orb_core_->_decr_refcnt ();


#if (TAO_HAS_RT_CORBA == 1)

  if (this->priority_model_policy_)
    this->priority_model_policy_->destroy ();

  if (this->priority_banded_connection_policy_)
    this->priority_banded_connection_policy_->destroy ();

  if (this->client_protocol_policy_)
    this->client_protocol_policy_->destroy ();

#endif /* TAO_HAS_RT_CORBA */
}

void
TAO_Stub::add_forward_profiles (const TAO_MProfile &mprofiles)
{
  // we assume that the profile_in_use_ is being
  // forwarded!  Grab the lock so things don't change.
  ACE_MT (ACE_GUARD (ACE_Lock,
                     guard,
                     *this->profile_lock_ptr_));

  TAO_MProfile *now_pfiles = this->forward_profiles_;
  if (now_pfiles == 0)
    now_pfiles = &this->base_profiles_;

  ACE_NEW (this->forward_profiles_,
           TAO_MProfile (mprofiles));

  // forwarded profile points to the new IOR (profiles)
  this->profile_in_use_->forward_to (this->forward_profiles_);

  // new profile list points back to the list which was forwarded.
  this->forward_profiles_->forward_from (now_pfiles);

  // make sure we start at the beginning of mprofiles
  this->forward_profiles_->rewind ();

  // Since we have been forwarded, we must set profile_success_ to 0
  // since we are starting a new with a new set of profiles!
  this->profile_success_ = 0;

  // Reset any flags that may be appropriate in the services that
  // selects profiles for invocation
  this->orb_core_->reset_service_profile_flags ();
}




// Quick'n'dirty hash of objref data, for partitioning objrefs into
// sets.
//
// NOTE that this must NOT go across the network!

// @@ Use all profiles for hash function!!!!!  FRED
//    can get different values, depending on the profile_in_use!!
CORBA::ULong
TAO_Stub::hash (CORBA::ULong max,
                CORBA::Environment &ACE_TRY_ENV)
{
  // we rely on the profile object that its address info
  if (profile_in_use_)
    return profile_in_use_->hash (max, ACE_TRY_ENV);

  if (TAO_debug_level > 3)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) hash called on a null profile\n")));

  ACE_THROW_RETURN (CORBA::INTERNAL (
                                     CORBA_SystemException::_tao_minor_code (
                                                                             TAO_DEFAULT_MINOR_CODE,
                                                                             0),
                                     CORBA::COMPLETED_NO),
                    0);
}

// Expensive comparison of objref data, to see if two objrefs
// certainly point at the same object. (It's quite OK for this to
// return FALSE, and yet have the two objrefs really point to the same
// object.)
//
// NOTE that this must NOT go across the network!
// @@ Two object references are the same if any two profiles are the
//    same! This function is only test the profile in use!!!
CORBA::Boolean
TAO_Stub::is_equivalent (CORBA::Object_ptr other_obj)
{
  if (CORBA::is_nil (other_obj) == 1)
    return 0;

  TAO_Profile *other_profile = other_obj->_stubobj ()->profile_in_use_;
  TAO_Profile *this_profile = this->profile_in_use_;

  if (other_profile == 0 || this_profile == 0)
    return 0;

  // Compare the profiles
  return this_profile->is_equivalent (other_profile);
}

// Memory managment

CORBA::ULong
TAO_Stub::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    guard,
                    this->refcount_lock_,
                    0);

  return this->refcount_++;
}

CORBA::ULong
TAO_Stub::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                      mon,
                      this->refcount_lock_,
                      0);

    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

// Note that if the repository ID (typeID) is NULL, it will make
// narrowing rather expensive, though it does ensure that type-safe
// narrowing code gets thoroughly exercised/debugged!  Without a
// typeID, the _narrow will be required to make an expensive remote
// "is_a" call.

// THREADING NOTE: Code below this point is of course thread-safe (at
// least on supported threaded platforms), so the caller of these
// routines need only ensure that the data being passed in is not
// being modified by any other thread.
//
// As an _experiment_ (to estimate the performance cost) remote calls
// are currently deemed "cancel-safe".  That means that they can be
// called by threads when they're in asynchronous cancellation mode.
// The only effective way to do this is to disable async cancellation
// for the duration of the call.  There are numerous rude interactions
// with code generators for C++ ... cancellation handlers just do
// normal stack unwinding like exceptions, but exceptions are purely
// synchronous and sophisticated code generators rely on that to
// generate better code, which in some cases may be very hard to
// unwind.

class TAO_Synchronous_Cancellation_Required
// = TITLE
//     Stick one of these at the beginning of a block that can't
//     support asynchronous cancellation, and which must be
//     cancel-safe.
//
// = EXAMPLE
//     somefunc()
//     {
//       TAO_Synchronous_Cancellation_Required NOT_USED;
//       ...
//     }
{
public:
  // These should probably be in a separate inline file, but they're
  // only used within this one file right now, and we always want them
  // inlined, so here they sit.
  TAO_Synchronous_Cancellation_Required (void)
    : old_type_ (0)
  {
#if !defined (VXWORKS)
    ACE_OS::thr_setcanceltype (THR_CANCEL_DEFERRED, &old_type_);
#endif /* ! VXWORKS */
  }

  ~TAO_Synchronous_Cancellation_Required (void)
  {
#if !defined (VXWORKS)
    int dont_care;
    ACE_OS::thr_setcanceltype(old_type_, &dont_care);
#endif /* ! VXWORKS */
  }
private:
  int old_type_;
};

// ****************************************************************

#if (TAO_HAS_CORBA_MESSAGING == 1)
#if (TAO_HAS_RT_CORBA == 1)

void
TAO_Stub::parse_policies (void)
{
  CORBA::PolicyList *policy_list
    = this->base_profiles_.policy_list ();

  CORBA::ULong length = policy_list->length ();
  //  CORBA::ULong index = 0;

  CORBA::ULong policy_type = 0;

  this->orb_core ()->get_protocols_hooks ()->call_policy_type_hook (policy_list,
                                                                    policy_type);

  for (unsigned int i = 0; i < length; ++i)
    {
      if (policy_type == 0)
        this->exposed_priority_model ((*policy_list)[i].in ());

      else if (policy_type == 1)
        this->exposed_priority_banded_connection ((*policy_list)[i].in ());

      else if (policy_type == 2)
        this->exposed_client_protocol ((*policy_list)[i].in ());
    }

  this->are_policies_parsed_ = 1;
}

CORBA::Policy *
TAO_Stub::exposed_priority_model (void)
{
  if (!this->are_policies_parsed_)
    this->parse_policies ();

  if (!CORBA::is_nil (this->priority_model_policy_))
    this->priority_model_policy_->_add_ref ();

  return this->priority_model_policy_;
}

void
TAO_Stub::exposed_priority_model (CORBA::Policy_ptr policy)
{
  if (!CORBA::is_nil (policy))
    {
      this->priority_model_policy_ = policy;
    }
}

CORBA::Policy *
TAO_Stub::exposed_priority_banded_connection (void)
{
  if (!this->are_policies_parsed_)
    this->parse_policies ();

  if (!CORBA::is_nil (this->priority_banded_connection_policy_))
    this->priority_banded_connection_policy_->_add_ref ();

  return this->priority_banded_connection_policy_;
}

void
TAO_Stub::exposed_priority_banded_connection (CORBA::Policy_ptr policy)
{
  if (!CORBA::is_nil (policy))
    {
      this->priority_banded_connection_policy_ =
        policy;
    }
}

CORBA::Policy *
TAO_Stub::exposed_client_protocol (void)
{
  if (!this->are_policies_parsed_)
    this->parse_policies ();

  if (!CORBA::is_nil (this->client_protocol_policy_))
    this->client_protocol_policy_->_add_ref ();

  return this->client_protocol_policy_;
}

void
TAO_Stub::exposed_client_protocol (CORBA::Policy_ptr policy)
{
  if (!CORBA::is_nil (policy))
    {
      this->client_protocol_policy_ = policy;
    }
}

#endif /* TAO_HAS_RT_CORBA == 1 */

CORBA::Policy_ptr
TAO_Stub::get_policy (CORBA::PolicyType type,
                      CORBA::Environment &ACE_TRY_ENV)
{
  // No need to lock, the stub only changes its policies at
  // construction time...

#if (TAO_HAS_RT_CORBA == 1)

  CORBA::ULong type_value = 0;

  // Validity check.  Make sure requested policy type is appropriate
  // for this scope.
  this->orb_core_->get_protocols_hooks ()->validate_policy_type (type,
                                                                 type_value,
                                                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // If we are dealing with a client exposed policy, check if any
  // value came in the IOR/reconcile IOR value and overrides.
  if (type_value == 0)
    return this->exposed_priority_model ();

  if (type_value == 1)
    return this->effective_priority_banded_connection (ACE_TRY_ENV);

  if (type_value == 2)
    return this->effective_client_protocol (ACE_TRY_ENV);

#endif /* TAO_HAS_RT_CORBA == 1 */

  if (this->policies_ == 0)
    return CORBA::Policy::_nil ();

  // Some policies can only be set locally on the client, while others
  // can only be exported in the IOR by the server, and yet others can
  // be set by both by client and server.  Furthermore, reconciliation
  // between client-set values and the ones exported in the IOR is
  // policy-specific.  Therefore, with the current state of things, it
  // isn't possible to write generic code for <get_policy> that will
  // work for any policy type.
  // Currently, we take specific action
  // for each of the known client-exposed policies (above), and simply
  // look up effective override for any other policy type (below).
  // Later, if there is a need/desire for generic code, it can be
  // done by pushing the smarts into the policies
  // implementations, and will involve modifying PolicyC* and friends,
  // e.g., to add methods for policy specific reconciliation, etc.

  return this->get_client_policy (type,
                                  ACE_TRY_ENV);
}

CORBA::Policy_ptr
TAO_Stub::get_client_policy (CORBA::PolicyType type,
                             CORBA::Environment &ACE_TRY_ENV)
{
  // No need to lock, the stub only changes its policies at
  // construction time...

#if (TAO_HAS_RT_CORBA == 1)

  CORBA::ULong type_value = 0;

  // Validity check.  Make sure requested policy type is appropriate
  // for this scope.
  this->orb_core_->get_protocols_hooks () ->validate_policy_type (type,
                                                                  type_value,
                                                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // @@ Here, an exception is to be thrown if type is one of the three
  // cases. The validate_policy_type
  // throws an exception for the first two policy types but here we
  // need to throw an excpetion for priority moel policy type too. So,
  // for software reuse, checking on the value of type_value to (or
  // not to) throw an exception. (Priyanka)
  if (type_value == 0)
    ACE_THROW_RETURN (CORBA::INV_POLICY (),
                      CORBA::Policy::_nil ());


#endif /* TAO_HAS_RT_CORBA == 1 */

  CORBA::Policy_var result;
  if (this->policies_ != 0)
    {
      result = this->policies_->get_policy (type,
                                            ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  if (CORBA::is_nil (result.in ()))
    {
      TAO_Policy_Current &policy_current = this->orb_core_->policy_current ();
      result = policy_current.get_policy (type,
                                          ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  if (CORBA::is_nil (result.in ()))
    {
      // @@ Must lock, but is is harder to implement than just modifying
      //    this call: the ORB does take a lock to modify the policy
      //    manager
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        {
          result = policy_manager->get_policy (type,
                                               ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::Policy::_nil ());
        }
    }

  if (CORBA::is_nil (result.in ()))
    {
      result = this->orb_core_->get_default_policy (type,
                                                    ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  return result._retn ();
}

TAO_Stub *
TAO_Stub::set_policy_overrides (const CORBA::PolicyList & policies,
                                CORBA::SetOverrideType set_add,
                                CORBA::Environment &ACE_TRY_ENV)
{
#if (TAO_HAS_RT_CORBA == 1)

  // Validity check.  Make sure requested policies are allowed to be
  // set at this scope.
  for (CORBA::ULong i = 0; i < policies.length ();  ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      if (CORBA::is_nil (policy))
        continue;

      CORBA::ULong slot = policy->policy_type (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      CORBA::ULong type_value = 0;

      // @@ Not sure if this is right.
      //    This method was throwing CORBA::NO_PERMISSION when the
      //    if statements (which are now in the validate_policy_type
      //    hook) were true. I am using the same hook method
      //    for two other methods which need to throw
      //    CORBA::INV_Policy (). So, in here, I am not checking the
      //    CORBA::Environment variable, but checking on the value of
      //    type_value to throw the right exception. - Priyanka
      this->orb_core_->get_protocols_hooks ()->validate_policy_type (slot,
                                                                     type_value,
                                                                     ACE_TRY_ENV);
      if (type_value == 0 | type_value == 3)
        ACE_THROW_RETURN (CORBA::NO_PERMISSION (), 0);
    }

  // We are not required to check for consistency of <policies> with
  // overrides at other levels or with policies exported in the IOR.

#endif /* TAO_HAS_RT_CORBA == 1 */

  // Notice the use of an explicit constructor....
  auto_ptr<TAO_Policy_Manager_Impl> policy_manager (new TAO_Policy_Manager_Impl);

  if (set_add == CORBA::SET_OVERRIDE)
    {
      policy_manager->set_policy_overrides (policies,
                                            set_add,
                                            ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }
  else if (this->policies_ == 0)
    {
      policy_manager->set_policy_overrides (policies,
                                            CORBA::SET_OVERRIDE,
                                            ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }
  else
    {
      policy_manager->copy_from (this->policies_,
                                 ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
      policy_manager->set_policy_overrides (policies,
                                            set_add,
                                            ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  TAO_Stub* stub;
  ACE_NEW_RETURN (stub, TAO_Stub (this->type_id.in (),
                                  this->base_profiles_,
                                  this->orb_core_),
                  0);
  stub->policies_ = policy_manager.release ();

  // Copy the servant ORB if it is present.
  stub->servant_orb (this->servant_orb_var ().in ());

  return stub;
}

CORBA::PolicyList *
TAO_Stub::get_policy_overrides (const CORBA::PolicyTypeSeq &types,
                                CORBA::Environment &ACE_TRY_ENV)
{
  if (this->policies_ == 0)
    return 0;

  return this->policies_->get_policy_overrides (types,
                                                ACE_TRY_ENV);
}

CORBA::Boolean
TAO_Stub::validate_connection (CORBA::PolicyList_out inconsistent_policies,
                               CORBA::Environment &ACE_TRY_ENV)
{
  // Use Locate Request to establish connection/make sure the object
  // is there ...
  TAO_GIOP_Locate_Request_Invocation locate_request (this,
                                                     this->orb_core_);

  locate_request.init_inconsistent_policies (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // @@ For some combinations of RTCORBA policies, location forwarding
  // isn't supported.  See <forward> method implementations in
  // Invocation_Endpoint_Selectors.cpp for more information.
  //
  ACE_TRY
    {
      for (;;)
        {
          locate_request.start (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          int status = locate_request.invoke (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // We'll get this only if the object was, in fact, forwarded.
          if (status == TAO_INVOKE_RESTART)
              continue;

          if (status != TAO_INVOKE_OK)
            {
              ACE_TRY_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                             CORBA::COMPLETED_YES));
            }
          break;
        }
    }
  ACE_CATCH (CORBA::INV_POLICY, ex)
    {
      inconsistent_policies =
        locate_request.get_inconsistent_policies ();
      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  return 1;
}

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

TAO_Sync_Strategy &
TAO_Stub::sync_strategy (void)
{
#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  int has_synchronization;
  int scope;

  this->orb_core_->call_sync_scope_hook (this,
                                         has_synchronization,
                                         scope);

  if (has_synchronization == 1)
    return this->orb_core_->get_sync_strategy (this,
                                               scope);

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

  return this->orb_core_->transport_sync_strategy ();
}

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

CORBA::Policy *
TAO_Stub::relative_roundtrip_timeout (void)
{
  CORBA::Policy *result = 0;

  this->policies_ = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
      result = this->policies_->relative_roundtrip_timeout ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.relative_roundtrip_timeout ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->relative_roundtrip_timeout ();
    }

  if (result == 0)
    result = this->orb_core_->default_relative_roundtrip_timeout ();

  return result;
}

#endif /* TAO_HAS_REALTIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

TAO_Client_Priority_Policy *
TAO_Stub::client_priority (void)
{
  TAO_Client_Priority_Policy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->client_priority ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.client_priority ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->client_priority ();
    }

  if (result == 0)
    result = this->orb_core_->default_client_priority ();

  return result;
}

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

CORBA::Policy *
TAO_Stub::sync_scope (void)
{
  CORBA::Policy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->sync_scope ();

  this->orb_core_->stubless_sync_scope (result);

  return result;
}

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

TAO_Buffering_Constraint_Policy *
TAO_Stub::buffering_constraint (void)
{
  TAO_Buffering_Constraint_Policy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->buffering_constraint ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.buffering_constraint ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->buffering_constraint ();
    }

  if (result == 0)
    result = this->orb_core_->default_buffering_constraint ();

  return result;
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_RT_CORBA == 1)

CORBA::Policy *
TAO_Stub::private_connection (void)
{
  CORBA::Policy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->private_connection ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.private_connection ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->private_connection ();
    }

  if (result == 0)
    result = this->orb_core_->default_private_connection ();

  return result;
}

CORBA::Policy *
TAO_Stub::client_protocol (void)
{
  CORBA::Policy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->client_protocol ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.client_protocol ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->client_protocol ();
    }

  // No default is used for client priority policy (default creates
  // conflict in case the policy is also set for the object on the
  // server side).

  return result;
}

CORBA::Policy *
TAO_Stub::priority_banded_connection (void)
{
  CORBA::Policy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->priority_banded_connection ();

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.priority_banded_connection ();
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->priority_banded_connection ();
    }

  if (result == 0)
    result = this->orb_core_->default_priority_banded_connection ();

  return result;
}

CORBA::Policy *
TAO_Stub::effective_priority_banded_connection (CORBA::Environment &ACE_TRY_ENV)
{
  // Get effective override.
  CORBA::Policy *override =
    this->priority_banded_connection ();

  // Get the value from the ior.
  CORBA::Policy *exposed =
    this->exposed_priority_banded_connection ();

  // Reconcile client-exposed and locally set values.
  if (exposed == 0)
    return override;

  if (override == 0)
    return exposed;

  CORBA::Policy_var policy =
    this->orb_core_->get_protocols_hooks ()->
    effective_priority_banded_connection_hook (override,
                                               exposed,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return policy.in ();
}

CORBA::Policy *
TAO_Stub::effective_client_protocol (CORBA::Environment &ACE_TRY_ENV)
{
  // Get effective override.
  CORBA::Policy_var override =
    this->client_protocol ();

  // Get the value from the ior.
  CORBA::Policy_var exposed =
    this->exposed_client_protocol ();

  // Reconcile client-exposed and locally set values.
  if (CORBA::is_nil (exposed.in ()))
    return override.in ();

  if (CORBA::is_nil (override.in ()))
    return exposed.in ();

  CORBA::Policy_var policy =
    this->orb_core_->get_protocols_hooks ()->effective_client_protocol_hook (
                                                override.in (),
                                                exposed.in (),
                                                ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  /*if (
  ACE_CHECK_RETURN (CORBA::INV_POLICY (),
                    CORBA::Policy::_nil ());
  */

  return policy.in ();
}

#endif /* TAO_HAS_RT_CORBA == 1 */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#if (TAO_HAS_CORBA_MESSAGING == 1)

template class auto_ptr<TAO_Policy_Manager_Impl>;
template class ACE_Auto_Basic_Ptr<TAO_Policy_Manager_Impl>;

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#if (TAO_HAS_CORBA_MESSAGING == 1)

#pragma instantiate auto_ptr<TAO_Policy_Manager_Impl>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Policy_Manager_Impl>

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
