// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/Triangle_Test
//
// = FILENAME
//    Initiator_i.h
//
// = DESCRIPTION
//    This class implements the Initiator of the
//    Nested Upcalls - Triangle test.
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#ifndef INITIATOR_IMPL_H
#  define INITIATOR_IMPL_H

#include "Triangle_TestS.h"

class Initiator_i : public POA_Initiator
{
  // = TITLE
  //     Implement the <Initiator> IDL interface.
public:
  Initiator_i (Object_A_ptr object_A_ptr,
                  Object_B_ptr object_B_ptr);
  // Constructor.

  virtual ~Initiator_i (void);
  // Destructor.

  virtual void foo_object_B (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  Object_A_var object_A_var_;
  // reference to object A

  Object_B_var object_B_var_;
  // reference to object B
};

#endif /* INITIATOR_IMPL_H */
