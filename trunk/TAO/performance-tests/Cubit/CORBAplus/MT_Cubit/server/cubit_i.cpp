// $Id$

// @(#)cubit_i.cpp 05/14/97
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// TEST:	hand-written Cubit Implementation
//
// Modified version of Cubit Example written by Sun Microsystems Inc.
// Modified by: Brian Mendel

#include "pbroker/corba/orb.h"
#include "pbroker/corba/environ.h"
#include "cubit_i.h"

ACE_RCSID(server, cubit_i, "$Id$")

Cubit_i::Cubit_i (const char *obj_name)
{
}

Cubit_i::~Cubit_i (void)
{
}

CORBA::Octet
Cubit_i::cube_octet (CORBA::Octet       o)
{
  return (CORBA::Octet) (o * o * o);
}

CORBA::Short
Cubit_i::cube_short (CORBA::Short       s)
{
  return (CORBA::Short) (s * s * s);
}

CORBA::Long
Cubit_i::cube_long (CORBA::Long        l)
{
  return (CORBA::Long) (l * l * l);
}

Cubit::Many 
Cubit_i::cube_struct (const Cubit::Many        &values)
{
  Cubit::Many out_values;
  out_values.o  = values.o * values.o * values.o;
  out_values.s  = values.s * values.s * values.s;
  out_values.l  = values.l * values.l * values.l;

  return out_values;
}

/*Cubit_oneof *
Cubit_i::cube_union (Cubit_oneof       &values,
                           CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  switch (values._disc)
    {
    case e_0th:
      values.o = (CORBA::Octet) (values.o * values.o * values.o);
      break;
    case e_1st:
      values.s = (CORBA::Short) (values.s * values.s * values.s);
      break;
    case e_2nd:
      values.l = values.l * values.l * values.l;
      break;
    case e_3rd:
    default:
      values.cm.o = (CORBA::Octet) (values.cm.o * values.cm.o * values.cm.o);
      values.cm.s = (CORBA::Short) (values.cm.s * values.cm.s * values.cm.s);
      values.cm.l = values.cm.l * values.cm.l * values.cm.l;
    }
  return &values;
} */

void Cubit_i::please_exit (void)
{
}
