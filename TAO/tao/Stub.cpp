// $Id$

// Portions Copyright 1995 by Sun Microsystems Inc.
// Portions Copyright 1997-2002 by Washington University
// All Rights Reserved
//
// Some CORBA::Object and other operations are specific to this STUB
// based implementation, and can neither be used by other kinds of
// objref nor have a default implementation.

#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Remote_Object_Proxy_Broker.h"
#include "tao/Transport_Queueing_Strategies.h"
#include "tao/debug.h"
#include "tao/Policy_Manager.h"
#include "tao/Policy_Set.h"
#include "tao/SystemException.h"
#include "tao/CDR.h"

#if !defined (__ACE_INLINE__)
# include "tao/Stub.inl"
#endif /* ! __ACE_INLINE__ */

#include "ace/Auto_Ptr.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Stub::TAO_Stub (const char *repository_id,
                    const TAO_MProfile &profiles,
                    TAO_ORB_Core *orb_core)
  : type_id (repository_id)
  , orb_core_ (orb_core)
  , orb_ ()
  , is_collocated_ (false)
  , servant_orb_ ()
  , collocated_servant_ (0)
  , object_proxy_broker_ (the_tao_remote_object_proxy_broker ())
  , base_profiles_ ((CORBA::ULong) 0)
  , forward_profiles_ (0)
  , forward_profiles_perm_ (0)
  , profile_in_use_ (0)
  , profile_success_ (false)
  , refcount_ (1)
#if (TAO_HAS_CORBA_MESSAGING == 1)
  , policies_ (0)
#endif
  , ior_info_ (0)
  , forwarded_ior_info_ (0)
  , collocation_opt_ (orb_core->optimize_collocation_objects ())
  , forwarded_on_exception_ (false)
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

  // Explicit trigger the loading of the client strategy factory at this moment.
  // Not doing it here could lead to a problem loading it later on during
  // an upcall
  (void) this->orb_core_->client_factory ();

  this->base_profiles (profiles);
}

TAO_Stub::~TAO_Stub (void)
{
  ACE_ASSERT (this->refcount_ == 0);

  if (this->forward_profiles_)
    reset_profiles ();

  // reset_profiles doesn't delete forward_profiles_perm_.
  delete this->forward_profiles_perm_;

  if (this->profile_in_use_ != 0)
    {
      // decrease reference count on profile
      this->profile_in_use_->_decr_refcnt ();
      this->profile_in_use_ = 0;
    }

#if (TAO_HAS_CORBA_MESSAGING == 1)
  delete this->policies_;
#endif

  delete this->ior_info_;

  delete this->forwarded_ior_info_;
}

void
TAO_Stub::add_forward_profiles (const TAO_MProfile &mprofiles,
                                const CORBA::Boolean permanent_forward)
{
  // we assume that the profile_in_use_ is being
  // forwarded!  Grab the lock so things don't change.
  ACE_MT (ACE_GUARD (TAO_SYNCH_MUTEX,
                     guard,
                     this->profile_lock_));

  if (permanent_forward)
    {
      // paranoid, reset the bookmark, then clear the forward-stack
      this->forward_profiles_perm_ = 0;

      this->reset_forward ();
    }

  TAO_MProfile *now_pfiles = this->forward_profiles_;
  if (now_pfiles == 0)
    now_pfiles = &this->base_profiles_;

  ACE_NEW (this->forward_profiles_,
           TAO_MProfile (mprofiles));

  if (permanent_forward)
    // bookmark the new element at bottom of stack
    this->forward_profiles_perm_ = this->forward_profiles_;

  // forwarded profile points to the new IOR (profiles)
  this->profile_in_use_->forward_to (this->forward_profiles_);

  // new profile list points back to the list which was forwarded.
  this->forward_profiles_->forward_from (now_pfiles);

  // make sure we start at the beginning of mprofiles
  this->forward_profiles_->rewind ();

  // Since we have been forwarded, we must set profile_success_ to false
  // since we are starting a new with a new set of profiles!
  this->profile_success_ = false;
}

int
TAO_Stub::create_ior_info (IOP::IOR *&ior_info, CORBA::ULong &index)
{
  // We are creating the IOR info. Let us not be disturbed. So grab a
  // lock.
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                            guard,
                            this->profile_lock_,
                            -1));

  IOP::IOR *tmp_info = 0;

  if (this->forward_profiles_ != 0)
    {
      if (this->forwarded_ior_info_ == 0)
        {
          this->get_profile_ior_info (*this->forward_profiles_, tmp_info);

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
      this->get_profile_ior_info (this->base_profiles_, tmp_info);

      this->ior_info_ = tmp_info;
    }


  for (CORBA::ULong ind = 0;
       ind < this->base_profiles_.profile_count ();
       ++ind)
    {
      if (this->base_profiles_.get_profile (ind) == this->profile_in_use_)
        {
          index = ind;
          ior_info = this->ior_info_;
          return 0;
        }
    }

  // Error, there was no match
  return -1;
}

const TAO::ObjectKey &
TAO_Stub::object_key (void) const
{
  // Return the profile in use's object key if you see one.
  if (this->profile_in_use_)
    return this->profile_in_use_->object_key ();

  if (this->forward_profiles_)
    {
      // Double-checked
      // FUZZ: disable check_for_ACE_Guard
      ACE_Guard<TAO_SYNCH_MUTEX> obj (const_cast <TAO_SYNCH_MUTEX&>(this->profile_lock_));
      // FUZZ: enable check_for_ACE_Guard

      if (obj.locked () != 0 &&  this->forward_profiles_ != 0)
        return this->forward_profiles_->get_profile (0)->object_key ();
    }

  // If no forwarded profiles, just use the base profile
  return this->base_profiles_.get_profile (0)->object_key ();
}

int
TAO_Stub::get_profile_ior_info (TAO_MProfile &profiles, IOP::IOR *&ior_info)
{
  ACE_NEW_THROW_EX (ior_info,
                    IOP::IOR (),
                    CORBA::NO_MEMORY ());

  // Get the number of elements
  CORBA::ULong const count = profiles.profile_count ();

  // Set the number of elements in the sequence of tagged_profile
  ior_info->profiles.length (count);

  // Call the create_tagged_profile one every member of the
  // profile and make the sequence
  for (CORBA::ULong index = 0; index < count; ++index)
    {
      TAO_Profile *prof = profiles.get_profile (index);

      IOP::TaggedProfile *tp = prof->create_tagged_profile ();

      if (tp == 0)
        throw ::CORBA::NO_MEMORY ();
      ior_info->profiles[index] = *tp;
    }

  return 0;
}

void
TAO_Stub::is_collocated (CORBA::Boolean collocated)
{
  if (this->is_collocated_ != collocated)
    {
      if (collocated &&
          _TAO_Object_Proxy_Broker_Factory_function_pointer != 0)
        {
          this->object_proxy_broker_ =
            _TAO_Object_Proxy_Broker_Factory_function_pointer ();
        }
      else
        {
          this->object_proxy_broker_ = the_tao_remote_object_proxy_broker ();
        }
      this->is_collocated_ = collocated;
    }
}

// Quick'n'dirty hash of objref data, for partitioning objrefs into
// sets.
//
// NOTE that this must NOT go across the network!

CORBA::ULong
TAO_Stub::hash (CORBA::ULong max)
{
  // we rely on the profile objects that its address info
  return this->base_profiles_.hash (max);
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
  if (CORBA::is_nil (other_obj))
    return false;

  TAO_Profile * const other_profile = other_obj->_stubobj ()->profile_in_use_;
  TAO_Profile * const this_profile = this->profile_in_use_;

  if (other_profile == 0 || this_profile == 0)
    return false;

  // Compare the profiles
  return this_profile->is_equivalent (other_profile);
}

// Memory managment

TAO_Profile *
TAO_Stub::set_profile_in_use_i (TAO_Profile *pfile)
{
  TAO_Profile *const old = this->profile_in_use_;

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

  // Only delete the forward location if it is not permanent
  if (this->forward_profiles_ != this->forward_profiles_perm_)
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
// ****************************************************************

#if (TAO_HAS_CORBA_MESSAGING == 1)

// Some policies can only be set locally on the client, while others
// can only be exported in the IOR by the server, and yet others can
// be set by both by client and server.  Furthermore, reconciliation
// between client-set values and the ones exported in the IOR is
// policy-specific.  Therefore, with the current state of things, it
// isn't possible to write generic code for <get_policy> that will
// work for any policy type.  Currently, we take specific action for
// each of the known client-exposed policies (above), and simply look
// up effective override for any other policy type (below).  Later, if
// there is a need/desire for generic code, it can be done by pushing
// the smarts into the policies implementations, and will involve
// modifying PolicyC* and friends, e.g., to add methods for policy
// specific reconciliation, etc.

CORBA::Policy_ptr
TAO_Stub::get_policy (CORBA::PolicyType type)
{
  // No need to lock, the stub only changes its policies at
  // construction time...

  CORBA::Policy_var result;
  if (this->policies_ != 0)
    {
      result = this->policies_->get_policy (type);
    }

  if (CORBA::is_nil (result.in ()))
    {
      result = this->orb_core_->get_policy_including_current (type);
    }

  return result._retn ();
}

CORBA::Policy_ptr
TAO_Stub::get_cached_policy (TAO_Cached_Policy_Type type)
{
  // No need to lock, the stub only changes its policies at
  // construction time...

  CORBA::Policy_var result;
  if (this->policies_ != 0)
    {
      result = this->policies_->get_cached_policy (type);
    }

  if (CORBA::is_nil (result.in ()))
    {
      result = this->orb_core_->get_cached_policy_including_current (type);
    }

  return result._retn ();
}

TAO_Stub *
TAO_Stub::set_policy_overrides (const CORBA::PolicyList & policies,
                                CORBA::SetOverrideType set_add)
{
  // Notice the use of an explicit constructor....
  auto_ptr<TAO_Policy_Set> policy_manager (
    new TAO_Policy_Set (TAO_POLICY_OBJECT_SCOPE));

  if (set_add == CORBA::SET_OVERRIDE)
    {
      policy_manager->set_policy_overrides (policies, set_add);
    }
  else if (this->policies_ == 0)
    {
      policy_manager->set_policy_overrides (policies, CORBA::SET_OVERRIDE);
    }
  else
    {
      policy_manager->copy_from (this->policies_);

      policy_manager->set_policy_overrides (policies, set_add);
    }

  TAO_Stub* stub = this->orb_core_->create_stub (this->type_id.in (),
                                                 this->base_profiles_);

  stub->policies_ = policy_manager.release ();

  // Copy the servant ORB if it is present.
  stub->servant_orb (this->servant_orb_var ().in ());

  return stub;
}

CORBA::PolicyList *
TAO_Stub::get_policy_overrides (const CORBA::PolicyTypeSeq &types)
{
  if (this->policies_ == 0)
    {
      CORBA::PolicyList *policy_list_ptr = 0;
      ACE_NEW_THROW_EX (policy_list_ptr,
                        CORBA::PolicyList (),
                        CORBA::NO_MEMORY ());

      return policy_list_ptr;
    }
  else
    {
      return this->policies_->get_policy_overrides (types);
    }
}
#endif

CORBA::Boolean
TAO_Stub::marshal (TAO_OutputCDR &cdr)
{
  // do as many outside of locked else-branch as posssible

  // STRING, a type ID hint
  if ((cdr << this->type_id.in()) == 0)
    return 0;

  if ( ! this->forward_profiles_perm_)
    {
      const TAO_MProfile& mprofile = this->base_profiles_;

      CORBA::ULong const profile_count = mprofile.profile_count ();
      if ((cdr << profile_count) == 0)
        return 0;

    // @@ The MProfile should be locked during this iteration, is there
    // anyway to achieve that?
    for (CORBA::ULong i = 0; i < profile_count; ++i)
      {
        const TAO_Profile* p = mprofile.get_profile (i);
        if (p->encode (cdr) == 0)
          return 0;
      }
    }
  else
    {
      ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                                guard,
                                this->profile_lock_,
                                0));

      ACE_ASSERT(this->forward_profiles_ !=0);

      // paranoid - in case of FT the basic_profiles_ would do, too,
      // but might be dated
      const TAO_MProfile& mprofile =
          this->forward_profiles_perm_
        ? *(this->forward_profiles_perm_)
        : this->base_profiles_;

      CORBA::ULong const profile_count = mprofile.profile_count ();
      if ((cdr << profile_count) == 0)
           return 0;

      // @@ The MProfile should be locked during this iteration, is there
      // anyway to achieve that?
      for (CORBA::ULong i = 0; i < profile_count; ++i)
        {
          const TAO_Profile* p = mprofile.get_profile (i);
          if (p->encode (cdr) == 0)
            return 0;
        }

      // release ACE_Lock
    }

  return (CORBA::Boolean) cdr.good_bit ();
}

void
TAO_Stub::_incr_refcnt (void)
{
  ++this->refcount_;
}

void
TAO_Stub::_decr_refcnt (void)
{
  if (--this->refcount_ == 0)
    delete this;
}

TAO_END_VERSIONED_NAMESPACE_DECL
