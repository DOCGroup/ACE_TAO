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

// Forward declarations.

class TAO_RelativeRoundtripTimeoutPolicy;
class TAO_Client_Priority_Policy;
class TAO_Sync_Scope_Policy;
class TAO_Buffering_Constraint_Policy;

class TAO_Sync_Strategy;
class TAO_GIOP_Invocation;
class TAO_ORB_Core;
class TAO_Policy_Manager_Impl;

// Descriptions of parameters.

enum TAO_Param_Type
{
  // = TITLE
  //   TAO_Param_Type
  //
  // = DESCRIPTION
  //   Parameter mode of a given parameter.
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
  //   TAO_Exception_Data
  //
  // = DESCRIPTION
  //   Description of a single exception
  //
  //   The interpreter needs a way to allocate memory to hold the exception
  //   that was raised by the stub. This data structure provides the typecode
  //   for the exception as well as a static function pointer that does the job
  //   of memory allocation.

  CORBA::TypeCode_ptr tc;
  // typecode describing the exception

  TAO_Exception_Alloc alloc;
  // the allocator for this exception
};

class TAO_Export TAO_Stub
{
  // = TITLE
  //   TAO_Stub
  //
  // = DESCRIPTION
  //   Per-objref data includes the (protocol-specific) Profile, which
  //   is handled by placing it into a subclass of this type along
  //   with data that may be used in protocol-specific caching
  //   schemes.
  //
  //   The type ID (the data specified by CORBA 2.0 that gets exposed
  //   "on the wire", and in stringified objrefs) is held by this
  //   module.
  //
  //   The stub and DII interpreter APIs are member functions of this
  //   type.
public:
#if (TAO_HAS_MINIMUM_CORBA == 0)

  void do_dynamic_call (const char *opname,
                        CORBA::Boolean is_roundtrip,
                        CORBA::NVList_ptr args,
                        CORBA::NamedValue_ptr result,
                        CORBA::Flags flags,
                        CORBA::ExceptionList &exceptions,
                        int lazy_evaluation,
                        CORBA_Environment &ACE_TRY_ENV =
                              TAO_default_environment ());
  // Dynamic invocations use a more costly "varargs" calling
  // convention; it's got the same input data as the (static)
  // stub-oriented one, but the data is represented somewhat
  // differently.
  //
  // Operation-specific data isn't collected in a stack frame and into
  // a static/readonly "calldata" structure, but instead is collected
  // into these parameters, the majority of which are heap-allocated:
  //
  //    - opname ... the name of the operation
  //    - is_roundtrip ... true except for oneway operations, or ones where
  //      the client doesn't care about responses
  //    - args ... call parameters and their descriptions
  //    - result ... result and its description
  //    - flags ... only one DII flag is legal
  //    - exceptions ... list of legal user-defined exceptions
  //    - ACE_TRY_ENV ... used for exception reporting.

  // Used with DII deferred synchronous requests.
  void do_deferred_call (const CORBA::Request_ptr req,
                         CORBA_Environment &ACE_TRY_ENV =
                              TAO_default_environment ());

#endif /* TAO_HAS_MINIMUM_CORBA */

#if (TAO_HAS_CORBA_MESSAGING == 1)

  CORBA::Policy_ptr get_policy (
      CORBA::PolicyType type,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  CORBA::Policy_ptr get_client_policy (
      CORBA::PolicyType type,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  TAO_Stub* set_policy_overrides (
      const CORBA::PolicyList & policies,
      CORBA::SetOverrideType set_add,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  CORBA::PolicyList * get_policy_overrides (
      const CORBA::PolicyTypeSeq & types,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  CORBA::Boolean validate_connection (
      CORBA::PolicyList_out inconsistent_policies,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

  TAO_RelativeRoundtripTimeoutPolicy *relative_roundtrip_timeout (void);

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  TAO_Client_Priority_Policy *client_priority (void);

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

  TAO_Sync_Scope_Policy *sync_scope (void);

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  TAO_Buffering_Constraint_Policy *buffering_constraint (void);

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

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

  TAO_Stub (char *repository_id,
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

  // manage forward and base profiles.
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

  // Just forward profiles.
  void use_locate_requests (CORBA::Boolean use_it);
  // set the flags to use locate_requests.

  void set_valid_profile (void);
   // NON-THREAD-SAFE.  Will set profile_success_ to 0.

   CORBA::Boolean valid_profile (void);
   // returns TRUE if a connection was successful with at least
   // one profile.

   TAO_Profile *base_profiles (const TAO_MProfile& mprofiles);
   // Initialize the base_profiles_ and set profile_in_use_ to
   // reference the first profile.

  void add_forward_profiles (const TAO_MProfile &mprofiles);
  // THREAD SAFE.
  // set the forward_profiles.  This object will assume ownership of
  // this TAO_MProfile object!!

  CORBA::Boolean next_profile_retry (void);
  // THREAD SAFE
  // used to get the next profile after the one being used has
  // failed during the initial connect or send of the message!

  TAO_ORB_Core* orb_core (void) const;
  // Accessor

  CORBA::ORB_ptr servant_orb_ptr (void);
  // This returns a duplicated ORB pointer.

  CORBA::ORB_var &servant_orb_var (void);
  // This returns the ORB var itself (generally for temporary use).

  void servant_orb (CORBA::ORB_ptr orb);
  // Accesor and mutator for the servant ORB.  Notice that the muatator
  // assumes the ownership of the passed in ORB and the accesor does not
  // return a copy of the orb since the accessing of the ORB is considered
  // temporary.

protected:
#if (TAO_HAS_MINIMUM_CORBA == 0)

  void put_params (TAO_GIOP_Invocation &call,
                   CORBA::NVList_ptr args,
                   CORBA_Environment &ACE_TRY_ENV =
                         TAO_default_environment ());
  // Helper method to factor out common code in dynamic oneway
  // vs. twoway invocations.

#endif /* TAO_HAS_MINIMUM_CORBA */

private:
  TAO_Profile *set_profile_in_use_i (TAO_Profile *pfile);
  // Makes a copy of the profile and frees the existing profile_in_use.
  // NOT THREAD SAFE

  void reset_first_locate_request (void);
  // NON-THREAD-SAFE.
  // reset the flag telling that the locate request should be used

  void reset_base ();
  // NON-THREAD-SAFE.  utility method which resets or initializes
  // the base_profile list and forward flags.

  void forward_back_one (void);
  // NON-THREAD-SAFE.  utility method which unrolls (removes or pops)
  // the top most forwarding profile list.

  void reset_forward ();
   // NOT THREAD-SAFE.  utility method which pops all forward profile
   // lists and resets the forward_profiles_ pointer.

  ~TAO_Stub (void);
  // Destructor is to be called only through _decr_refcnt()

  TAO_Profile *next_forward_profile (void);
  // NON-THREAD-SAFE.  utility method for next_profile.

private:
  TAO_MProfile base_profiles_;
  // ordered list of profiles for this object.

  TAO_MProfile *forward_profiles_;
  // The list of forwarding profiles.  This is actually implemented as a
  // linked list of TAO_MProfile objects.

  TAO_Profile *profile_in_use_;
  // this is the profile that we are currently sending/receiving with

  ACE_Lock* profile_lock_ptr_;
  // Mutex to protect access to the forwarding profile

  size_t profile_success_;
  // Have we successfully talked to the forward profile yet?

  ACE_SYNCH_MUTEX refcount_lock_;
  // Mutex to protect reference count

  CORBA::ULong refcount_;
  // Number of outstanding references to this object.

  CORBA::Boolean use_locate_request_;
  // set if locate request should be used

  CORBA::Boolean first_locate_request_;
  // distinguishes the first from following calls

  TAO_ORB_Core* orb_core_;
  // The ORB

  CORBA::ORB_var orb_;
  // ORB required for reference counting.  This will help us keep the
  // ORB around until the CORBA::Object we represent dies.

  CORBA::ORB_var servant_orb_;
  // If this stub refers to a collocated object then we need to hold on to
  // the servant's ORB (which may be different from the client ORB) so that,
  //   1. we know that the ORB will stay alive long enough, and,
  //   2. we can search for the servant/POA's status starting from
  //      the ORB's RootPOA.

#if (TAO_HAS_CORBA_MESSAGING == 1)

  TAO_Policy_Manager_Impl *policies_;
  // The policy overrides in this object, if nil then use the default
  // policies.

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  // = Disallow copy constructor and assignment operator
  ACE_UNIMPLEMENTED_FUNC (TAO_Stub (const TAO_Stub &))
  ACE_UNIMPLEMENTED_FUNC (TAO_Stub &operator = (const TAO_Stub &))

#if defined (__GNUG__)
  // G++ (even 2.6.3) stupidly thinks instances can't be created.
  // This de-warns.
  friend class everyone_needs_a_friend;
#endif /* __GNUG__ */
};

// Define a TAO_Stub auto_ptr class
class TAO_Stub_Auto_Ptr
{
  // = TITLE
  //     Implements the draft C++ standard auto_ptr abstraction.
  //     This class allows one to work Stub Objects *Only*!
public:
  // = Initialization and termination methods
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
