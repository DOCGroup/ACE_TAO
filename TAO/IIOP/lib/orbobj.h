// This may look like C, but it's really -*- C++ -*-
//
// $Id$
//
// Header file for Win32 interface to CORBA's "ORB" type.
//
// The "ORB" pseudo-object is used in bootstrapping, such as to
// create object references from strings.  It's also used to
// create strings from object references.
//

#if !defined(TAO_ORBOBJ_H)
#  define TAO_ORBOBJ_H

#  include "ace/OS.h"

void				CORBA_release (CORBA_ORB_ptr orb);
CORBA_Boolean			CORBA_is_nil (CORBA_ORB_ptr orb);

extern const IID		IID_CORBA_ORB;

//
// ORB initialisation, per OMG document 94-9-46.
//
CORBA_ORB_ptr
CORBA_ORB_init (
    int			&argc,
    char		*const *argv,
    char		*orb_name,
    CORBA_Environment	&env
);

//
// ORB pseudo-objref
//
class ACE_Svc_Export CORBA_ORB : public IUnknown
{
public:
  CORBA_BOA_ptr BOA_init(int &argc, char **argv, const char *boa_identifier=0);
  static CORBA_ORB_ptr _duplicate (CORBA_ORB_ptr orb);
  static CORBA_ORB_ptr _nil ();

  virtual CORBA_Object_ptr string_to_object (CORBA_String str,
                                             CORBA_Environment &env) = 0;
  virtual CORBA_String object_to_string (CORBA_Object_ptr obj,
                                         CORBA_Environment &env) = 0;

  // similar for TypeCodes and Anys ... to/from octet sequences

  void create_list (CORBA_Long count,
                    CORBA_NVList_ptr &retval);

  //
  // Stuff required for COM IUnknown support ... this class is intended
  // to be inherited by others, which will provide some more of the
  // CORBA/COM support.  Implementations of this "CORBA_ORB" class must
  // know how to create stringify/destringify their objrefs, as well as
  // how to marshal and unmarshal them ... as well as provide their
  // own QueryInterface.
  //
  ULONG __stdcall AddRef ();
  ULONG __stdcall Release ();

  // = TAO-SPECIFIC THINGS
  TAO_Client_Factory& client_factory();
  TAO_Server_Factory& server_factory();
  TAO_ORB_Parameters& params();

protected:
  CORBA_ORB ();
  virtual ~CORBA_ORB ();

private:
  ACE_Thread_Mutex lock_;
  unsigned _refcount;

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

#endif

