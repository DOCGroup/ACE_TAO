
//=============================================================================
/**
 *  @file    Cubit_i.cpp
 *
 *  $Id$
 *
 *  @author Andy Gokhale
 *  @author Sumedh Mungee and Sergio Flores-Gaitan
 */
//=============================================================================


#include "Cubit_Client.h"
#include "Cubit_i.h"
#include "RTI_IO.h"

#include "tao/ORB_Constants.h"
#include "tao/debug.h"
#include "tao/Timeprobe.h"

#include "ace/Auto_Ptr.h"

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

  "Cubit_i::cube_any - start",
  "Cubit_i::cube_any - end",

  "Cubit_i::cube_any_struct - start",
  "Cubit_i::cube_any_struct - end",
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

  CUBIT_I_CUBE_ANY_START,
  CUBIT_I_CUBE_ANY_END,

  CUBIT_I_CUBE_ANY_STRUCT_START,
  CUBIT_I_CUBE_ANY_STRUCT_END,
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (Cubit_i_Timeprobe_Description,
                                  CUBIT_I_CUBE_ONEWAY_START);

#endif /* ACE_ENABLE_TIMEPROBES */

// Constructor

Cubit_Factory_i::Cubit_Factory_i (CORBA::ORB_ptr orb)
  : my_cubit_ (orb)
{
}

// Destructor

Cubit_Factory_i::~Cubit_Factory_i (void)
{
}

Cubit_ptr
Cubit_Factory_i::make_cubit (void)
{
  return my_cubit_._this ();
}

void
Cubit_Factory_i::set_default_poa (PortableServer::POA_ptr poa)
{
  this->my_cubit_.set_default_poa (poa);
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

PortableServer::POA_ptr
Cubit_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
Cubit_i::set_default_poa (PortableServer::POA_ptr poa)
{
  this->poa_ = PortableServer::POA::_duplicate (poa);
}

void
Cubit_i::cube_oneway (void)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_ONEWAY_START);
}

void
Cubit_i::cube_void (void)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_VOID_START);
}

// Cube an octet

CORBA::Octet
Cubit_i::cube_octet (CORBA::Octet o)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_OCTET_START);

  return o * o * o;
}

// Cube a short.

CORBA::Short
Cubit_i::cube_short (CORBA::Short s)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_SHORT_START);

  return s * s * s;
}

// Cube a long

CORBA::Long
Cubit_i::cube_long (CORBA::Long l)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_LONG_START);

  return l * l * l;
}

// Cube a struct

Cubit::Many
Cubit_i::cube_struct (const Cubit::Many &values)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_STRUCT_START);

  Cubit::Many temp;

  temp.o = values.o * values.o * values.o;
  temp.s = values.s * values.s * values.s;
  temp.l = values.l * values.l * values.l;

  return temp;
}

// Cube a union

Cubit::oneof
Cubit_i::cube_union (const Cubit::oneof &values)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_UNION_START);

  Cubit::oneof temp;

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
                             Cubit::long_seq_out output)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_LONG_SEQUENCE_START);

  if (output.ptr () == 0)
    output = new Cubit::long_seq (input.length ());

  output->length (input.length ());

#if defined (ACE_HAS_MEMORY_PROFILER)
  for (CORBA::ULong i = 0; i < input.length (); ++i)
    {
      CORBA::Long x = input[i];
      output[i] = x * x * x;
    }
#else
  CORBA::ULong i = 0;
  CORBA::Long x = input[0];
  output[i] = x * x * x;
#endif /* ACE_HAS_MEMORY_PROFILER */
}

// Cube an octet sequence
void
Cubit_i::cube_octet_sequence (const Cubit::octet_seq &input,
                              Cubit::octet_seq_out output)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_OCTET_SEQUENCE_START);

  if (output.ptr () == 0)
    {
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
      ACE_Message_Block mb (input.length ());
      mb.wr_ptr (input.length ());
      output = new Cubit::octet_seq (input.length (),  &mb);
#else
      output = new Cubit::octet_seq (input.length ());
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 0 */
    }

  output->length (input.length ());

#if defined (ACE_INITIALIZE_MEMORY_BEFORE_USE)
  // For Purify, initialize every output since we're looking for
  // accesses to uninitialized memory addresses.  Performance
  // is secondary when compiling for purify.
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
#endif /* ACE_INITIALIZE_MEMORY_BEFORE_USE */
}

void
Cubit_i::cube_many_sequence (const Cubit::many_seq & input,
                             Cubit::many_seq_out output)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_MANY_SEQUENCE_START);

  if (output.ptr () == 0)
    output = new Cubit::many_seq (input.length ());

  output->length (input.length ());

#if defined (ACE_HAS_PURIFY) && (ACE_HAS_PURIFY == 1)
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
#endif /* ACE_HAS_PURIFY == 1 */
}

void
Cubit_i::cube_rti_data (const Cubit::RtiPacket &input,
                        Cubit::RtiPacket_out output)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_LONG_SEQUENCE_START);

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Input:\n"));
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
                  "Output:\n"));
      print_RtiPacket (*output.ptr ());
    }
}

// Cube a long contained in an any

CORBA::Any *
Cubit_i::cube_any (const CORBA::Any & any)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_ANY_START);

  CORBA::Long l;
  any >>= l;
  l = l * l * l;
  CORBA::Any * ret_any = new CORBA::Any();
  *ret_any <<= l;
  return ret_any;
}

// Cube a struct contained in an any

CORBA::Any *
Cubit_i::cube_any_struct (const CORBA::Any & any)
{
  ACE_FUNCTION_TIMEPROBE (CUBIT_I_CUBE_ANY_STRUCT_START);
  Cubit::Many * arg_struct;
  Cubit::Many ret_struct;
  any >>= arg_struct;

  ret_struct.o = arg_struct->o * arg_struct->o * arg_struct->o;
  ret_struct.s = arg_struct->s * arg_struct->s * arg_struct->s;
  ret_struct.l = arg_struct->l * arg_struct->l * arg_struct->l;

  CORBA::Any * ret_any = new CORBA::Any();
  *ret_any <<= ret_struct;
  return ret_any;
}

// Shutdown.

void Cubit_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "%C\n",
              "Cubit_i is shutting down"));

  this->orb_->shutdown ();
}


void
Cubit_i::ping (void)
{
  // do nothing
}
