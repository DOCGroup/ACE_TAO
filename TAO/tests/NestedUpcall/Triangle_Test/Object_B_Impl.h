// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/Triangle_Test
//
// = FILENAME
//    Object_B_Impl.h
//
// = DESCRIPTION
//    This class implements the Object B of the 
//    Nested Upcalls - Triangle test.
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#if !defined (OBJECT_B_IMPL_H)
#  define OBJECT_B_IMPL_H

#include "Triangle_TestS.h"

class Object_B_Impl : public POA_Object_B
{
  // = TITLE
  //     Implement the <Object_B> IDL interface.
public:
  Object_B_Impl (void);
  // Constructor.

  virtual ~Object_B_Impl (void);
  // Destructor.

  virtual void foo(Object_A_ptr object_A_ptr,
                   CORBA::Environment &env);
};

#endif /* OBJECT_B_IMPL_H */
