// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/IDL_Cubit
//
// = FILENAME
//    Cubit_Client.cpp
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee and Sergio Flores-Gaitan
//
// ============================================================================

#include "tao/corba.h"
#include "ace/Auto_Ptr.h"
#include "Cubit_Client.h"
#include "Cubit_i.h"

#include "tao/Timeprobe.h"
#include "RTI_IO.h"

ACE_RCSID(IDL_Cubit, Cubit_Client, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *Cubit_i_Timeprobe_Description[] =
{
  "Cubit_i::cube_oneway - start",
  "Cubit_i::cube_oneway - end",

  "Cubit_i::cube_void - start",
  "Cubit_i::cube_void - end",

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

  "Cubit_i::cube_long_sequence - start",
  "Cubit_i::cube_long_sequence - end",

  "Cubit_i::cube_octet_sequence - start",
  "Cubit_i::cube_octet_sequence - end",

  "Cubit_i::cube_many_sequence - start",
  "Cubit_i::cube_many_sequence - end"
};

enum
{
  // Timeprobe description table start key
  CUBIT_I_CUBE_ONEWAY_START = 10100,
  CUBIT_I_CUBE_ONEWAY_END,

  CUBIT_I_CUBE_VOID_START,
  CUBIT_I_CUBE_VOID_END,

  CUBIT_I_CUBE_OCTET_START,
  CUBIT_I_CUBE_OCTET_END,

  CUBIT_I_CUBE_SHORT_START,
  CUBIT_I_CUBE_SHORT_END,

  CUBIT_I_CUBE_LONG_START,
  CUBIT_I_CUBE_LONG_END,

  CUBIT_I_CUBE_STRUCT_START,
  CUBIT_I_CUBE_STRUCT_END,

  CUBIT_I_CUBE_UNION_START,
  CUBIT_I_CUBE_UNION_END,

  CUBIT_I_CUBE_LONG_SEQUENCE_START,
  CUBIT_I_CUBE_LONG_SEQUENCE_END,

  CUBIT_I_CUBE_OCTET_SEQUENCE_START,
  CUBIT_I_CUBE_OCTET_SEQUENCE_END,

  CUBIT_I_CUBE_MANY_SEQUENCE_START,
  CUBIT_I_CUBE_MANY_SEQUENCE_END
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (Cubit_i_Timeprobe_Description,
                                  CUBIT_I_CUBE_ONEWAY_START);

#endif /* ACE_ENABLE_TIMEPROBES */

// Constructor

Cubit_Factory_i::Cubit_Factory_i (CORBA::ORB_ptr orb)
  : my_cubit_ (orb),
    cubit_registered_ (0)
{
}

// Destructor

Cubit_Factory_i::~Cubit_Factory_i (void)
{
  if (this->cubit_registered_)
    {
      ACE_TRY_NEW_ENV
        {
          PortableServer::POA_var poa = this->my_cubit_._default_POA (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          PortableServer::ObjectId_var id = poa->servant_to_id (&this->my_cubit_,
                                                                ACE_TRY_ENV);
          ACE_TRY_CHECK;

          poa->deactivate_object (id.in (),
                                  ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught and ignored");
        }
      ACE_ENDTRY;
    }
}

Cubit_ptr
Cubit_Factory_i::make_cubit (CORBA::Environment &env)
{
  this->cubit_registered_ = 1;

  return my_cubit_._this (env);
}

// Constructor

Cubit_i::Cubit_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

// Destructor

Cubit_i::~Cubit_i (void)
{
}

void
Cubit_i::cube_oneway (CORBA::Environment &)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_ONEWAY_START);
}

void
Cubit_i::cube_void (CORBA::Environment &)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_VOID_START);
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
Cubit_i::cube_long_sequence (const Cubit::long_seq &input,
			     Cubit::long_seq_out output,
			     CORBA::Environment &)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_LONG_SEQUENCE_START);

  if (output.ptr () == 0)
    output = new Cubit::long_seq (input.length ());

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
Cubit_i::cube_octet_sequence (const Cubit::octet_seq &input,
			      Cubit::octet_seq_out output,
			      CORBA::Environment &)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_OCTET_SEQUENCE_START);

  if (output.ptr () == 0)
    {
#if defined (TAO_NO_COPY_OCTET_SEQUENCES)
      ACE_Message_Block mb (input.length ());
      mb.wr_ptr (input.length ());
      output = new Cubit::octet_seq (input.length (),  &mb);
#else
      output = new Cubit::octet_seq (input.length ());
#endif /* TAO_NO_COPY_OCTET_SEQUENCES */
    }

  output->length (input.length ());

#if 0
  for (CORBA::ULong i = 0; i < input.length (); ++i)
    {
      CORBA::Octet x = input[i];
      output[i] = x * x * x;
    }
#else
  // We don't want to cube all the elements on the sequence because
  // that will take too long and will affect the performance. Further,
  // we want to show that octet sequences have constant marshalling
  // time, but making a copy of each element will hide that.
  CORBA::ULong i = 0;
  CORBA::Octet x = input[0];
  output[i] = x * x * x;
#endif
}

void
Cubit_i::cube_many_sequence (const Cubit::many_seq & input,
                             Cubit::many_seq_out output,
                             CORBA::Environment &)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_MANY_SEQUENCE_START);

  if (output.ptr () == 0)
    output = new Cubit::many_seq (input.length ());

  output->length (input.length ());

#if 0
  for (CORBA::ULong i = 0; i < input.length (); ++i)
    {
      const Cubit::Many &in = input[i];
      Cubit::Many &out = output[i];

      out.o = in.o * in.o * in.o;
      out.s = in.s * in.s * in.s;
      out.l = in.l * in.l * in.l;
    }
#else
  CORBA::ULong i = 0;
  const Cubit::Many &in = input[i];
  Cubit::Many &out = output[i];

  out.o = in.o * in.o * in.o;
  out.s = in.s * in.s * in.s;
  out.l = in.l * in.l * in.l;
#endif
}

void
Cubit_i::cube_rti_data (const Cubit::RtiPacket &input,
			Cubit::RtiPacket_out output,
			CORBA::Environment &)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_LONG_SEQUENCE_START);

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Input: \n"));
      print_RtiPacket (input);
    }

  if (output.ptr () == 0)
    output = new Cubit::RtiPacket (input);

  output->packetHeader.packetColor
    = input.packetHeader.packetColor
    * input.packetHeader.packetColor
    * input.packetHeader.packetColor;

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Output: \n"));
      print_RtiPacket (*output.ptr ());
    }
}

// Shutdown.

void Cubit_i::shutdown (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "Cubit_i is shutting down"));

  this->orb_->shutdown ();
}
