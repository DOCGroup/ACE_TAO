// @(#)cubit.hh	1.1 95/09/10
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Hand-crafted C language binding glue ...
//
// This doesn't use C++ since doing the obvious derivation
// (all parent interfaces are virtual public parents) makes
// object references have different "views".  That is, a
// pointer to a Cubit (i.e. a Cubit_ptr) would not have
// the same binary value as a pointer to a CORBA_Object
// (i.e. a CORBA_Object_ptr, generic objref).  That'd
// mean lots of narrowing/widening/RTTI infrastructure.
//

#ifndef	_CUBIT_HH
#define	_CUBIT_HH

#ifdef	_MSC_VER
#pragma	pack (push, 1)			// VC++, known padding rules
#endif	// VC++

#include "tao/corba.h"

//
// C style binding
//

typedef CORBA_Object	Cubit;
typedef Cubit *Cubit_ptr, *CubitRef;

extern CORBA_TypeCode_ptr	TC_Cubit_Many;
extern CORBA_TypeCode_ptr	TC_Cubit_oneof;

struct Cubit_Many {
    CORBA_Octet	o;
    CORBA_Long	l;
    CORBA_Short	s;
};

enum Cubit_discrim {e_0th = 0, e_1st = 1, e_2nd = 2,
                    e_3rd = 3, e_4th = 4, e_5th = 5};

struct Cubit_oneof {
    Cubit_discrim          _disc;

    union {    
        CORBA_Octet	o;
        CORBA_Short	s;
        CORBA_Long	l;
        Cubit_Many      cm;
    };
};

CORBA_Octet
Cubit_cube_octet (
    Cubit_ptr		target,
    CORBA_Octet	o,
    CORBA_Environment	&env
);

CORBA_Short
Cubit_cube_short (
    Cubit_ptr		target,
    CORBA_Short	s,
    CORBA_Environment	&env
);

CORBA_Long
Cubit_cube_long (
    Cubit_ptr		target,
    CORBA_Long		l,
    CORBA_Environment	&env
);

Cubit_Many *
Cubit_cube_struct (
    Cubit_ptr		target,
    Cubit_Many		&values,
    CORBA_Environment	&env
);

Cubit_oneof *
Cubit_cube_union (
    Cubit_ptr		target,
    Cubit_oneof         &values,
    CORBA_Environment	&env
);

void
Cubit_please_exit (
    Cubit_ptr		target,
    CORBA_Environment	&env
);

extern const CORBA_Char *Cubit__id;		// type ID

extern const TAO_Skel_Entry Cubit_operations [];

#ifdef	_MSC_VER
#pragma	pack (pop)			// VC++, go back to other padding rules
#endif	// VC++

#endif	// _CUBIT_HH
