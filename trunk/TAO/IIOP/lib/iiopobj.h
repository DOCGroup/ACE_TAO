// This may look like C, but it's really -*- C++ -*-
//
// @(#) $Id$
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// IIOP		objref representation
//
// This allows stubs which support multiple protocols, since the
// stub (and DII) code only work with the parent "STUB_Objref"
// class when making calls.
//

#if !defined(TAO_IIOPOBJ_H)
#  define TAO_IIOPOBJ_H

#  include "ace/OS.h"

#  if !defined(ACE_HAS_THREADS)
typedef ACE_Null_Mutex ACE_Thread_Mutex;
#  endif

typedef CORBA_SEQUENCE <CORBA_Octet> opaque;

class ACE_Svc_Export IIOP
{			// namespace
public:
  //
  // IIOP Protocol version is distinct from GIOP version.
  //
  enum { MY_MAJOR = 1, MY_MINOR = 0 };

  struct Version
  {
    CORBA_Octet	major;
    CORBA_Octet minor;

    Version(CORBA_Octet maj = MY_MAJOR, CORBA_Octet min = MY_MINOR);
  };

  //
  // IOR support ... ProfileBody is encapsulated in an IIOP
  // profile entry within an IOR.
  //
  struct ProfileBody
  {
    Version			iiop_version;
    CORBA_String		host;
    CORBA_UShort		port;
    opaque			object_key;

    ProfileBody ();

    ProfileBody (const ProfileBody &src);
    ProfileBody (const Version& v, const CORBA_String& h, const CORBA_UShort& p, const opaque& object_key);

    ~ProfileBody ();

  private:
    ProfileBody		&operator = (const ProfileBody &src);
  };
};


//
// Representation of an IIOP objref:  the profile body, and any
// forwarded pointer.  Implementations of basic invocation code;
// how to marshal an objref.  Contains a CORBA::Object interface.
//
// NOTE that this uses (single) implementation inheritance to share
// most of the basic code for an object reference.
//
extern "C" const IID		IID_IIOP_Object;

class ACE_Svc_Export IIOP_Object : public STUB_Object
{
public:
  //
  // stub-based invocation
  //
  void		do_call (
			 CORBA_Environment		&env,
			 const calldata		*info,
			 ...
			 );

  //
  // DII based invocation
  //
  void		do_dynamic_call (
				 const char			*opname,
				 CORBA_Boolean		is_roundtrip,
				 CORBA_NVList_ptr		args,
				 CORBA_NamedValue_ptr	result,
				 CORBA_Flags			flags,
				 CORBA_ExceptionList		&exceptions,
				 CORBA_Environment		&env
				 );

  //
  // Support for tables keyed by objrefs.
  //
  CORBA_ULong		hash (CORBA_ULong maximum,
			      CORBA_Environment &env);
  CORBA_Boolean	is_equivalent (CORBA_Object_ptr other_obj,
			       CORBA_Environment &env);

  //
  // XXX All objref representations should know how to marshal themselves.
  // That will involve ensuring that the IOR that gets marshaled talks a
  // specific protocol, otherwise the target of a message would not be
  // invoke using the objref it receives (compromising functionality in
  // a very basic and mysterious mannter).  So for example an objref might
  // need to create a proxy for itself rather than marshaling its own
  // representation.  [ The IIOP engine does not need to worry about such
  // issues since it only supports one protocol -- the problem won't show
  // up.  "Multiprotocol ORBs" will need to solve that problem though.  ]
  //

  IIOP::ProfileBody		profile;
  IIOP::ProfileBody		*fwd_profile;

  IIOP_Object (char *repository_id);
  IIOP_Object (char* repository_id,
               IIOP::ProfileBody profile);

  //
  // COM stuff
  //
  ULONG __stdcall AddRef ();
  ULONG __stdcall Release ();
  HRESULT __stdcall QueryInterface (REFIID type_id, void **ppv);

  virtual CORBA_String  _get_name(CORBA_Environment &env);

private:
  CORBA_Object		base;
  ACE_Thread_Mutex lock_;
  unsigned			_refcount;

  //
  // Destructor is to be called only through Release()
  //
  ~IIOP_Object ();

  //
  // Disallow copy constructor and assignment operator
  //
  IIOP_Object (const IIOP_Object &);
  operator = (const IIOP_Object &);
#if	defined (__GNUG__)
  //
  // G++ (even 2.6.3) stupidly thinks instances can't be
  // created.  This de-warns.
  //
  friend class everyone_needs_a_friend;
#endif
};

#  if defined(__ACE_INLINE__)
#    include "iiopobj.i"
#  endif

#endif	// _iiopobj_hh
