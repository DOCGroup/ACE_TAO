// -*- C++ -*-

//=============================================================================
/**
 *  @file    Stub.h
 *
 *  $Id$
 *
 *  @author  Portions Copyright 1994-1995 by Sun Microsystems Inc.
 *  @author  Portions Copyright 1997-2002 by Washington University
 */
//=============================================================================

#ifndef TAO_STUB_H
#define TAO_STUB_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/MProfile.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/ORB_Core_Auto_Ptr.h"


// Forward declarations.
class TAO_RelativeRoundtripTimeoutPolicy;
class TAO_Client_Priority_Policy;
class TAO_Sync_Scope_Policy;
class TAO_Buffering_Constraint_Policy;

class TAO_Sync_Strategy;
class TAO_GIOP_Invocation;
class TAO_Policy_Set;

class TAO_Profile;

/// Forward declaration for ObjectKey
namespace TAO
{
  class ObjectKey;
}
// Function pointer returning a pointer to CORBA::Exception. This is used to
// describe the allocator for user-defined exceptions that are used internally
// by the interpreter.
typedef CORBA::Exception* (*TAO_Exception_Alloc) (void);

/**
 * @struct TAO_Exception_Data
 *
 * @brief Description of a single exception.
 *
 * The interpreter needs a way to allocate memory to hold the exception
 * that was raised by the stub. This data structure provides the typecode
 * for the exception as well as a static function pointer that
 * does the job of memory allocation.
 */
struct TAO_Exception_Data
{
  /// Repository id of the exception.
  const char *id;

  /// The allocator for this exception.
  TAO_Exception_Alloc alloc;
};

/**
 * @class TAO_Stub
 *
 * @brief TAO_Stub
 *
 * Per-objref data includes the (protocol-specific) Profile, which
 * is handled by placing it into a subclass of this type along
 * with data that may be used in protocol-specific caching
 * schemes.
 * The type ID (the data specified by CORBA 2.0 that gets exposed
 * "on the wire", and in stringified objrefs) is held by this
 * module.
 * The stub APIs are member functions of this type.
 */
class TAO_Export TAO_Stub
{
public:
#if (TAO_HAS_CORBA_MESSAGING == 1)

  /**
   * Returns the effective policy if <type> is a known client-exposed
   * policy type.  Returns the effective override for all other policy
   * types.
   */
  virtual CORBA::Policy_ptr get_policy (CORBA::PolicyType type
                                        ACE_ENV_ARG_DECL_WITH_DEFAULTS);


  virtual CORBA::Policy_ptr get_client_policy (CORBA::PolicyType type
                                               ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  virtual TAO_Stub* set_policy_overrides (const CORBA::PolicyList & policies,
                                          CORBA::SetOverrideType set_add
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  virtual CORBA::PolicyList * get_policy_overrides (
    const CORBA::PolicyTypeSeq & types
    ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  CORBA::Boolean validate_connection (
    CORBA::PolicyList_out inconsistent_policies
    ACE_ENV_ARG_DECL_WITH_DEFAULTS);

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */


  // = Methods for obtaining effective overrides.
  //
  //   Same functionality as <get_client_policy>, but more efficient.
  //   Each of the methods below returns effective override for the
  //   corresponding policy.  The effective override is obtained by
  //   first checking for an override of the given policy type at the
  //   Object scope, then at the Current scope, then at the ORB scope,
  //   and, finally, ORB default values are checked.

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

  CORBA::Policy_ptr relative_roundtrip_timeout (void);

#endif  /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

  CORBA::Policy_ptr sync_scope (void);

#endif  /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  CORBA::Policy_ptr buffering_constraint (void);

#endif  /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_CONNECTION_TIMEOUT_POLICY == 1)

  CORBA::Policy_ptr connection_timeout (void);

#endif  /* TAO_HAS_CONNECTION_TIMEOUT_POLICY == 1 */

  /// Return the sync strategy to be used in by the transport.
  /// Selection will be based on the SyncScope policies.
  TAO_Sync_Strategy &sync_strategy (void);

  /// All objref representations carry around a type ID.
  CORBA::String_var type_id;

  /**
   * All objref representations know how to hash themselves and
   * compare themselves for equivalence to others.  It's easily
   * possible to have two objrefs that are distinct copies of data
   * that refers/points to the same remote object (i.e. are
   * equivalent).
   */
  CORBA::ULong hash (CORBA::ULong maximum
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Implement the is_equivalent() method for the CORBA::Object
  CORBA::Boolean is_equivalent (CORBA::Object_ptr other_obj);

  // Our Constructors ...

  /// Construct from a repository ID and a list of profiles.
  TAO_Stub (const char *repository_id,
            const TAO_MProfile &profiles,
            TAO_ORB_Core *orb_core);

  // = Memory management.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  /// Return the Profile lock. This lock can be used at places where
  /// profiles need to be edited.
  ACE_Lock *profile_lock (void) const;

  // Manage the base (non-forwarded) profiles.
  /// returns a pointer to the profile_in_use object.  This object
  /// retains ownership of this profile.
  TAO_Profile *profile_in_use (void);

  /// Return the ObjectKey
  const TAO::ObjectKey &object_key (void) const;

  /**
   * Copy of the profile list, user must free memory when done.
   * although the user can call make_profiles() then reorder
   * the list and give it back to TAO_Stub.
   */
  TAO_MProfile *make_profiles (void);

  /// Obtain a reference to the basic profile set.
  const TAO_MProfile& base_profiles (void) const;

  /// Obtain a reference to the basic profile set.
  TAO_MProfile& base_profiles (void);

  /// Obtain a pointer to the forwarded profile set
  const TAO_MProfile *forward_profiles (void) const;

  // Manage forward and base profiles.
  /**
   * THREAD SAFE.  If forward_profiles is null then this will
   * get the next profile in the base_profiles list.  If forward is not null
   * then this will get the next profile for the list of forwarding
   * profiles.  If all profiles have been tried then 0 is returned and
   * profile_in_use_ is set to the first profile in the base_profiles
   * list.
   */
  TAO_Profile *next_profile (void);

  /// NON-THREAD SAFE version of next_profile (void)
  TAO_Profile *next_profile_i (void);

  /**
   * THREAD SAFE
   * this method will reset the base profile list to reference the first
   * profile and if there are anmy existing forward profiles they are
   * reset.
   */
  void reset_profiles (void);

  /// NON-THREAD SAFE version of reset_profiles (void);
  void reset_profiles_i (void);

  /// Returns 1 if a forward profile has successfully been used.
  /// profile_success_ && forward_profiles_
  CORBA::Boolean valid_forward_profile (void);

  /// NON-THREAD-SAFE.  Will set profile_success_ to 0.
  void set_valid_profile (void);

  /// Returns TRUE if a connection was successful with at least
  /// one profile.
  CORBA::Boolean valid_profile (void);

  /// Initialize the base_profiles_ and set profile_in_use_ to
  /// reference the first profile.
  TAO_Profile *base_profiles (const TAO_MProfile& mprofiles);

  /**
   * THREAD SAFE.
   * Set the forward_profiles.  This object will assume ownership of
   * this TAO_MProfile object!!
   */
  void add_forward_profiles (const TAO_MProfile &mprofiles);

  /**
   * THREAD SAFE
   * Used to get the next profile after the one being used has
   * failed during the initial connect or send of the message!
   */
  CORBA::Boolean next_profile_retry (void);

  /// Accessor.
  TAO_ORB_Core* orb_core (void) const;

  /// This returns a duplicated ORB pointer.
  CORBA::ORB_ptr servant_orb_ptr (void);

  /// This returns the ORB var itself (generally for temporary use).
  CORBA::ORB_var &servant_orb_var (void);

  /**
   * Accesor and mutator for the servant ORB.  Notice that the muatator
   * assumes the ownership of the passed in ORB and the accesor does not
   * return a copy of the orb since the accessing of the ORB is considered
   * temporary.
   */
  void servant_orb (CORBA::ORB_ptr orb);

  /// Make a call on to services to see whether they have some
  /// preferences on selecting the right profiles.
  CORBA::Boolean service_profile_selection (void);

  /**
   * Create the IOP::IOR info. We will create the info at most once.
   * Get the index of the profile we are using to make the invocation.
   */
  int create_ior_info (IOP::IOR *&ior_info,
                       CORBA::ULong &index
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return a reference to the reference count lock.
  /**
   * This method is meant to be used by the CORBA::Object
   * implementation to allow it to directly manipulate the reference
   * count.
   */
  TAO_SYNCH_MUTEX & refcount_lock (void);

  /// Return number of outstanding references to this object.
  /**
   * This method is meant to be used by the CORBA::Object
   * implementation to allow it to directly manipulate the reference
   * count.
   */
  CORBA::ULong & refcount (void);

  /// Deallocate the TAO_Stub object.
  /**
   * This method is intended to be used only by the CORBA::Object
   * class.
   */
  void destroy (void);

protected:

  /// Destructor is to be called only through _decr_refcnt() to
  /// enforce proper reference counting.
  virtual ~TAO_Stub (void);

private:
  /// Makes a copy of the profile and frees the existing profile_in_use.
  /// NOT THREAD SAFE
  TAO_Profile *set_profile_in_use_i (TAO_Profile *pfile);

  /// NON-THREAD-SAFE.  Utility method which resets or initializes
  /// the base_profile list and forward flags.
  void reset_base ();

  /// NON-THREAD-SAFE.  Utility method which unrolls (removes or pops)
  /// the top most forwarding profile list.
  void forward_back_one (void);

   /// NOT THREAD-SAFE.  Utility method which pops all forward profile
   /// lists and resets the forward_profiles_ pointer.
  void reset_forward ();

  /// NON-THREAD-SAFE.  utility method for next_profile.
  TAO_Profile *next_forward_profile (void);

  /// THREAD-SAFE Create the IOR info
  int get_profile_ior_info (TAO_MProfile &profile,
                            IOP::IOR *&ior_info
                            ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));


protected:
  /// Automatically manage the ORB_Core reference count
  /**
   * The ORB_Core cannot go away until the object references it
   * creates are destroyed.  There are multiple reasons for this, but
   * in particular, the allocators used for some of the TAO_Profile
   * objects contained on each TAO_Stub are owned by the TAO_ORB_Core.
   *
   * This <B>must</B> be the first field of the class, otherwise the
   * TAO_ORB_Core is destroyed too early!
   *
   */
  TAO_ORB_Core_Auto_Ptr orb_core_;

  /// ORB required for reference counting.  This will help us keep the
  /// ORB around until the CORBA::Object we represent dies.
  /**
   * @todo Why do we need both a reference to the ORB_Core and its
   *       ORB? It think the memory management rules for the ORB_Core
   *       changed, in the good old days it was the CORBA::ORB class
   *       who owned the ORB_Core, now it is the other way around....
   */
  CORBA::ORB_var orb_;

  /**
   * If this stub refers to a collocated object then we need to hold on to
   * the servant's ORB (which may be different from the client ORB) so that,
   *   1. we know that the ORB will stay alive long enough, and,
   *   2. we can search for the servant/POA's status starting from
   *      the ORB's RootPOA.
   */
  CORBA::ORB_var servant_orb_;

  /// Ordered list of profiles for this object.
  TAO_MProfile base_profiles_;

  /// The list of forwarding profiles.  This is actually implemented as a
  /// linked list of TAO_MProfile objects.
  TAO_MProfile *forward_profiles_;

  /// This is the profile that we are currently sending/receiving with.
  TAO_Profile *profile_in_use_;

  /// Mutex to protect access to the forwarding profile.
  ACE_Lock* profile_lock_ptr_;

  /// Have we successfully talked to the forward profile yet?
  size_t profile_success_;

  /// Mutex to protect reference count.
  TAO_SYNCH_MUTEX refcount_lock_;

  /// Number of outstanding references to this object.
  CORBA::ULong refcount_;

  /// The policy overrides in this object, if nil then use the default
  /// policies.
  TAO_Policy_Set *policies_;

  /**
   * The ior info. This is needed for GIOP 1.2, as the clients could
   * receive an exception from the server asking for this info.  The
   * exception that the client receives is LOC_NEEDS_ADDRESSING_MODE.
   * The data is set up here to be passed on to Invocation classes
   * when they receive an exception. This info is for the base
   * profiles that this class stores
   */
  IOP::IOR *ior_info_;

  /// Forwarded IOR info
  IOP::IOR *forwarded_ior_info_;

  // = Disallow copy constructor and assignment operator.
  ACE_UNIMPLEMENTED_FUNC (TAO_Stub (const TAO_Stub &))
  ACE_UNIMPLEMENTED_FUNC (TAO_Stub &operator = (const TAO_Stub &))

#if defined (__GNUG__)
  // G++ (even 2.6.3) stupidly thinks instances can't be created.
  // This de-warns.
  friend class everyone_needs_a_friend;
#endif /* __GNUG__ */
};

// Define a TAO_Stub auto_ptr class.
/**
 * @class TAO_Stub_Auto_Ptr
 *
 * @brief Implements the draft C++ standard auto_ptr abstraction.
 * This class allows one to work Stub Objects *Only*!
 */
class TAO_Export TAO_Stub_Auto_Ptr
{
public:

  // = Initialization and termination methods.
  /* explicit */ TAO_Stub_Auto_Ptr (TAO_Stub *p = 0);
  TAO_Stub_Auto_Ptr (TAO_Stub_Auto_Ptr &ap);
  TAO_Stub_Auto_Ptr &operator= (TAO_Stub_Auto_Ptr &rhs);
  ~TAO_Stub_Auto_Ptr (void);

  // = Accessor methods.
  TAO_Stub &operator *() const;
  TAO_Stub *get (void) const;
  TAO_Stub *release (void);
  void reset (TAO_Stub *p = 0);
  TAO_Stub *operator-> () const;

protected:

  TAO_Stub *p_;

};

#if defined (__ACE_INLINE__)
# include "tao/Stub.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_STUB_H */
