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

CORBA_Octet
Cubit_i::Cubit_cube_octet (CORBA_Octet       o,
                           CORBA_Environment &env)
{
  ACE_UNUSED_ARG (env);
  return (CORBA_Octet) (o * o * o);
}

CORBA_Short
Cubit_i::Cubit_cube_short (CORBA_Short       s, 
                           CORBA_Environment &env)
{
  ACE_UNUSED_ARG (env);
  return (CORBA_Short) (s * s * s);
}

CORBA_Long
Cubit_i::Cubit_cube_long (CORBA_Long        l,
                          CORBA_Environment &env)
{
  ACE_UNUSED_ARG (env);
  return (CORBA_Long) (l * l * l);
}

Cubit_Many *
Cubit_i::Cubit_cube_struct (Cubit_Many        &values,
                            CORBA_Environment &env)
{
  ACE_UNUSED_ARG (env);
  values.o = values.o * values.o * values.o;
  values.s = values.s * values.s * values.s;
  values.l = values.l * values.l * values.l;
   
  return &values;
}

Cubit_oneof *
Cubit_i::Cubit_cube_union (Cubit_oneof       &values,
                           CORBA_Environment &env)
{
  ACE_UNUSED_ARG (env);
  switch (values._disc)
    {
    case e_0th:
      values.o = (CORBA_Octet) (values.o * values.o * values.o);
      break;
    case e_1st:
      values.s = (CORBA_Short) (values.s * values.s * values.s);
      break;
    case e_2nd:
      values.l = values.l * values.l * values.l;
      break;
    case e_3rd:
    default:
      values.cm.o = (CORBA_Octet) (values.cm.o * values.cm.o * values.cm.o);
      values.cm.s = (CORBA_Short) (values.cm.s * values.cm.s * values.cm.s);
      values.cm.l = values.cm.l * values.cm.l * values.cm.l;
    }
  return &values;
}

void Cubit_i::Cubit_please_exit (CORBA_Environment &env)
{
  TAO_OA_Parameters* params = TAO_OA_Parameters::instance ();
   
<<<<<<< cubit_i.cpp
   dmsg ("I've been asked to shut down...");
   params->oa()->please_shutdown(env);
   ACE_Service_Config::end_reactor_event_loop();
   dexc (env, "please_exit, please_shutdown");
=======
  dmsg ("I've been asked to shut down...");
  params->oa ()->please_shutdown (env);
  dexc (env, "please_exit, please_shutdown");
>>>>>>> 1.4
}
