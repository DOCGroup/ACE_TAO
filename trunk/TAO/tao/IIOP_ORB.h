// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    iiopobj.h
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
// 
// ============================================================================

#if !defined (TAO_IIOPORB_H)
#  define TAO_IIOPORB_H

#include "tao/corba.h"

// ORB pseudo-objref

class TAO_Export IIOP_ORB : public CORBA_ORB
{
  // = TITLE
  //    Bridge implementation of <IIOP_ORB>.
  //
  // = DESCRIPTION
  //    This is one of the two main hooks into IIOP in this C/C++
  //    based implementation; the other being <IIOP_Object>.
public:
  IIOP_ORB (void);
  // constructor

  ~IIOP_ORB (void);
  // destructor

  CORBA::Object_ptr string_to_object (CORBA::String str,
                                      CORBA::Environment &env);
  // Get an object reference from the IOR string.

  CORBA::String object_to_string (CORBA::Object_ptr obj,
                                  CORBA::Environment &env);
  // Convert an object reference to an IOR stringified form.

  // = ACCESSORS
  void use_omg_ior_format (CORBA::Boolean ior);
  // Set the IOR flag.
  CORBA::Boolean use_omg_ior_format (void);
  // Get the IOR flag.
  
private:
  CORBA::Boolean use_omg_ior_format_;
  // Decides whether to use the URL notation or to use IOR notation.

  // = These are not provided.
  IIOP_ORB (const IIOP_ORB &);
  IIOP_ORB &operator = (const IIOP_ORB &);
};

typedef IIOP_ORB *IIOP_ORB_ptr;

#endif	/* TAO_IIOPORB_H */
