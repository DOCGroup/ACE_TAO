// $Id$

#include "tao/corba.h"
#include "cubit_i.h"

Cubit_i::Cubit_i (Task_State *ts)
  :ts_ (ts)
{
}

Cubit_i::~Cubit_i (void)
{
}

CORBA::Octet
Cubit_i::cube_octet (CORBA::Octet o,
                     CORBA::Environment &)
{
  //ACE_ERROR (( LM_ERROR, "   {%t}\n"));
  if (ts_->utilization_task_started_ == 0)
    ts_->barrier_->wait ();
  return (CORBA::Octet) (o * o * o);
}

CORBA::Short
Cubit_i::cube_short (CORBA::Short s,
                     CORBA::Environment &)
{
  return (CORBA::Short) (s * s * s);
}

CORBA::Long
Cubit_i::cube_long (CORBA::Long l,
                    CORBA::Environment &)
{
  return (CORBA::Long) (l * l * l);
}

Cubit::Many 
Cubit_i::cube_struct (const Cubit::Many &values,
                      CORBA::Environment &)
{
  Cubit::Many out_values;
  out_values.o = values.o * values.o * values.o;
  out_values.s = values.s * values.s * values.s;
  out_values.l = values.l * values.l * values.l;

  return out_values;
}

void 
Cubit_i::noop (CORBA::Environment &)
{
  // does nothing.
}

void Cubit_i::shutdown (CORBA::Environment &)
{
  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}
