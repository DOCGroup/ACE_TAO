// $Id$

// Portions Copyright 1995 by Sun Microsystems Inc.
// Portions Copyright 1997-2002 by Washington University
// All Rights Reserved
//
// Some CORBA::Object and other operations are specific to this STUB
// based implementation, and can neither be used by other kinds of
// objref nor have a default implementation.

#include "Endpoint.h"
#include "Stub.h"
#include "Profile.h"
#include "Sequence.h"
#include "Object.h"
#include "Invocation.h"
#include "Asynch_Invocation.h"
#include "ORB_Core.h"
#include "Client_Strategy_Factory.h"
#include "Sync_Strategies.h"
#include "Buffering_Constraint_Policy.h"
#include "debug.h"

#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
# include "Stub.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           TAO_Stub,
           "$Id$")

TAO_Stub::TAO_Stub (const char *repository_id,
                    const TAO_MProfile &profiles,
                    TAO_ORB_Core *orb_core)
  : type_id (repository_id),
    orb_core_ (orb_core),
    orb_ (),
    servant_orb_ (),
    base_profiles_ ((CORBA::ULong) 0),
    forward_profiles_ (0),
    profile_in_use_ (0),
    profile_lock_ptr_ (0),
    profile_success_ (0),
    refcount_lock_ (),
    refcount_ (1),
#if (TAO_HAS_CORBA_MESSAGING == 1)
    policies_ (0),
#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
    ior_info_ (0),
    forwarded_ior_info_ (0)
{
  if (this->orb_core_.get() == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO: (%P|%t) TAO_Stub created with default ")
                      ACE_TEXT ("ORB core\n")));
        }
      this->orb_core_.reset (TAO_ORB_Core_instance ());
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
          TAO_Endpoint* ep = this->profile_in_use_->endpoint ();
          if (ep)
            ep->reset_hint ();
        }

      // decrease reference count on profile
      this->profile_in_use_->_decr_refcnt ();
      this->profile_in_use_ = 0;
    }

  delete this->profile_lock_ptr_;

#if (TAO_HAS_CORBA_MESSAGING == 1)

  delete this->policies_;

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  if (this->ior_info_)
    delete this->ior_info_;

  if (this->forwarded_ior_info_)
    delete this->forwarded_ior_info_;
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


int
TAO_Stub::create_ior_info (IOP::IOR *&ior_info,
                           CORBA::ULong &index
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // We are creating the IOR info. Let us not be disturbed. So grab a
  // lock.
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->profile_lock_ptr_,
                            -1));

  IOP::IOR *tmp_info = 0;

  if (this->forward_profiles_ != 0)
    {
      if (this->forwarded_ior_info_ == 0)
        {
          this->get_profile_ior_info (*this->forward_profiles_,
                                      tmp_info
                                       ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

          this->forwarded_ior_info_ = tmp_info;
        }

      // First we look at the forward profiles to see whether the
      // profile_in_use is any of it.
      for (CORBA::ULong i = 0;
           i < this->forward_profiles_->profile_count ();
           ++i)
        {
          if (this->forward_profiles_->get_profile (i)
              == this->profile_in_use_)
            {
              ior_info = this->forwarded_ior_info_;
              index = i;
              return 0;
            }
        }
    }

  // Else we look at the base profiles
  if (this->ior_info_ == 0)
    {
      this->get_profile_ior_info (this->base_profiles_,
                                  tmp_info
                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      this->ior_info_ = tmp_info;
    }


  for (CORBA::ULong ind = 0;
       ind < this->base_profiles_.profile_count ();
       ++ind)
    {
      if (this->base_profiles_.get_profile (ind) ==
          this->profile_in_use_)
        {
          index = ind;
          ior_info = this->ior_info_;
          return 0;
        }
    }

  // Error, there was no match
  return -1;
}


int
TAO_Stub::get_profile_ior_info (TAO_MProfile &profiles,
                                IOP::IOR *&ior_info
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{


  ACE_NEW_THROW_EX (ior_info,
                    IOP::IOR (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (-1);


  // Get the number of elements
  CORBA::ULong count = profiles.profile_count ();

  // Set the number of elements in the sequence of tagged_profile
  ior_info->profiles.length (count);

  // Call the create_tagged_profile one every member of the
  // profile and make the sequence
  for (CORBA::ULong index = 0; index < count; ++index)
    {
      TAO_Profile *prof = profiles.get_profile (index);

      ior_info->profiles[index] = prof->create_tagged_profile ();
    }

  return 0;
}



// Quick'n'dirty hash of objref data, for partitioning objrefs into
// sets.
//
// NOTE that this must NOT go across the network!

CORBA::ULong
TAO_Stub::hash (CORBA::ULong max
                ACE_ENV_ARG_DECL)
{
  // we rely on the profile objects that its address info
  return this->base_profiles_.hash (max ACE_ENV_ARG_PARAMETER);
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
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->refcount_lock_,
                    0);

  return this->refcount_++;
}

CORBA::ULong
TAO_Stub::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
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

TAO_Profile *
TAO_Stub::set_profile_in_use_i (TAO_Profile *pfile)
{
  TAO_Profile *old = this->profile_in_use_;

  // Since we are actively using this profile we dont want
  // it to disappear, so increase the reference count by one!!
  if (pfile && (pfile->_incr_refcnt () == 0))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) unable to increment profile ref!\n")),
                        0);
    }

  this->profile_in_use_ = pfile;

  if (old)
    old->_decr_refcnt ();

  return this->profile_in_use_;
}

void
TAO_Stub::forward_back_one (void)
{
  TAO_MProfile *from = forward_profiles_->forward_from ();

  delete this->forward_profiles_;

  // the current profile in this profile list is no
  // longer being forwarded, so set the reference to zero.
  if (from == &this->base_profiles_)
    {
      this->base_profiles_.get_current_profile ()->forward_to (0);
      this->forward_profiles_ = 0;
    }
  else
    {
      from->get_current_profile ()->forward_to (0);
      this->forward_profiles_ = from;
    }

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

CORBA::Policy_ptr
TAO_Stub::get_policy (CORBA::PolicyType type
                      ACE_ENV_ARG_DECL)
{

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

  return this->get_client_policy (type
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::Policy_ptr
TAO_Stub::get_client_policy (CORBA::PolicyType type
                             ACE_ENV_ARG_DECL)
{
  // No need to lock, the stub only changes its policies at
  // construction time...


  CORBA::Policy_var result;
  if (this->policies_ != 0)
    {
      result = this->policies_->get_policy (type
                                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  if (CORBA::is_nil (result.in ()))
    {
      TAO_Policy_Current &policy_current = this->orb_core_->policy_current ();
      result = policy_current.get_policy (type
                                           ACE_ENV_ARG_PARAMETER);
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
          result = policy_manager->get_policy (type
                                                ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::Policy::_nil ());
        }
    }

  if (CORBA::is_nil (result.in ()))
    {
      result = this->orb_core_->
              get_default_policies ()->get_policy (type
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  return result._retn ();
}

TAO_Stub *
TAO_Stub::set_policy_overrides (const CORBA::PolicyList & policies,
                                CORBA::SetOverrideType set_add
                                ACE_ENV_ARG_DECL)
{
  // Notice the use of an explicit constructor....
  auto_ptr<TAO_Policy_Set> policy_manager (
    new TAO_Policy_Set (TAO_POLICY_OBJECT_SCOPE));

  if (set_add == CORBA::SET_OVERRIDE)
    {
      policy_manager->set_policy_overrides (policies,
                                            set_add
                                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  else if (this->policies_ == 0)
    {
      policy_manager->set_policy_overrides (policies,
                                            CORBA::SET_OVERRIDE
                                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  else
    {
      policy_manager->copy_from (this->policies_
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      policy_manager->set_policy_overrides (policies,
                                            set_add
                                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  TAO_Stub* stub = this->orb_core_->create_stub (this->type_id.in (),
                                                 this->base_profiles_
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  stub->policies_ = policy_manager.release ();

  // Copy the servant ORB if it is present.
  stub->servant_orb (this->servant_orb_var ().in ());

  return stub;
}

CORBA::PolicyList *
TAO_Stub::get_policy_overrides (const CORBA::PolicyTypeSeq &types
                                ACE_ENV_ARG_DECL)
{
  if (this->policies_ == 0)
    return 0;

  return this->policies_->get_policy_overrides (types
                                                 ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_Stub::validate_connection (CORBA::PolicyList_out inconsistent_policies
                               ACE_ENV_ARG_DECL)
{
  // Use Locate Request to establish connection/make sure the object
  // is there ...
  TAO_GIOP_Locate_Request_Invocation locate_request (this,
                                                     this->orb_core_.get ());

  locate_request.init_inconsistent_policies (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // @@ For some combinations of RTCORBA policies, location forwarding
  // isn't supported.  See <forward> method implementations in
  // Invocation_Endpoint_Selectors.cpp for more information.
  //
  ACE_TRY
    {
      for (;;)
        {
          locate_request.start (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          int status = locate_request.invoke (ACE_ENV_SINGLE_ARG_PARAMETER);
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

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
      result = this->policies_->get_cached_policy (TAO_CACHED_POLICY_RELATIVE_ROUNDTRIP_TIMEOUT);

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();
      result = policy_current.get_cached_policy (TAO_CACHED_POLICY_RELATIVE_ROUNDTRIP_TIMEOUT);
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->get_cached_policy (TAO_CACHED_POLICY_RELATIVE_ROUNDTRIP_TIMEOUT);
    }

  if (result == 0)
    result = this->orb_core_->get_default_policies ()->get_cached_policy (TAO_CACHED_POLICY_RELATIVE_ROUNDTRIP_TIMEOUT);

  return result;
}

#endif /* TAO_HAS_REALTIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */


#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

CORBA::Policy *
TAO_Stub::sync_scope (void)
{
  CORBA::Policy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...
  if (this->policies_ != 0)
    result = this->policies_->get_cached_policy (TAO_CACHED_POLICY_SYNC_SCOPE);

  // If there are no cached policies, look at the thread or ORB level
  // for the policy.
  if (result == 0)
    this->orb_core_->stubless_sync_scope (result);

  return result;
}

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

CORBA::Policy *
TAO_Stub::buffering_constraint (void)
{
  CORBA::Policy *result = 0;

  // No need to lock, the stub only changes its policies at
  // construction time...

  if (this->policies_ != 0)
    {
      result =
        this->policies_->get_cached_policy (TAO_CACHED_POLICY_BUFFERING_CONSTRAINT);
    }

  // No need to lock, the object is in TSS storage....
  if (result == 0)
    {
      TAO_Policy_Current &policy_current =
        this->orb_core_->policy_current ();

      result =
        policy_current.get_cached_policy (TAO_CACHED_POLICY_BUFFERING_CONSTRAINT);
    }

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->orb_core_->policy_manager ();
      if (policy_manager != 0)
        {
          result =
            policy_manager->get_cached_policy (TAO_CACHED_POLICY_BUFFERING_CONSTRAINT);
        }
    }

  if (result == 0)
    result = this->orb_core_->default_buffering_constraint ();

  return result;
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */



#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#if (TAO_HAS_CORBA_MESSAGING == 1)

template class auto_ptr<TAO_Policy_Set>;
template class ACE_Auto_Basic_Ptr<TAO_Policy_Set>;

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#if (TAO_HAS_CORBA_MESSAGING == 1)

#pragma instantiate auto_ptr<TAO_Policy_Set>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Policy_Set>

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
