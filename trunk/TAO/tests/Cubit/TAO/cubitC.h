// @(#)cubitC.h 05/14/97
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Modified version of Cubit Example written by Sun Microsystems Inc.
// Modified by: Brian Mendel

/**************************************************************************
This file has been hand-crafted to work with the ORB API of the SUN's IIOP
implementation 
***************************************************************************/

#ifndef	_CUBIT_HH
#define	_CUBIT_HH

#ifdef	_MSC_VER
#pragma	pack (push, 1)			// VC++, known padding rules
#endif	// VC++

#include "tao/corba.h"

#include <iostream.h>

extern CORBA::TypeCode_ptr	TC_Cubit_Many;
extern CORBA::TypeCode_ptr	TC_Cubit_oneof;

struct Cubit_Many 
{
  CORBA::Octet o;
  CORBA::Long  l;
  CORBA::Short s;
};

enum Cubit_discrim
{ 
  e_0th = 0,
  e_1st = 1,
  e_2nd = 2,
  e_3rd = 3,
  e_4th = 4,
  e_5th = 5 
};

struct Cubit_oneof 
{
  Cubit_discrim  _disc;
   
  union 
  {    
    CORBA::Octet o;
    CORBA::Short s;
    CORBA::Long  l;
    Cubit_Many  cm;
  };
};

class Cubit;
typedef Cubit *Cubit_ptr;
typedef Cubit_ptr Cubit_ref;

class Cubit : public virtual CORBA_Object
{
public:
 
  static Cubit_ptr _duplicate(Cubit_ptr obj);
  static Cubit_ptr _narrow(CORBA::Object_ptr obj);
  static Cubit_ptr _nil();

  virtual CORBA::Octet Cubit_cube_octet (CORBA::Octet       o,
					CORBA::Environment &env);

  virtual CORBA::Short Cubit_cube_short (CORBA::Short       s,
					CORBA::Environment &env);
  
  virtual CORBA::Long Cubit_cube_long (CORBA::Long        l,
				      CORBA::Environment &env);
  
  virtual Cubit_Many* Cubit_cube_struct (Cubit_Many        &values,
					 CORBA::Environment &env);
  
  virtual Cubit_oneof* Cubit_cube_union (Cubit_oneof       &values,
					 CORBA::Environment &env);
  
  virtual void Cubit_please_exit (CORBA::Environment &env);

protected:
  Cubit (void) { }
  Cubit (STUB_Object *objref)
    : CORBA_Object (objref) { }
  virtual ~Cubit (void) { }
};

extern const CORBA::Char *Cubit__id;		// type ID

#ifdef	_MSC_VER
#pragma	pack (pop)			// VC++, go back to other padding rules
#endif	// VC++

#endif	// _CUBIT_HH


