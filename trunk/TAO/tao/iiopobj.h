// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    iiopobj.h
//
// = DESCRIPTION
//     IIOP objref representation
//
//     This allows stubs which support multiple protocols, since the
//     stub (and DII) code only work with the parent "STUB_Objref"
//     class when making calls.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
// 
// ============================================================================

#if !defined (TAO_IIOPOBJ_H)
#  define TAO_IIOPOBJ_H

#if 0
#  include "ace/OS.h"

#  include "tao/corbacom.h"
#  include "tao/sequence.h"
#  include "tao/object.h"
#  include "tao/stub.h"
#endif

// @@ Can we use the ACE_SYNCH_MUTEX here rather than these typedefs?
#  if !defined (ACE_HAS_THREADS)
typedef ACE_Null_Mutex ACE_Thread_Mutex;
#  endif /* ACE_HAS_THREADS */

class ACE_Svc_Export IIOP // namespace
{			
public:
  // IIOP Protocol version is distinct from GIOP version.

  enum { MY_MAJOR = 1, MY_MINOR = 0 };

  struct Version
  {
    CORBA::Octet	major;
    CORBA::Octet minor;

    Version (CORBA::Octet maj = MY_MAJOR, CORBA::Octet min = MY_MINOR);
  };

  // IOR support ... ProfileBody is encapsulated in an IIOP
  // profile entry within an IOR.

  struct ProfileBody
  {
    Version iiop_version;
    CORBA::String host;
    CORBA::UShort port;
    TAO_opaque object_key;

    ProfileBody (void);

    ProfileBody (const ProfileBody &src);
    ProfileBody (const Version &v, 
		 const CORBA::String &h, 
		 const CORBA::UShort &p, 
		 const TAO_opaque &object_key);

    ~ProfileBody (void);

  private:
    ProfileBody &operator = (const ProfileBody &src);
  };
};

// Representation of an IIOP objref: the profile body, and any
// forwarded pointer.  Implementations of basic invocation code; how
// to marshal an objref.  Contains a CORBA::Object interface.
//
// NOTE that this uses (single) implementation inheritance to share
// most of the basic code for an object reference.

// @@ What does it mean to have an 'extern "C"' object?!  This is so
// that it is visible to DCOM, which is a C linkage, I believe.  This
// is all holdover from the original DB code, and the COM integration
// is the least understood fo any of it.
class ACE_Svc_Export IIOP_Object : public STUB_Object
{
public:
  void do_call (CORBA::Environment &env,
		const TAO_Call_Data *info,
		...);
  // Stub-based invocation.

  void do_dynamic_call (const char *opname,
			CORBA::Boolean is_roundtrip,
			CORBA::NVList_ptr args,
			CORBA::NamedValue_ptr result,
			CORBA::Flags flags,
			CORBA::ExceptionList &exceptions,
			CORBA::Environment &env);
  // DII based invocation.

  // = Support for tables keyed by objrefs.

  CORBA::ULong hash (CORBA::ULong maximum,
		    CORBA::Environment &env);
  CORBA::Boolean	is_equivalent (CORBA::Object_ptr other_obj,
			       CORBA::Environment &env);

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

  IIOP::ProfileBody profile;

  // = Thread-safe accessors for the forwarding profile
  IIOP::ProfileBody *fwd_profile (void);
  // THREAD-SAFE.  Returns the current forwarding profile.
  
  IIOP::ProfileBody *fwd_profile (IIOP::ProfileBody *new_profile);
  // THREAD-SAFE.  Sets a new value for the forwarding profile and
  // returns the current value.

  // = Non-thread-safe accessors for the forwarding profile
  ACE_Thread_Mutex &fwd_profile_lock (void);
  // Gives reference to the lock guarding the forwarding profile.
  
  IIOP::ProfileBody *fwd_profile_i (void);
  // THREAD-SAFE.  Returns the current forwarding profile.
  
  IIOP::ProfileBody *fwd_profile_i (IIOP::ProfileBody *new_profile);
  // THREAD-SAFE.  Sets a new value for the forwarding profile and
  // returns the current value.

  // = Construction
  IIOP_Object (char *repository_id);
  IIOP_Object (char *repository_id,
               const IIOP::ProfileBody &profile);

  // = COM stuff
  ULONG __stdcall AddRef (void);
  ULONG __stdcall Release (void);
  HRESULT __stdcall QueryInterface (REFIID type_id, 
				    void **ppv);

  virtual CORBA::String _get_name (CORBA::Environment &env);

private:
  CORBA::Object base;

  ACE_Thread_Mutex IUnknown_lock_;
  // Mutex to protect <IUnknown>-related stuff.
  
  u_int refcount_;
  // Number of outstanding references to this object.

  ACE_Thread_Mutex fwd_profile_lock_;
  // This lock covers the mutable info in all IIOP objref data,
  // namely the forwarded-to objref.  It must be held when a client
  // thread is reading or modifying that data, to prevent one from
  // overwriting data the other's reading or writing.

  IIOP::ProfileBody *fwd_profile_;
  // This is a pointer to a profile used if the object is not
  // colocated in the current process.

  ~IIOP_Object (void);
  // Destructor is to be called only through Release()

  // = Disallow copy constructor and assignment operator

  IIOP_Object (const IIOP_Object &);
  operator = (const IIOP_Object &);

#if defined (__GNUG__)
  // G++ (even 2.6.3) stupidly thinks instances can't be created.
  // This de-warns.
  friend class everyone_needs_a_friend;
#endif /* __GNUG__ */
};

#endif	/* TAO_IIOPOBJ_H */
