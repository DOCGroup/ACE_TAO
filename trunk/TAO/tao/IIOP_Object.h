// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    IIOP_Object.h
//
// = DESCRIPTION
//     IIOP objref representation.
//
//     This allows stubs which support multiple protocols, since the
//     stub (and DII) code only work with the parent "STUB_Objref"
//     class when making calls.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//
// ============================================================================

#ifndef TAO_IIOPOBJ_H
#  define TAO_IIOPOBJ_H

class TAO_GIOP_Invocation;

class TAO_Export IIOP
{
  // = TITLE
  //   This class provides a namespace.
public:
  // = IIOP Protocol version is distinct from GIOP version.
  enum
  {
    MY_MAJOR = 1,
    MY_MINOR = 0
  };

  struct Version
  {
    CORBA::Octet major;
    CORBA::Octet minor;

    Version (CORBA::Octet maj = MY_MAJOR,
             CORBA::Octet min = MY_MINOR);
  };

  struct TAO_Export Profile
    // = TITLE
    // IOR support ... Profile is encapsulated in an IIOP profile
    // entry within an IOR.  Note that this structure is specified
    // by CORBA 2.0, so we can't screw with it too much.
  {
    Version iiop_version;
    TAO_opaque object_key;
    char *host;
    CORBA::UShort port;

    Profile (void);
    // Default constructor

    Profile (const char *host,
             CORBA::UShort port,
             const TAO_opaque &object_key,
             const ACE_INET_Addr &addr);
    // Called by server.

    Profile (const Profile &src);
    // Copy constructor.

    ~Profile (void);
    // Destructor.

    void reset_object_addr (void);
    // Sets <object_addr_> cache from <host> and <port>

    ACE_INET_Addr &object_addr (void);
    // Returns the <ACE_INET_Addr> for this profile.

    Profile &operator= (const Profile &src);
    // copy operator

    int operator== (const Profile &src);
    // comparison

  protected:

    void set (const char *h,
              CORBA::UShort p,
              const TAO_opaque &key,
              const ACE_INET_Addr &addr);
    // Workhorse

    ACE_INET_Addr object_addr_;
    // Cached instance of <ACE_INET_Addr> for use in making
    // invocations, etc.
  };
};

class TAO_Export IIOP_Object : public STUB_Object
{
  // = TITLE
  //   Representation of an IIOP objref: the profile body, and any
  //   forwarded pointer.  Implementations of basic invocation code;
  //   how to marshal an objref.  Contains a CORBA::Object interface.
  //
  // = DESCRIPTION
  //   NOTE that this uses (single) implementation inheritance to share
  //   most of the basic code for an object reference.
  //
  //   This implementation provides DII support, and an analagous
  //   interpreter that let  static stubs be very small.  It's
  //   specific to objrefs with IIOP::Profile.
public:
  virtual void do_static_call (CORBA_Environment &TAO_IN_ENV,
                               const TAO_Call_Data *info,
                               void** args);
  // SII-based "Stub interpreter" for static stubs.  IDL compiler just
  // dumps a read-only description of the call into "calldata" and do
  // varargs calls to this routine, which does all the work.

  virtual void do_dynamic_call (const char *opname,
                                CORBA::Boolean is_roundtrip,
                                CORBA::NVList_ptr args,
                                CORBA::NamedValue_ptr result,
                                CORBA::Flags flags,
                                CORBA::ExceptionList &exceptions,
                                CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());
  // DII-based invocation analogue of the <do_static_call> above.
  // Differs in how the vararg calling convention is implemented --
  // DII doesn't use the normal call stack with its implicit typing,
  // but instead uses heap-based arguments with explicit typing.

  // = Support for tables keyed by objrefs.

  CORBA::ULong hash (CORBA::ULong maximum,
                     CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());
  // Compute a hash value for the object.

  CORBA::Boolean is_equivalent (CORBA::Object_ptr other_obj,
                                CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());
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

  // = Construction
  IIOP_Object (char *repository_id);
  // Construct from a repository (type) ID.

  IIOP_Object (char *repository_id,
               const char *host,
               CORBA::UShort port,
               const TAO_opaque &object_key,
               const ACE_INET_Addr &addr);
  // Construct from a repository ID and IIOP profile information.

  // = Memory management.
  virtual CORBA::ULong _incr_refcnt (void);
  virtual CORBA::ULong _decr_refcnt (void);

  virtual TAO_ObjectKey *key (CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());
  // Return the object key as an out parameter.  Caller should release
  // return value when finished with it.

  IIOP::Profile profile;
  // Profile for this object.

  IIOP::Profile *get_fwd_profile (void);
  // THREAD-SAFE.  Returns the current forwarding profile.

  IIOP::Profile *get_fwd_profile_i (void);
  // NON-THREAD-SAFE.  Returns the current forwarding profile.

  IIOP::Profile *set_fwd_profile (IIOP::Profile *new_profile);
  // THREAD-SAFE.  Sets a new value for the forwarding profile and
  // returns the current value.

  ACE_Lock &get_fwd_profile_lock (void);
  // Gives reference to the lock guarding the forwarding profile.

  void reset_first_locate_request (void);
  // reset the flag telling that the locate request should be used

  void use_locate_requests (CORBA::Boolean use_it);
  // set the flags to use locate_requests.

  TAO_Client_Connection_Handler *&handler (void);
  // Return the <handler_> pointer by reference.

  void reset_handler (void);
  // Reset the <handler_>.  Usually used on errors.

protected:
  void put_params (CORBA_Environment &TAO_IN_ENV,
                   const TAO_Call_Data *info,
                   TAO_GIOP_Invocation &call,
                   void** args);
  // Helper method to factor out common code in static oneway
  // vs. twoway invocations.

  void put_params (TAO_GIOP_Invocation &call,
                   CORBA::NVList_ptr args,
                   CORBA_Environment &TAO_IN_ENV = CORBA_Environment::default_environment ());
  // Helper method to factor out common code in dynamic oneway
  // vs. twoway invocations.

protected:
  IIOP::Profile *fwd_profile_;
  // Store the forwarding profile

  ACE_Lock* fwd_profile_lock_ptr_;
  // Mutex to protect access to the forwarding profile

  size_t fwd_profile_success_;
  // Have we successfully talked to the forward profile yet?

  ACE_SYNCH_MUTEX refcount_lock_;
  // Mutex to protect reference count

  CORBA::ULong refcount_;
  // Number of outstanding references to this object.

  CORBA::Boolean use_locate_request_;
  // set if locate request should be used

  CORBA::Boolean first_locate_request_;
  // distinguishes the first from following calls

  TAO_Client_Connection_Handler *handler_;
  // This handler is going to be used to keep track of the last client
  // connection handler used by the stub.  It is also used as a "hint"
  // to the cached connector.  Note that all changes to this pointer
  // are made by the cached connector, i.e., under the lock of the
  // cached connector. Don't modify this pointer at will except in the
  // case of error, in which case should be set to zero.

  ~IIOP_Object (void);
  // Destructor is to be called only through _decr_refcnt()

  // = Disallow copy constructor and assignment operator
  ACE_UNIMPLEMENTED_FUNC (IIOP_Object (const IIOP_Object &))
  ACE_UNIMPLEMENTED_FUNC (IIOP_Object &operator = (const IIOP_Object &))

#if defined (__GNUG__)
  // G++ (even 2.6.3) stupidly thinks instances can't be created.
  // This de-warns.
  friend class everyone_needs_a_friend;
#endif /* __GNUG__ */
};

#if defined (__ACE_INLINE__)
# include "tao/IIOP_Object.i"
#endif /* __ACE_INLINE__ */

#endif  /* TAO_IIOPOBJ_H */
