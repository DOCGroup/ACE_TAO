// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/Triangle_Test
//
// = FILENAME
//    Object_B_i.h
//
// = DESCRIPTION
//    This class implements the Object B of the
//    Nested Upcalls - Triangle test.
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#ifndef OBJECT_B_IMPL_H
#  define OBJECT_B_IMPL_H

#include "Triangle_TestS.h"

class Object_B_i : public POA_Object_B
{
  // = TITLE
  //     Implement the <Object_B> IDL interface.
public:
  Object_B_i (void);
  // Constructor.

  virtual ~Object_B_i (void);
  // Destructor.

  virtual void foo(Object_A_ptr theObject_A_ptr
                   TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#endif /* OBJECT_B_IMPL_H */
