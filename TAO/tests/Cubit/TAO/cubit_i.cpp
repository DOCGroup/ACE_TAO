// @(#)cubit_i.cpp 05/14/97
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// TEST:	hand-written Cubit Implementation
//
// Modified version of Cubit Example written by Sun Microsystems Inc.
// Modified by: Brian Mendel

#include "tao/corba.h"    // ... and skeletons
#include "cubit_i.h"

Cubit_i::Cubit_i (const char *obj_name)
  : _skel_Cubit (obj_name)
{
}

Cubit_i::~Cubit_i (void)
{
}

CORBA::Octet
Cubit_i::Cubit_cube_octet (CORBA::Octet       o,
                           CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return (CORBA::Octet) (o * o * o);
}

CORBA::Short
Cubit_i::Cubit_cube_short (CORBA::Short       s, 
                           CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return (CORBA::Short) (s * s * s);
}

CORBA::Long
Cubit_i::Cubit_cube_long (CORBA::Long        l,
                          CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return (CORBA::Long) (l * l * l);
}

Cubit_Many *
Cubit_i::Cubit_cube_struct (Cubit_Many        &values,
                            CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  values.o = values.o * values.o * values.o;
  values.s = values.s * values.s * values.s;
  values.l = values.l * values.l * values.l;
   
  return &values;
}

Cubit_oneof *
Cubit_i::Cubit_cube_union (Cubit_oneof       &values,
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
}

void Cubit_i::Cubit_please_exit (CORBA::Environment &env)
{
  dmsg ("I've been asked to shut down...");
  TAO_ORB_CORE::instance()->root_poa ()->please_shutdown (env);
  // end_reactor_event_loop() no longer?
  dexc (env, "please_exit, please_shutdown");
}
