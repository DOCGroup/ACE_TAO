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
               const TAO_Profile *profile);
  // Construct from a repository ID and a profile ID.

  IIOP_Object (char *repository_id,
               const TAO_Profile &profile);
  // Construct from a repository ID and a profile ID.

  // @@ IIOP_Object should take a IIOP_Profile object and not
  // this connection specific information like hostname, port etc.
  IIOP_Object (const char *host,
               const CORBA::UShort p,
               const TAO_ObjectKey &objkey,
               char *repository_id = 0);
  // This constructor will usually be used by the client side.

  IIOP_Object (char *repository_id,
               const ACE_INET_Addr &addr,
               const TAO_ObjectKey &objkey);
  // Constructor used typically by the server side.

  // = Memory management.
  virtual CORBA::ULong _incr_refcnt (void);
  virtual CORBA::ULong _decr_refcnt (void);

  // @@ Doesn't belong here!! This is profile specific, thus it is now
  //    kept in the Profile Object!  fredk
  // TAO_ObjectKey *key (CORBA_Environment &_env = CORBA_Environment::default_environment ());
  // Return the object key as an out parameter.  Caller should release
  // return value when finished with it.

  TAO_Profile *set_profile_in_use (void);

  TAO_Profile *set_profile_in_use (TAO_Profile *pfile);
  // Makes a copy of the profile and frees the existing profile_in_use.

  // @@ replaces direct acces to the iiop_profile, the OLD way of
  //    of doing it!  fredk
  TAO_Profile *profile_in_use(void);
  // returns a pointer to the profile_in_use object.  This object
  // retains ownership of this object.

  virtual TAO_Profile *get_profile(void);
  // returns null if profile_in_use == null
  // otherwise return a pointer to a new profile object.
  // The caller is responsible for freeing this memory!

  TAO_Profile *get_fwd_profile (void);
  //@@ IIOP::Profile *get_fwd_profile (void);
  // THREAD-SAFE.  Returns the current forwarding profile.

  TAO_Profile *get_fwd_profile_i (void);
  //@@ IIOP::Profile *get_fwd_profile_i (void);
  // NON-THREAD-SAFE.  Returns the current forwarding profile.

  TAO_Profile *set_fwd_profile (const TAO_Profile *new_profile);
  //@@ IIOP::Profile *set_fwd_profile (IIOP::Profile *new_profile);
  // THREAD-SAFE.  Sets a new value for the forwarding profile and
  // returns the current value.

  ACE_Lock &get_fwd_profile_lock (void);
  // Gives reference to the lock guarding the forwarding profile.

  void reset_first_locate_request (void);
  // reset the flag telling that the locate request should be used

  void use_locate_requests (CORBA::Boolean use_it);
  // set the flags to use locate_requests.

  // Previously, the handler pointer was stored here and used as the
  // hint when the connect was performed.  After the connect it was used
  // for communicating with the handler.  Now, these operations take
  // place via the *selected* profile.  Thus there is a 
  // profile->{hint(),cleanup_hint(),reset_hint(),idle()}.  reads and
  // writes the the underlying socket are performed using the new
  // transport object. profile->transport ().{receive(),send()}
  // 
  // NOTE, After the connection is successful, the selected profile will
  // be referenece in 
  // profile_in_use_, profile_in_use()

  //TAO_Client_Connection_Handler *&handler (void);
  //@@ void reset_handler (void);
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
  // @@ For now, we keep track of transport specific profiles here,
  //    but in the next iteration this will go away ... only transport
  //    neutral info is kept here => IIOP_Object should also go away!
  //    fredk
  TAO_IIOP_Profile *profile_;
  TAO_IIOP_Profile *profile_in_use_;
  // this is the profile that we are currently sending/receiving with

  TAO_IIOP_Profile *fwd_profile_;
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

  // TAO_Client_Connection_Handler *handler_;
  // This handler is going to be used to keep track of the last client
  // connection handler used by the stub.  It is also used as a "hint"
  // to the cached connector.  Note that all changes to this pointer
  // are made by the cached connector, i.e., under the lock of the
  // cached connector. Don't modify this pointer at will except in the
  // case of error, in which case should be set to zero.
  // @@ Change this in order to support pluggable protocols, i.e. multiple
  //    transport layer protocols at the GIOP level.  Replace this with
  //    a refernece to the TAO_IIOP_Connector object which will reference
  //    both the new TAO_IIOP_Transport and specific service handler. fredk

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
