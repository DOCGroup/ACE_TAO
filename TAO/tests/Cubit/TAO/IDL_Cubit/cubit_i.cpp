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

#include "tao/Timeprobe.h"

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *Cubit_i_Timeprobe_Description[] =
{
  "Cubit_i::cube_octet - start",
  "Cubit_i::cube_octet - end",

  "Cubit_i::cube_short - start",
  "Cubit_i::cube_short - end",

  "Cubit_i::cube_long - start",
  "Cubit_i::cube_long - end",

  "Cubit_i::cube_struct - start",
  "Cubit_i::cube_struct - end",

  "Cubit_i::cube_union - start",
  "Cubit_i::cube_union - end",

  "Cubit_i::cube_sequence - start",
  "Cubit_i::cube_sequence - end"
};

enum
{
  CUBIT_I_CUBE_OCTET_START = 10000,
  CUBIT_I_CUBE_OCTET_END,

  CUBIT_I_CUBE_SHORT_START,
  CUBIT_I_CUBE_SHORT_END,

  CUBIT_I_CUBE_LONG_START,
  CUBIT_I_CUBE_LONG_END,

  CUBIT_I_CUBE_STRUCT_START,
  CUBIT_I_CUBE_STRUCT_END,

  CUBIT_I_CUBE_UNION_START,
  CUBIT_I_CUBE_UNION_END,

  CUBIT_I_CUBE_SEQUENCE_START,
  CUBIT_I_CUBE_SEQUENCE_END
};

#endif /* ACE_ENABLE_TIMEPROBES */

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (Cubit_i_Timeprobe_Description,
                                  CUBIT_I_CUBE_OCTET_START)

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
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_OCTET_START);

  ACE_UNUSED_ARG (env);
  return o * o * o;
}

// Cube a short.

CORBA::Short
Cubit_i::cube_short (CORBA::Short s,
                     CORBA::Environment &env)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_SHORT_START);

  ACE_UNUSED_ARG (env);
  return s * s * s;
}

// Cube a long

CORBA::Long
Cubit_i::cube_long (CORBA::Long l,
                    CORBA::Environment &env)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_LONG_START);

  ACE_UNUSED_ARG (env);
  return l * l * l;
}

// Cube a struct

Cubit::Many
Cubit_i::cube_struct (const Cubit::Many &values,
                      CORBA::Environment &env)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_STRUCT_START);

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
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_UNION_START);

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
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_SEQUENCE_START);

  if (output.ptr () == 0)
    output = new Cubit::vector (input.length ());

  output->length (input.length ());

#if 0
  for (CORBA::ULong i = 0; i < input.length (); ++i)
    {
      CORBA::Long x = input[i];
      output[i] = x * x * x;
    }
#else
  CORBA::ULong i = 0;
  CORBA::Long x = input[0];
  output[i] = x * x * x;
#endif
}

// Cube an octet sequence
void
Cubit_i::cube_raw (const Cubit::Raw &input,
                   Cubit::Raw_out output,
                   CORBA::Environment &)
{
  if (output.ptr () == 0)
    {
      ACE_Message_Block mb (input.length ());
      mb.wr_ptr (input.length ());
      TAO_Unbounded_Sequence<CORBA::Octet>* tmp =
        new TAO_Unbounded_Sequence<CORBA::Octet> (&mb);
      // @@ TODO this is a temporary hack until the IDL compiler
      // generates the constructor taking a Message_Block.
      output = (Cubit::Raw*)tmp;
    }

  output->length (input.length ());

#if 0
  for (CORBA::ULong i = 0; i < input.length (); ++i)
    {
      CORBA::Octet x = input[i];
      output[i] = x * x * x;
    }
#else
  CORBA::ULong i = 0;
  CORBA::Octet x = input[0];
  output[i] = x * x * x;
#endif
}

// Shutdown.

void Cubit_i::shutdown (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "Cubit_i is shutting down"));

  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}

// Constructor

Cubit_Shutdown_i::Cubit_Shutdown_i (const char *)
{
}

// Destructor

Cubit_Shutdown_i::~Cubit_Shutdown_i (void)
{
}

void
Cubit_Shutdown_i::shutdown (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "Cubit_i is shutting down"));

  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}

