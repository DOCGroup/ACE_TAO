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

extern CORBA_TypeCode_ptr	TC_Cubit_Many;
extern CORBA_TypeCode_ptr	TC_Cubit_oneof;

struct Cubit_Many 
{
   CORBA_Octet o;
   CORBA_Long  l;
   CORBA_Short s;
};

enum Cubit_discrim
  { e_0th = 0, e_1st = 1, e_2nd = 2,
    e_3rd = 3, e_4th = 4, e_5th = 5 };

struct Cubit_oneof 
{
   Cubit_discrim  _disc;
   
   union 
   {    
      CORBA_Octet o;
      CORBA_Short s;
      CORBA_Long  l;
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
   static Cubit_ptr _narrow(CORBA_Object_ptr obj);
   static Cubit_ptr _nil();

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

protected:
   Cubit() { }
   Cubit(STUB_Object *objref)
      : CORBA_Object(objref) { }
   virtual ~Cubit() { }

   
};

extern const CORBA_Char *Cubit__id;		// type ID

#ifdef	_MSC_VER
#pragma	pack (pop)			// VC++, go back to other padding rules
#endif	// VC++

#endif	// _CUBIT_HH
