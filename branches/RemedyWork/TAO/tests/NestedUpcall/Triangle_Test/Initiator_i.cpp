
//=============================================================================
/**
 *  @file    Initiator_i.cpp
 *
 *  $Id$
 *
 *  This class implements the Object A  of the
 *  Nested Upcalls - Triangle test.
 *
 *
 *  @author Michael Kircher
 */
//=============================================================================

#include "tao/Exception.h"
#include "Initiator_i.h"

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
Initiator_i::foo_object_B (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) BEGIN Initiator_i::foo_object_B ()\n"));

  try
    {
      this->object_B_var_->foo (this->object_A_var_.in ());
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Initiator_i::foo_object_B: Returned from call.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("calling the server");
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) END Initiator_i::foo_object_B ()\n"));
}
