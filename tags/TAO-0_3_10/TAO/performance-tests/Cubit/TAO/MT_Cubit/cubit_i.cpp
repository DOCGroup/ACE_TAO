// $Id$

#include "tao/corba.h"
#include "cubit_i.h"
#include "Task_Client.h"

ACE_RCSID(MT_Cubit, cubit_i, "$Id$")

Cubit_i::Cubit_i (CORBA::ORB_ptr orb)
  :orb_ (CORBA::ORB::_duplicate (orb))
{
}

Cubit_i::~Cubit_i (void)
{
}

CORBA::Octet
Cubit_i::cube_octet (CORBA::Octet o,
                     CORBA::Environment &)
{
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
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) Calling orb ()->shutdown ()\n"));
  this->orb_->shutdown ();
}
