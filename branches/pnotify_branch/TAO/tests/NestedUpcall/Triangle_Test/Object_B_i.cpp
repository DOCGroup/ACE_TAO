// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/Triangle_Test
//
// = FILENAME
//    Object_B_i.cpp
//
// = DESCRIPTION
//    This class implements the Object B  of the
//    Nested Upcalls - Triangle test.
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#include "tao/Exception.h"
#include "Object_B_i.h"
#include "ace/OS_NS_unistd.h"

ACE_RCSID(Triangle_Test, Object_B_i, "$Id$")

// CTOR
Object_B_i::Object_B_i (void)
{
}

// DTOR
Object_B_i::~Object_B_i (void)
{
}



void
Object_B_i::foo (Object_A_ptr theObject_A_ptr
                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) BEGIN Object_B_i::foo: Trying to call Object A\n"));

      theObject_A_ptr->finish (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Start to wait on this variable, it is set to true
      // by the method finish ()
      long int usecs = 500000;
      ACE_Time_Value pause(0, usecs);

      ACE_OS::sleep(pause);

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "calling the initiator");
    }
  ACE_ENDTRY;

}
