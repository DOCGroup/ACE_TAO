// @(#)cubit_i.cpp 05/14/97
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// TEST:	hand-written Cubit Implementation
//
// Modified version of Cubit Example written by Sun Microsystems Inc.
// Modified by: Brian Mendel

/**************************************************************************
This file has been hand-crafted to work with the ORB API of the SUN's IIOP
implementation 
***************************************************************************/

#ifndef	_CUBIT_I_HH
#define	_CUBIT_I_HH

#include "cubitS.h"

class Cubit_i;
typedef Cubit_i *Cubit_i_ptr;
typedef Cubit_i_ptr Cubit_i_ref;

class Cubit_i : public _skel_Cubit
{
public:
   Cubit_i(const char* obj_name = 0);
   ~Cubit_i();
   
   virtual
   CORBA_Octet Cubit_cube_octet (CORBA_Octet       o,
                                 CORBA_Environment &env);

   virtual
   CORBA_Short Cubit_cube_short (CORBA_Short       s,
                                 CORBA_Environment &env);
  
   virtual
   CORBA_Long Cubit_cube_long (CORBA_Long        l,
                               CORBA_Environment &env);
  
   virtual
   Cubit_Many* Cubit_cube_struct (Cubit_Many        &values,
                                  CORBA_Environment &env);
  
   virtual
   Cubit_oneof* Cubit_cube_union (Cubit_oneof       &values,
                                  CORBA_Environment &env);
  
   virtual
   void Cubit_please_exit (CORBA_Environment &env);
};

#endif	// _CUBIT_I_HH
