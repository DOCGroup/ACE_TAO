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

#if 0
#  include "ace/OS.h"

#  include "tao/factories.h"
#  include "tao/params.h"
#endif /* 0 */

class ACE_Svc_Export CORBA_ORB : public IUnknown
{
  // = TITLE
  // ORB pseudo-objref
public:
  CORBA::BOA_ptr BOA_init (int &argc, 
			  char **argv, 
			  const char *boa_identifier = 0);
  static CORBA::ORB_ptr _duplicate (CORBA::ORB_ptr orb);
  static CORBA::ORB_ptr _nil (void);

  virtual CORBA::Object_ptr string_to_object (CORBA::String str,
                                             CORBA::Environment &env) = 0;
  virtual CORBA::String object_to_string (CORBA::Object_ptr obj,
                                         CORBA::Environment &env) = 0;

  // similar for TypeCodes and Anys ... to/from octet sequences

  void create_list (CORBA::Long count,
                    CORBA::NVList_ptr &retval);

  // Stuff required for COM IUnknown support ... this class is
  // intended to be inherited by others, which will provide some more
  // of the CORBA/COM support.  Implementations of this "CORBA::ORB"
  // class must know how to create stringify/destringify their
  // objrefs, as well as how to marshal and unmarshal them ... as well
  // as provide their own QueryInterface.

  ULONG __stdcall AddRef (void);
  ULONG __stdcall Release (void);

  // = TAO-specific methods.
  TAO_Client_Strategy_Factory *client_factory (void);
  TAO_Server_Strategy_Factory *server_factory (void);
  TAO_ORB_Parameters *params (void);

protected:
  CORBA_ORB (void);
  virtual ~CORBA_ORB (void);

private:
  ACE_SYNCH_MUTEX lock_;
  u_int refcount_;

  TAO_Client_Strategy_Factory *client_factory_;

  CORBA::Boolean client_factory_from_service_config_;

  TAO_Server_Strategy_Factory *server_factory_;

  CORBA::Boolean server_factory_from_service_config_;

  TAO_ORB_Parameters params_;

  // = NON-PROVIDED METHODS
  CORBA_ORB (const CORBA_ORB &);
  CORBA_ORB &operator= (const CORBA_ORB &);
};

#endif /* TAO_ORBOBJ_H */

