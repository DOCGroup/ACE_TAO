// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    orbobj.h
//
// = DESCRIPTION
//     Header file for Win32 interface to CORBA's "ORB" type.
//
//     The "ORB" pseudo-object is used in bootstrapping, such as to
//     create object references from strings.  It's also used to
//     create strings from object references.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
// 
// ============================================================================

#if !defined (TAO_ORBOBJ_H)
#  define TAO_ORBOBJ_H

#  include "ace/OS.h"
#  include "factories.h"
#  include "params.h"

// Can't have forward decls if we're doing inlines of these functions
ACE_INLINE void CORBA_release (CORBA_ORB_ptr orb);
ACE_INLINE CORBA_Boolean CORBA_is_nil (CORBA_ORB_ptr orb);

extern const IID 		IID_CORBA_ORB;

// @@ Shoudn't this be hidden within a namespace?  According to the
// spec, CORBA_ORB_init() is a valid signature for this function, as
// is CORBA::ORB_init(), as is (I think) CORBA::ORB::init().  It *IS*
// hidden within the CORBA namespace.  It's just that it uses the
// underscore as the namespace separator since we don't have the
// luxury of real C++ namespaces. --cjc

// ORB initialisation, per OMG document 94-9-46.

CORBA_ORB_ptr ACE_Svc_Export
CORBA_ORB_init (int &argc,
		char *const *argv,
		char *orb_name,
		CORBA_Environment &env);

class ACE_Svc_Export CORBA_ORB : public IUnknown
// = TITLE
// ORB pseudo-objref
{
public:
  CORBA_BOA_ptr BOA_init (int &argc, 
			  char **argv, 
			  const char *boa_identifier = 0);
  static CORBA_ORB_ptr _duplicate (CORBA_ORB_ptr orb);
  static CORBA_ORB_ptr _nil (void);

  virtual CORBA_Object_ptr string_to_object (CORBA_String str,
                                             CORBA_Environment &env) = 0;
  virtual CORBA_String object_to_string (CORBA_Object_ptr obj,
                                         CORBA_Environment &env) = 0;

  // similar for TypeCodes and Anys ... to/from octet sequences

  void create_list (CORBA_Long count,
                    CORBA_NVList_ptr &retval);

  // Stuff required for COM IUnknown support ... this class is
  // intended to be inherited by others, which will provide some more
  // of the CORBA/COM support.  Implementations of this "CORBA_ORB"
  // class must know how to create stringify/destringify their
  // objrefs, as well as how to marshal and unmarshal them ... as well
  // as provide their own QueryInterface.

  ULONG __stdcall AddRef (void);
  ULONG __stdcall Release (void);

  // = TAO-SPECIFIC THINGS
  TAO_Client_Factory &client_factory (void);
  TAO_Server_Factory &server_factory (void);
  TAO_ORB_Parameters &params (void);

protected:
  CORBA_ORB (void);
  virtual ~CORBA_ORB (void);

private:
  ACE_SYNCH_MUTEX lock_;
  u_int refcount_;

  TAO_Client_Factory client_factory_;
  TAO_Server_Factory server_factory_;
  TAO_ORB_Parameters params_;

  // = NON-PROVIDED METHODS
  CORBA_ORB (const CORBA_ORB &);
  CORBA_ORB &operator= (const CORBA_ORB &);
};

#  if defined(__ACE_INLINE__)
#    include "orbobj.i"
#  endif

#endif /* TAO_ORBOBJ_H */

