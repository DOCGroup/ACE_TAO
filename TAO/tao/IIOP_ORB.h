// This may look like C, but it's really -*- C++ -*-
// $Id$

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

#ifndef TAO_IIOPORB_H
#define TAO_IIOPORB_H

#include "ace/INET_Addr.h"

#include "tao/ORB.h"

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

  CORBA::Object_ptr string_to_object (const char *str,
                                      CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Get an object reference from the IOR string.

  CORBA::String object_to_string (CORBA::Object_ptr obj,
                                  CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Convert an object reference to an IOR stringified form.

  TAO_ServantBase *_get_collocated_servant (STUB_Object *sobj);
  // Return the object pointer of an collocated object it there is
  // one, otherwise, return 0.

  // = ACCESSORS
  void _use_omg_ior_format (CORBA::Boolean ior);
  // Set the IOR flag.
  CORBA::Boolean _use_omg_ior_format (void);
  // Get the IOR flag.

  void _optimize_collocation_objects (CORBA::Boolean opt);
  // Set collocation optimization status.
  CORBA::Boolean _optimize_collocation_objects (void);
  // Get collocation optimization status.

private:
  CORBA::Boolean use_omg_ior_format_;
  // Decides whether to use the URL notation or to use IOR notation.

  CORBA::Boolean optimize_collocation_objects_;
  // Decides whether to use the URL notation or to use IOR notation.

  ACE_Unbounded_Set<ACE_INET_Addr> collocation_record_;

  // = These are not provided.
  IIOP_ORB (const IIOP_ORB &);
  IIOP_ORB &operator = (const IIOP_ORB &);
};

typedef IIOP_ORB *IIOP_ORB_ptr;

#if defined (__ACE_INLINE__)
# include "tao/IIOP_ORB.i"
#endif /* __ACE_INLINE__ */

#endif  /* TAO_IIOPORB_H */
