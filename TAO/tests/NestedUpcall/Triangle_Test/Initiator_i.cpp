// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/Triangle_Test
//
// = FILENAME
//    Object_A_i.cpp
//
// = DESCRIPTION
//    This class implements the Object A  of the
//    Nested Upcalls - Triangle test.
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#include "tao/corba.h"
#include "Initiator_i.h"

ACE_RCSID(Triangle_Test, Initiator_i, "$Id$")

// CTOR
Initiator_i::Initiator_i (Object_A_ptr object_A_ptr,
                                Object_B_ptr object_B_ptr)
: object_A_var_ (Object_A::_duplicate (object_A_ptr)),
  object_B_var_ (Object_B::_duplicate (object_B_ptr))
{
}

// DTOR
Initiator_i::~Initiator_i (void)
{
}

void
Initiator_i::foo_object_B (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) BEGIN Initiator_i::foo_object_B ()\n"));

  ACE_TRY
    {
      this->object_B_var_->foo (this->object_A_var_.in ()
                                TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Initiator_i::foo_object_B: Returned from call.\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "calling the server");
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) END Initiator_i::foo_object_B ()\n"));
}
