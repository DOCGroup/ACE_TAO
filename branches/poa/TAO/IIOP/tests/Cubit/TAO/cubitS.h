// @(#)cubitS.h 05/14/97
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Modified version of Cubit Example written by Sun Microsystems Inc.
// Modified by: Brian Mendel

/**************************************************************************
This file has been hand-crafted to work with the ORB API of the SUN's IIOP
implementation 
***************************************************************************/

#ifndef	_SKEL_CUBIT_HH
#define	_SKEL_CUBIT_HH

#include "cubitC.h"

class _skel_Cubit;
typedef _skel_Cubit *_skel_Cubit_ptr;
typedef _skel_Cubit_ptr _skel_Cubit_ref;

class _skel_Cubit : public Cubit
{
public:
   virtual
   CORBA_Octet Cubit_cube_octet (CORBA_Octet       o,
                                 CORBA_Environment &env) = 0;
   static 
   void _cube_octet_skel(CORBA_ServerRequest &req,
                         CORBA_Object_ptr    obj,
                         CORBA_Environment   &env);

   virtual
   CORBA_Short Cubit_cube_short (CORBA_Short       s,
                                 CORBA_Environment &env) = 0;
  
   static
   void _cube_short_skel (CORBA_ServerRequest &req,
                          CORBA_Object_ptr    obj,
                          CORBA_Environment   &env);

   virtual
   CORBA_Long Cubit_cube_long (CORBA_Long        l,
                               CORBA_Environment &env) = 0;
  
   static 
   void _cube_long_skel (CORBA_ServerRequest &req,
                         CORBA_Object_ptr    obj,
                         CORBA_Environment   &env);

   virtual
   Cubit_Many* Cubit_cube_struct (Cubit_Many        &values,
                                  CORBA_Environment &env) = 0;
  
   static 
   void _cube_struct_skel (CORBA_ServerRequest &req,
                           CORBA_Object_ptr    obj,
                           CORBA_Environment   &env);

   virtual 
   Cubit_oneof* Cubit_cube_union (Cubit_oneof       &values,
                                  CORBA_Environment &env) = 0;
  
   static 
   void _cube_union_skel (CORBA_ServerRequest &req,
                          CORBA_Object_ptr    obj,
                          CORBA_Environment   &env);

   virtual 
   void Cubit_please_exit (CORBA_Environment &env) = 0;

   static 
   void _please_exit_skel (CORBA_ServerRequest  &req,
                           CORBA_Object_ptr     obj,
                           CORBA_Environment    &env);
protected:
   _skel_Cubit(const char* obj_name = 0);
   virtual ~_skel_Cubit() { }

};

#endif	// _SKEL_CUBIT_HH
