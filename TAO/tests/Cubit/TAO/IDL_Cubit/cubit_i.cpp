// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/IDL_Cubit
//
// = FILENAME
//    cubit_i.cpp
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee and Sergio Flores-Gaitan
//
// ============================================================================

#include "tao/corba.h"
#include "ace/Auto_Ptr.h"
#include "cubit_i.h"

// Constructor

Cubit_Factory_i::Cubit_Factory_i (void)
{
}

// Destructor

Cubit_Factory_i::~Cubit_Factory_i (void)
{
}

Cubit_ptr
Cubit_Factory_i::make_cubit (const char *,
			     CORBA::Environment &env)
{
  return my_cubit_._this (env);
}

// Constructor

Cubit_i::Cubit_i (const char *)
{
}

// Destructor

Cubit_i::~Cubit_i (void)
{
}

// Cube an octet

CORBA::Octet
Cubit_i::cube_octet (CORBA::Octet o,
                     CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return o * o * o;
}

// Cube a short.

CORBA::Short
Cubit_i::cube_short (CORBA::Short s,
                     CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return s * s * s;
}

// Cube a long

CORBA::Long
Cubit_i::cube_long (CORBA::Long l,
                    CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return l * l * l;
}

// Cube a struct

Cubit::Many
Cubit_i::cube_struct (const Cubit::Many &values,
                      CORBA::Environment &env)
{
  Cubit::Many temp;

  ACE_UNUSED_ARG (env);
  temp.o = values.o * values.o * values.o;
  temp.s = values.s * values.s * values.s;
  temp.l = values.l * values.l * values.l;

  return temp;
}

// Cube a union

Cubit::oneof
Cubit_i::cube_union (const Cubit::oneof &values,
                     CORBA::Environment &env)
{
  Cubit::oneof temp;

  ACE_UNUSED_ARG (env);
  switch (values._d ())
    {
    case Cubit::e_0th:
      temp.o (values.o () * values.o () * values.o ());
      break;
    case Cubit::e_1st:
      temp.s (values.s () * values.s () * values.s ());
      break;
    case Cubit::e_2nd:
      temp.l (values.l () * values.l () * values.l ());
      break;
    case Cubit::e_3rd:
    default:
      temp._d (values._d ()); // set the discriminant
      // use the read/write accessor
      temp.cm ().o  = values.cm ().o * values.cm ().o * values.cm ().o;
      temp.cm ().s  = values.cm ().s * values.cm ().s * values.cm ().s;
      temp.cm ().l  = values.cm ().l * values.cm ().l * values.cm ().l;

    }
  return temp;
}

// Cube a sequence
void
Cubit_i::cube_sequence(const Cubit::vector &input,
		       Cubit::vector_out output,
		       CORBA::Environment &)
{
  if (output.ptr () == 0)
    {
      output = new Cubit::vector (input.length ());
    }
  output->length (input.length ());
  for (CORBA::ULong i = 0; i < input.length (); ++i)
    {
      CORBA::Long x = input[i];
      output[i] = x * x * x;
    }
}


// Shutdown.

void Cubit_i::please_exit (CORBA::Environment &env)
{
  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (env);

  dmsg ("I've been asked to shut down...");
  TAO_ORB_Core_instance ()->orb ()->shutdown ();
  dexc (env, "please_exit, shutdown");
}
