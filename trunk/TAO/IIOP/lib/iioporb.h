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
//    Bridge implementation of IIOP_ORB
//
//    This is one of the two main hooks into IIOP in this Win32
//    C/C++/COM based implementation; the other being IIOP_Object.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
// 
// ============================================================================

#if !defined (TAO_IIOPORB_H)
#  define TAO_IIOPORB_H

#  include "ace/OS.h"
#  include "ace/Singleton.h"
#  include "orbobj.h"

typedef class IIOP_ORB *IIOP_ORB_ptr;

extern const IID IID_IIOP_ORB;

// ORB pseudo-objref

class ACE_Svc_Export IIOP_ORB : public CORBA_ORB
{
public:
  IIOP_ORB(void);

  ~IIOP_ORB (void) {}

  CORBA_Object_ptr string_to_object (CORBA_String str,
				     CORBA_Environment &env);

  CORBA_String object_to_string (CORBA_Object_ptr obj,
				 CORBA_Environment &env);

  HRESULT __stdcall QueryInterface (REFIID riid, 
				    void **ppv);

  // = ACCESSORS
  void use_omg_ior_format(CORBA_Boolean ior);
  // Set the IOR flag.
  CORBA_Boolean use_omg_ior_format(void);
  // Get the IOR flag.
  
private:
  CORBA_Boolean use_omg_ior_format_;

  // These are not provided.
  IIOP_ORB (const IIOP_ORB &);
  IIOP_ORB &operator = (const IIOP_ORB &);
};

// Create a type for the singleton.
typedef ACE_Singleton<IIOP_ORB, ACE_Thread_Mutex> TAO_ORB;

#  if defined(__ACE_INLINE__)
#    include "iioporb.i"
#  endif

#endif	/* TAO_IIOPORB_H */
