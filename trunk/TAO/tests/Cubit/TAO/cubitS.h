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
#include "tao/corba.h"

class _skel_Cubit;
typedef _skel_Cubit *_skel_Cubit_ptr;
typedef _skel_Cubit_ptr _skel_Cubit_ref;

class _skel_Cubit : public Cubit
{
public:
  virtual
  CORBA::Octet Cubit_cube_octet (CORBA::Octet       o,
				CORBA::Environment &env) = 0;
  static 
  void _cube_octet_skel(CORBA::ServerRequest &req,
			CORBA::Object_ptr    obj,
			CORBA::Environment   &env);

  virtual
  CORBA::Short Cubit_cube_short (CORBA::Short       s,
				CORBA::Environment &env) = 0;
  
  static
  void _cube_short_skel (CORBA::ServerRequest &req,
			 CORBA::Object_ptr    obj,
			 CORBA::Environment   &env);

  virtual
  CORBA::Long Cubit_cube_long (CORBA::Long        l,
			      CORBA::Environment &env) = 0;
  
  static 
  void _cube_long_skel (CORBA::ServerRequest &req,
			CORBA::Object_ptr    obj,
			CORBA::Environment   &env);

  virtual
  Cubit_Many* Cubit_cube_struct (Cubit_Many        &values,
				 CORBA::Environment &env) = 0;
  
  static 
  void _cube_struct_skel (CORBA::ServerRequest &req,
			  CORBA::Object_ptr    obj,
			  CORBA::Environment   &env);

  virtual 
  Cubit_oneof* Cubit_cube_union (Cubit_oneof       &values,
				 CORBA::Environment &env) = 0;
  
  static 
  void _cube_union_skel (CORBA::ServerRequest &req,
			 CORBA::Object_ptr    obj,
			 CORBA::Environment   &env);

  virtual 
  void Cubit_please_exit (CORBA::Environment &env) = 0;

  static 
  void _please_exit_skel (CORBA::ServerRequest  &req,
			  CORBA::Object_ptr     obj,
			  CORBA::Environment    &env);
protected:
  _skel_Cubit (const char* obj_name = 0);
  virtual ~_skel_Cubit (void) { }
};

#endif	// _SKEL_CUBIT_HH
