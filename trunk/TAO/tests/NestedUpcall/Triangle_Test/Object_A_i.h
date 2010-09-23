// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/Triangle_Test
//
// = FILENAME
//    Object_A_i.h
//
// = DESCRIPTION
//    This class implements the Object A of the
//    Nested Upcalls - Triangle test.
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#ifndef OBJECT_B_IMPL_H
#  define OBJECT_B_IMPL_H

#include "Triangle_TestS.h"

class Object_A_i : public POA_Object_A
{
  // = TITLE
  //     Implement the <Object_A> IDL interface.
public:
  Object_A_i (void);
  // Constructor.

  virtual ~Object_A_i (void);
  // Destructor.

  virtual void foo (Initiator_ptr theInitiator_ptr);

  virtual void finish (void);

  virtual void shutdown (void);

  unsigned long finish_two_way_call_;
};

#endif /* OBJECT_B_IMPL_H */
