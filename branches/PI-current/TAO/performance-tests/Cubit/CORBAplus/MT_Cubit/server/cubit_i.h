// $Id$

// @(#)cubit_i.cpp 05/14/97
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// TEST:        hand-written Cubit Implementation
//
// Modified version of Cubit Example written by Sun Microsystems Inc.
// Modified by: Brian Mendel

/**************************************************************************
This file has been hand-crafted to work with the ORB API of the SUN's IIOP
implementation
***************************************************************************/

#ifndef _CUBIT_I_HH
#define _CUBIT_I_HH

#include "cubit_s.h"

class Cubit_i;
typedef Cubit_i *Cubit_i_ptr;
typedef Cubit_i_ptr Cubit_i_ref;

class Cubit_i : public Cubit_base_impl
{
public:
  Cubit_i (const char *obj_name = 0);
  ~Cubit_i (void);

  virtual
  CORBA::Octet cube_octet (CORBA::Octet       o);

  virtual
  CORBA::Short cube_short (CORBA::Short       s);

  virtual
  CORBA::Long cube_long (CORBA::Long        l);

  virtual
    Cubit::Many cube_struct (const Cubit::Many        &values);

  /*  virtual
  Cubit::oneof *cube_union (Cubit::oneof       &values,
  CORBA::Environment &env); */

  virtual
  void please_exit (void);
};

#endif  // _CUBIT_I_HH
