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
//    These constitute the stub API to this "ORB Core".  Such
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

#include "tao/Pluggable.h"
#include "tao/MProfile.h"
#include "tao/ORB.h"

class TAO_GIOP_Invocation;
class TAO_ORB_Core;

// Descriptions of parameters.

enum TAO_Param_Type
{
  // = TITLE
  //   TAO_Param_Type
  // =DESCRIPTION
  // Parameter mode of a given parameter.
  PARAM_IN,
  PARAM_OUT,
  PARAM_INOUT,
  PARAM_RETURN  // = PARAM_OUT
};

class TAO_Profile;

struct TAO_Param_Data
{
  // = TITLE
  //   TAO_Param_Data
  //
  // = DESCRIPTION
  //   Description of a single parameter.
  //
  //   If value_size is nonzero for OUT, INOUT, or RETURN parameters,
  //   it's (a) an indicator that the ORB returns a pointer-to-value
  //   for this parameter, and also (b) is the size of the top-level
  //   of the value that's returned (e.g. ignoring nested sequence
  //   buffers).  That is, it moves CPU cycles from runtime -- some
  //   calls to tc->size() -- to compile time where they're
  //   cheap/free.
  //
  //   It _must_ only be set for cases where the ORB allocates the
  //   return value, which must then be ORB::free()d ... e.g. where
  //   the value is a pointer to data such as a struct, sequence, or
  //   union.  (The CORBA C++ mapping doesn't require that for all
  //   "out" structs; only those of "variable size".)  If this value
  //   is nonzero, the value passed to do_static_call() must be the address
  //   of a pointer.

  CORBA::TypeCode_ptr tc;
  // TypeCode for the parameter

  TAO_Param_Type mode;
  // Its mode.

  size_t value_size;
  // zero or tc->size (). For SII, we always know its size since it is the IDL
  // compiler which generates the stub code.
};

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

struct TAO_Call_Data
{
  // = TITLE
  //   TAO_Call_Data
  //
  // = DESCRIPTION
  //   Descriptions of operations, as used by the stub interpreter.
  //   Only interpretive marshaling/unmarshaling is used, and the
  //   stubs don't know what particular on-the-wire protocol is being
  //   used.
  //
  //   When using C++ exceptions, many C++ compilers will require the
  //   use of compiled code throw the exception.  As binary standards
  //   for exception throwing evolve, it may become practical to
  //   interpretively throw exceptions.

  const char *opname;
  // Operation name.

  CORBA::Boolean is_roundtrip;
  // !oneway

  // When constructing tables of parameters, put them in the same
  // order they appear in the IDL spec: return value, then parameters
  // left to right.  Other orders may produce illegal protocol
  // messages.

  u_int param_count;
  // # parameters.

  const TAO_Param_Data *params;
  // Their descriptions.

  // The exceptions allowed by this operation can be listed in any
  // order, and need not include the system exceptions which may be
  // returned by OMG-IDL operations.  If an operation tries to return
  // any exception not allowed by its type signature, this causes a a
  // system exception to be reported.

  u_int except_count;
  // # exceptions.

  //  CORBA::TypeCode_ptr *excepts;
  TAO_Exception_Data *excepts;
  // Their descriptions.
};

struct TAO_Skel_Entry
{
  // = TITLE
  //   TAO_Skel_Entry
  //
  // = DESCRIPTION
  //   Skeletons map "ServerRequest" generic signatures to the static
  //   call signature required by the implementation's methods.  table
  //   of these per implementation
  //
  //   There are several optimizations that'd be desirable for use by
  //   "static skeletons", notably (a) passing of per-object data held
  //   by the OA so that the method doesn't need to look it up itself,
  //   (b) removing all mandatory heap allocation of data, and of
  //   course (c) handling all the built-in ORB calls like "is_a" and
  //   "get_implementation".  This code is currently set up only for
  //   Dynamic Skeletons and bridging, for which none of those are
  //   real issues.

  const TAO_Call_Data *op_descriptor;
  // pointer to the calldata structure that holds information about all the
  // parameters

  TAO_Skeleton impl_skeleton;
  // skeleton corresponding to the operation
};

class TAO_Export STUB_Object
{
  // = TITLE
  //   STUB_Object
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
  void do_static_call (CORBA_Environment &TAO_IN_ENV,
                               const TAO_Call_Data *info,
                               void** args);
  // The "stub interpreter" method parameters are:
  //
  //    - TAO_IN_ENV ... used for exception reporting
  //    - info ... describes the call
  //    - args parameters follow
  //
  // The varargs parameters are pointers to data instances as
  // described by info->params.  (If the value_size is nonzero, the
  // data instance is itself a pointer.)  The order on the call stack
  // must be exactly the order they're listed in info->params;
  // normally this is the order the values are listed in the OMG-IDL
  // operation specification.
  //
  // NOTE: This can be sped up by eliminating some parameter
  // shuffling.  The stub and "do_static_call" parameters are all but the
  // same, except that their order is different.

#if !defined (TAO_HAS_MINIMUM_CORBA)

  void do_dynamic_call (const char *opname,
                        CORBA::Boolean is_roundtrip,
                        CORBA::NVList_ptr args,
                        CORBA::NamedValue_ptr result,
                        CORBA::Flags flags,
                        CORBA::ExceptionList &exceptions,
                        CORBA_Environment &TAO_IN_ENV =
                              CORBA::default_environment ());
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
  //    - TAO_IN_ENV ... used for exception reporting.

#endif /* TAO_HAS_MINIMUM_CORBA */

  CORBA::String_var type_id;
  // All objref representations carry around a type ID.

  CORBA::ULong hash (CORBA::ULong maximum,
          CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // All objref representations know how to hash themselves and
  // compare themselves for equivalence to others.  It's easily
  // possible to have two objrefs that are distinct copies of data
  // that refers/points to the same remote object (i.e. are
  // equivalent).

  CORBA::Boolean is_equivalent (CORBA::Object_ptr other_obj,
        CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // XXX All objref representations should know how to marshal
  // themselves.  That will involve ensuring that the IOR that gets
  // marshaled talks a specific protocol, otherwise the target of a
  // message would not be invoke using the objref it receives
  // (compromising functionality in a very basic and mysterious
  // mannter).  So for example an objref might need to create a proxy
  // for itself rather than marshaling its own representation.  [ The
  // IIOP engine does not need to worry about such issues since it
  // only supports one protocol -- the problem won't show up.
  // "Multiprotocol ORBs" will need to solve that problem though.  ]

  // Our Constructors ...

  STUB_Object (char * repository_id);
  // XXX All objref representations should know how to marshal
  // themselves.  That will involve ensuring that the IOR that gets
  // marshaled talks a specific protocol, otherwise the target of a
  // message would not be invoke using the objref it receives
  // (compromising functionality in a very basic and mysterious
  // manner).  So for example an objref might need to create a proxy
  // for itself rather than marshaling its own representation.  [ The
  // IIOP engine does not need to worry about such issues since it
  // only supports one protocol -- the problem won't show up.
  // "Multiprotocol ORBs" will need to solve that problem though.  ]

  STUB_Object (char *repository_id,
               TAO_Profile *profile);
  // degenerate case where only one profile is wanted.  This method
  // is depricated and is here ONLY for compatibility with multiple
  // profile unfriendly code!  The profile is given to the MProfile
  // object.

  STUB_Object (char *repository_id,
               TAO_MProfile *profiles);
  // Construct from a repository ID and a list of profiles.

  STUB_Object (char *repository_id,
               TAO_MProfile &profiles);
  // Construct from a repository ID and a profile ID.profile ID.

  // = Memory management.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  // Manage the base (non-forwarded) profiles.
  TAO_Profile *profile_in_use (void) ;
  // returns a pointer to the profile_in_use object.  This object
  // retains ownership of this profile.

  TAO_MProfile *get_profiles (void);
  // Copy of the profile list, user must free memory when done.
  // although the user can call get_profiles then reorder
  // the list and give it back to STUB_Object.

  const TAO_MProfile& get_base_profiles (void) const;
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

   TAO_Profile *set_base_profiles (TAO_MProfile *mprofiles);
   // Initialize the base_profiles_ and set profile_in_use_ to
   // reference the first profile.

  void add_forward_profiles (TAO_MProfile *mprofiles);
  // THREAD SAFE.
  // set the forward_profiles.  This object will assume ownership of
  // this TAO_MProfile object!!

  CORBA::Boolean next_profile_retry (void);
  // THREAD SAFE
  // used to get the next profile after the one being used has
  // failed during the initial connect or send of the message!

protected:
  void put_params (CORBA_Environment &TAO_IN_ENV,
                   const TAO_Call_Data *info,
                   TAO_GIOP_Invocation &call,
                   void** args);
  // Helper method to factor out common code in static oneway
  // vs. twoway invocations.

#if !defined (TAO_HAS_MINIMUM_CORBA)

  void put_params (TAO_GIOP_Invocation &call,
                   CORBA::NVList_ptr args,
                   CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
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

  ~STUB_Object (void);
  // Destructor is to be called only through _decr_refcnt()

  TAO_Profile *next_forward_profile (void);
  // NON-THREAD-SAFE.  utility method for next_profile.

private:
    // @@ For now, we keep track of transport specific profiles here,
  //    but in the next iteration this will go away ... only transport
  //    neutral info is kept here => STUB_Object should also go away!
  //    fredk
  TAO_MProfile     base_profiles_;
  // ordered list of profiles for this object.
  TAO_MProfile     *forward_profiles_;

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

  // = Disallow copy constructor and assignment operator
  ACE_UNIMPLEMENTED_FUNC (STUB_Object (const STUB_Object &))
  ACE_UNIMPLEMENTED_FUNC (STUB_Object &operator = (const STUB_Object &))

#if defined (__GNUG__)
  // G++ (even 2.6.3) stupidly thinks instances can't be created.
  // This de-warns.
  friend class everyone_needs_a_friend;
#endif /* __GNUG__ */
};

#if defined (__ACE_INLINE__)
# include "tao/Stub.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_STUB_H */
