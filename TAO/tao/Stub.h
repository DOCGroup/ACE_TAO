// This may look like C, but it's really -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Stub.h
//
// = DESCRIPTION
//
//    Data structures used by static and dynamic stubs, and to a
//    lesser degree by skeletons
//
//    Header file for Win32 C/C++/COM interface to one kind of CORBA
//    remote invocation framework.  This is for use by
//    compiler-generated code, not by portable applications!
//
//    These constitute the stub API to this "ORB Core."  Such
//    interfaces are not specified by OMG, and may differ between
//    different ORBs.  This one has the particular advantage that
//    stubs can be quite small.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
//
// ============================================================================

#ifndef TAO_STUB_H
#define TAO_STUB_H
#include "ace/pre.h"

#include "tao/Pluggable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/MProfile.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"

// Forward declarations.

class TAO_RelativeRoundtripTimeoutPolicy;
class TAO_Client_Priority_Policy;
class TAO_Sync_Scope_Policy;
class TAO_Buffering_Constraint_Policy;

class TAO_Sync_Strategy;
class TAO_GIOP_Invocation;
class TAO_Policy_Manager_Impl;

#if (TAO_HAS_RT_CORBA == 1)

class TAO_PriorityModelPolicy;
class TAO_PriorityBandedConnectionPolicy;
class TAO_ClientProtocolPolicy;
class TAO_PrivateConnectionPolicy;

#endif /* TAO_HAS_RT_CORBA == 1 */

// Descriptions of parameters.

enum TAO_Param_Type
{
  // = TITLE
  //    TAO_Param_Type
  //
  // = DESCRIPTION
  //    Parameter mode of a given parameter.
  PARAM_IN,
  PARAM_OUT,
  PARAM_INOUT,
  PARAM_RETURN  // = PARAM_OUT
};

class TAO_Profile;

// Function pointer returning a pointer to CORBA::Exception. This is used to
// describe the allocator for user-defined exceptions that are used internally
// by the interpreter.
typedef CORBA::Exception* (*TAO_Exception_Alloc) (void);

struct TAO_Exception_Data
{
  // = TITLE
  //    TAO_Exception_Data
  //
  // = DESCRIPTION
  //    Description of a single exception.
  //
  //    The interpreter needs a way to allocate memory to hold the exception
  //    that was raised by the stub. This data structure provides the typecode
  //    for the exception as well as a static function pointer that
  //    does the job of memory allocation.

  CORBA::TypeCode_ptr tc;
  // Typecode describing the exception.

  TAO_Exception_Alloc alloc;
  // The allocator for this exception.
};

class TAO_Export TAO_Stub
{
  // = TITLE
  //    TAO_Stub
  //
  // = DESCRIPTION
  //    Per-objref data includes the (protocol-specific) Profile, which
  //    is handled by placing it into a subclass of this type along
  //    with data that may be used in protocol-specific caching
  //    schemes.
  //
  //    The type ID (the data specified by CORBA 2.0 that gets exposed
  //    "on the wire", and in stringified objrefs) is held by this
  //    module.
  //
  //    The stub APIs are member functions of this
  //    type.
public:
#if (TAO_HAS_CORBA_MESSAGING == 1)

  CORBA::Policy_ptr get_policy (CORBA::PolicyType type,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ()
    );
  // Returns the effective policy if <type> is a known client-exposed
  // policy type.  Returns the effective override for all other policy
  // types.

  CORBA::Policy_ptr get_client_policy (CORBA::PolicyType type,
                                       CORBA::Environment &ACE_TRY_ENV =
                                       TAO_default_environment ()
    );
  TAO_Stub* set_policy_overrides (const CORBA::PolicyList & policies,
                                  CORBA::SetOverrideType set_add,
                                  CORBA::Environment &ACE_TRY_ENV =
                                  TAO_default_environment ()
    );
  CORBA::PolicyList * get_policy_overrides (const CORBA::PolicyTypeSeq & types,
                                            CORBA::Environment &ACE_TRY_ENV =
                                            TAO_default_environment ()
    );
  CORBA::Boolean validate_connection (
      CORBA::PolicyList_out inconsistent_policies,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#if (TAO_HAS_RT_CORBA == 1)

  TAO_PriorityModelPolicy *exposed_priority_model (void);
  // Returns the RTCORBA::PriorityModelPolicy exported
  // in object's IOR.

  TAO_PriorityBandedConnectionPolicy *exposed_priority_banded_connection (void);
  // Returns the RTCORBA::PriorityBandedConnectionPolicy exported
  // in object's IOR.

  TAO_ClientProtocolPolicy *exposed_client_protocol (void);
  // Returns the RTCORBA::ClientProtocolPolicy exported
  // in object's IOR.

# endif /*TAO_HAS_RT_CORBA == 1*/

  // = Methods for obtaining effective overrides.
  //
  //   Same functionality as <get_client_policy>, but more efficient.
  //   Each of the methods below returns effective override for the
  //   corresponding policy.  The effective override is obtained by
  //   first checking for an override of the given policy type at the
  //   Object scope, then at the Current scope, then at the ORB scope,
  //   and, finally, ORB default values are checked.

  CORBA::Policy *relative_roundtrip_timeout (void);

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  TAO_Client_Priority_Policy *client_priority (void);

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

  CORBA::Policy *sync_scope (void);

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  TAO_Buffering_Constraint_Policy *buffering_constraint (void);

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_RT_CORBA == 1)

  TAO_PrivateConnectionPolicy *private_connection (void);

  TAO_PriorityBandedConnectionPolicy *
  priority_banded_connection (void);

  TAO_ClientProtocolPolicy *client_protocol (void);

  // = Methods for obtaining effective policies.
  //
  //   Effective policy is the one that would be used if a request
  //   were made.  The policy is determined by obtaining the effective
  //   override for a given policy type, and then reconciling it with
  //   the policy value exported in the Object's IOR.

  TAO_PriorityBandedConnectionPolicy *
  effective_priority_banded_connection (CORBA::Environment
                                        &ACE_TRY_ENV =
                                        TAO_default_environment ());
  TAO_ClientProtocolPolicy *
  effective_client_protocol (CORBA::Environment &ACE_TRY_ENV =
                             TAO_default_environment ());

#endif /* TAO_HAS_RT_CORBA == 1 */

  TAO_Sync_Strategy &sync_strategy (void);
  // Return the sync strategy to be used in by the transport.
  // Selection will be based on the SyncScope policies.

  CORBA::String_var type_id;
  // All objref representations carry around a type ID.

  CORBA::ULong hash (CORBA::ULong maximum,
          CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  // All objref representations know how to hash themselves and
  // compare themselves for equivalence to others.  It's easily
  // possible to have two objrefs that are distinct copies of data
  // that refers/points to the same remote object (i.e. are
  // equivalent).

  CORBA::Boolean is_equivalent (CORBA::Object_ptr other_obj);
  // Implement the is_equivalent() method for the CORBA::Object

  // Our Constructors ...

  TAO_Stub (const char *repository_id,
            const TAO_MProfile &profiles,
            TAO_ORB_Core *orb_core);
  // Construct from a repository ID and a list of profiles.

  // = Memory management.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  // Manage the base (non-forwarded) profiles.
  TAO_Profile *profile_in_use (void) ;
  // returns a pointer to the profile_in_use object.  This object
  // retains ownership of this profile.

  TAO_MProfile *make_profiles (void);
  // Copy of the profile list, user must free memory when done.
  // although the user can call make_profiles() then reorder
  // the list and give it back to TAO_Stub.

  const TAO_MProfile& base_profiles (void) const;
  // Obtain a reference to the basic profile set.

  TAO_MProfile& base_profiles (void);
  // Obtain a reference to the basic profile set.

  // Manage forward and base profiles.
  TAO_Profile *next_profile (void);
  // THREAD SAFE.  If forward_profiles is null then this will
  // get the next profile in the base_profiles list.  If forward is not null
  // then this will get the next profile for the list of forwarding
  // profiles.  If all profiles have been tried then 0 is returned and
  // profile_in_use_ is set to the first profile in the base_profiles
  // list.

  TAO_Profile *next_profile_i (void);
  // NON-THREAD SAFE version of next_profile (void)

  void reset_profiles (void);
  // THREAD SAFE
  // this method will reset the base profile list to reference the first
  // profile and if there are anmy existing forward profiles they are
  // reset.

  void reset_profiles_i (void);
  // NON-THREAD SAFE version of reset_profiles (void);

  CORBA::Boolean valid_forward_profile (void);
  // Returns 1 if a forward profile has successfully been used.
  // profile_success_ && forward_profiles_

  void set_valid_profile (void);
  // NON-THREAD-SAFE.  Will set profile_success_ to 0.

  CORBA::Boolean valid_profile (void);
  // Returns TRUE if a connection was successful with at least
  // one profile.

  TAO_Profile *base_profiles (const TAO_MProfile& mprofiles);
  // Initialize the base_profiles_ and set profile_in_use_ to
  // reference the first profile.

  void add_forward_profiles (const TAO_MProfile &mprofiles);
  // THREAD SAFE.
  // Set the forward_profiles.  This object will assume ownership of
  // this TAO_MProfile object!!

  CORBA::Boolean next_profile_retry (void);
  // THREAD SAFE
  // Used to get the next profile after the one being used has
  // failed during the initial connect or send of the message!

  TAO_ORB_Core* orb_core (void) const;
  // Accessor.

  CORBA::ORB_ptr servant_orb_ptr (void);
  // This returns a duplicated ORB pointer.

  CORBA::ORB_var &servant_orb_var (void);
  // This returns the ORB var itself (generally for temporary use).

  void servant_orb (CORBA::ORB_ptr orb);
  // Accesor and mutator for the servant ORB.  Notice that the muatator
  // assumes the ownership of the passed in ORB and the accesor does not
  // return a copy of the orb since the accessing of the ORB is considered
  // temporary.

  void addressing_mode (CORBA::Short addr_mode);
  // Set the addressing mode.

  CORBA::Short addressing_mode (void);
  // Return the Addressing mode.

  CORBA::Boolean service_profile_selection (void);
  // Make a call on to services to see whether they have some
  // preferences on selecting the right profiles.

private:
  TAO_Profile *set_profile_in_use_i (TAO_Profile *pfile);
  // Makes a copy of the profile and frees the existing profile_in_use.
  // NOT THREAD SAFE

  void reset_base ();
  // NON-THREAD-SAFE.  Utility method which resets or initializes
  // the base_profile list and forward flags.

  void forward_back_one (void);
  // NON-THREAD-SAFE.  Utility method which unrolls (removes or pops)
  // the top most forwarding profile list.

  void reset_forward ();
   // NOT THREAD-SAFE.  Utility method which pops all forward profile
   // lists and resets the forward_profiles_ pointer.

  ~TAO_Stub (void);
  // Destructor is to be called only through _decr_refcnt().

  TAO_Profile *next_forward_profile (void);
  // NON-THREAD-SAFE.  utility method for next_profile.

#if (TAO_HAS_RT_CORBA == 1)

private:

  void parse_policies (void);
  // Helper method used to parse the policies.

  void exposed_priority_model (CORBA::Policy_ptr policy);

  void exposed_priority_banded_connection (CORBA::Policy_ptr policy);

  void exposed_client_protocol (CORBA::Policy_ptr policy);

private:

  // The following attribute are used to cache
  // the different kind of policies and avoid to
  // parse the MProfile's policy list each time we
  // are asked about a given policy.

  TAO_PriorityModelPolicy *priority_model_policy_;

  TAO_PriorityBandedConnectionPolicy *priority_banded_connection_policy_;

  TAO_ClientProtocolPolicy *client_protocol_policy_;

  CORBA::Boolean are_policies_parsed_;

#endif /* TAO_HAS_RT_CORBA == 1 */
private:

  TAO_MProfile base_profiles_;
  // Ordered list of profiles for this object.

  TAO_MProfile *forward_profiles_;
  // The list of forwarding profiles.  This is actually implemented as a
  // linked list of TAO_MProfile objects.

  TAO_Profile *profile_in_use_;
  // This is the profile that we are currently sending/receiving with.

  ACE_Lock* profile_lock_ptr_;
  // Mutex to protect access to the forwarding profile.

  size_t profile_success_;
  // Have we successfully talked to the forward profile yet?

  ACE_SYNCH_MUTEX refcount_lock_;
  // Mutex to protect reference count.

  CORBA::ULong refcount_;
  // Number of outstanding references to this object.

  TAO_ORB_Core* orb_core_;
  // The ORB.

  CORBA::ORB_var orb_;
  // ORB required for reference counting.  This will help us keep the
  // ORB around until the CORBA::Object we represent dies.

  CORBA::ORB_var servant_orb_;
  // If this stub refers to a collocated object then we need to hold on to
  // the servant's ORB (which may be different from the client ORB) so that,
  //   1. we know that the ORB will stay alive long enough, and,
  //   2. we can search for the servant/POA's status starting from
  //      the ORB's RootPOA.

  TAO_Policy_Manager_Impl *policies_;
  // The policy overrides in this object, if nil then use the default
  // policies.

  CORBA::Short addressing_mode_;
  // The addressing mode.

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
class TAO_Export TAO_Stub_Auto_Ptr
{
  // = TITLE
  //     Implements the draft C++ standard auto_ptr abstraction.
  //     This class allows one to work Stub Objects *Only*!
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
