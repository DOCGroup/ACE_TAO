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

typedef class IIOP_ORB *IIOP_ORB_ptr;

extern const IID IID_IIOP_ORB;

// ORB pseudo-objref

class ACE_Svc_Export IIOP_ORB : public CORBA_ORB
{
public:
  IIOP_ORB (CORBA_Boolean flag)
  { use_omg_ior_format = flag; }

  ~IIOP_ORB (void) {}

  CORBA_Object_ptr string_to_object (CORBA_String str,
				     CORBA_Environment &env);

  CORBA_String object_to_string (CORBA_Object_ptr obj,
				 CORBA_Environment &env);

  HRESULT __stdcall QueryInterface (REFIID riid, 
				    void **ppv);
private:
  CORBA_Boolean use_omg_ior_format;

  // These are not provided.
  IIOP_ORB (const IIOP_ORB &);
  IIOP_ORB &operator = (const IIOP_ORB &);
};

#endif	/* TAO_IIOPORB_H */
