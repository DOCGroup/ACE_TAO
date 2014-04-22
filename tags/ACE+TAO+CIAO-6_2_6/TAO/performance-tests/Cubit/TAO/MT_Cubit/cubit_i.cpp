// $Id$

#include "tao/ORB_Constants.h"
#include "cubit_i.h"
#include "Task_Client.h"

Cubit_i::Cubit_i (CORBA::ORB_ptr orb,
                  PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

Cubit_i::~Cubit_i (void)
{
}

CORBA::Octet
Cubit_i::cube_octet (CORBA::Octet o)
{
  return (CORBA::Octet) (o * o * o);
}

CORBA::Short
Cubit_i::cube_short (CORBA::Short s)
{
  return (CORBA::Short) (s * s * s);
}

CORBA::Long
Cubit_i::cube_long (CORBA::Long l)
{
  return (CORBA::Long) (l * l * l);
}

Cubit::Many
Cubit_i::cube_struct (const Cubit::Many &values)
{
  Cubit::Many out_values;

  out_values.o = values.o * values.o * values.o;
  out_values.s = values.s * values.s * values.s;
  out_values.l = values.l * values.l * values.l;

  return out_values;
}

void
Cubit_i::noop (void)
{
  // does nothing.
}

void Cubit_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Calling orb ()->shutdown ()\n"));
  this->orb_->shutdown ();
}

PortableServer::POA_ptr
Cubit_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
