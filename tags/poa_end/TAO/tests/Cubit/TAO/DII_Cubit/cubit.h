// @(#)cubit.hh	1.1 95/09/10
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Hand-crafted C language binding glue ...
//
// This doesn't use C++ since doing the obvious derivation (all parent
// interfaces are virtual public parents) makes object references have
// different "views".  That is, a pointer to a Cubit (i.e. a
// Cubit_ptr) would not have the same binary value as a pointer to a
// CORBA::Object (i.e. a CORBA::Object_ptr, generic objref).  That'd
// mean lots of narrowing/widening/RTTI infrastructure.

#ifndef	_CUBIT_HH
#define	_CUBIT_HH

#ifdef	_MSC_VER
#pragma	pack (push, 1)			// VC++, known padding rules
#endif	// VC++

#include "tao/corba.h"

//
// C style binding
//

typedef CORBA::Object	Cubit;
typedef Cubit *Cubit_ptr, *CubitRef;

extern CORBA::TypeCode_ptr	TC_Cubit_Many;
extern CORBA::TypeCode_ptr	TC_Cubit_oneof;

struct Cubit_Many 
{
  CORBA::Octet	o;
  CORBA::Long	l;
  CORBA::Short	s;
};

enum Cubit_discrim {e_0th = 0, e_1st = 1, e_2nd = 2,
                    e_3rd = 3, e_4th = 4, e_5th = 5};

struct Cubit_oneof 
{
  Cubit_discrim _disc;

  union 
  {
    CORBA::Octet	o;
    CORBA::Short	s;
    CORBA::Long	l;
    Cubit_Many      cm;
  };
};

CORBA::Octet
Cubit_cube_octet (
    Cubit_ptr		target,
    CORBA::Octet	o,
    CORBA::Environment	&env
);

CORBA::Short
Cubit_cube_short (
    Cubit_ptr		target,
    CORBA::Short	s,
    CORBA::Environment	&env
);

CORBA::Long
Cubit_cube_long (
    Cubit_ptr		target,
    CORBA::Long		l,
    CORBA::Environment	&env
);

Cubit_Many *
Cubit_cube_struct (
    Cubit_ptr		target,
    Cubit_Many		&values,
    CORBA::Environment	&env
);

Cubit_oneof *
Cubit_cube_union (
    Cubit_ptr		target,
    Cubit_oneof         &values,
    CORBA::Environment	&env
);

void
Cubit_please_exit (
    Cubit_ptr		target,
    CORBA::Environment	&env
);

extern const CORBA::Char *Cubit__id;		// type ID

extern const TAO_Skel_Entry Cubit_operations [];

#ifdef	_MSC_VER
#pragma	pack (pop)			// VC++, go back to other padding rules
#endif	// VC++

#endif	// _CUBIT_HH
