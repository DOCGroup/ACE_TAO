// $Id$

#ifndef TAO_INTERPRETER_UTILS_CPP
#define TAO_INTERPRETER_UTILS_CPP

#include "orbsvcs/orbsvcs/Trader/Interpreter_Utils_T.h"
#include "orbsvcs/orbsvcs/Trader/Constraint_Visitors.h"

template <class OPERAND_TYPE> CORBA::Boolean
TAO_find (const CORBA::Any& sequence, const OPERAND_TYPE& element)
{
  CORBA::Boolean return_value = 0;
  TAO_Element_Equal<OPERAND_TYPE> functor;
  TAO_DynSequence_i dyn_seq (sequence);

  CORBA::ULong length = dyn_seq.length ();

  for (CORBA::ULong i = 0; i < length && ! return_value; i++)
    if (functor (dyn_seq, element))
      return_value = 1;

  return return_value;
}

#endif /* TAO_INTERPRETER_UTILS_CPP */
