// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/MT_Client test
//
// = FILENAME
//    MT_Object_A_i.h
//
// = DESCRIPTION
//    This class implements the Object A of the
//    Nested Upcalls - MT Client test
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#ifndef MT_OBJECT_IMPL_H
#  define MT_OBJECT_IMPL_H

#include "MT_Client_TestS.h"
#include "MT_Client_TestC.h"

class MT_Object_i : public POA_MT_Object
{
  // = TITLE
  //     Implement the <MT_Object> IDL interface.
public:
  MT_Object_i (void);
  // Constructor.

  virtual ~MT_Object_i (void);
  // Destructor.

  virtual CORBA::Long yadda (CORBA::Long hop_count,
                             MT_Object_ptr partner
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

};

#endif /* MT_OBJECT_IMPL_H */
