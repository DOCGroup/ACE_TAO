// @(#)test1.hh	1.2 95/09/12
// Copyright 1995 by Sun Microsystems, Inc.
// All Rights Reserved
//
// TEST		interface for "test1"
//

#ifndef	_TEST1_HH
#define	_TEST1_HH

#include "tao/corba.h"

#ifdef	_MSC_VER
#pragma	pack (push, 1)			// VC++, known padding rules
#endif	// VC++

typedef CORBA::UShort		test1_ushort;
typedef CORBA::ULong		test1_ulong;

typedef CORBA::LongLong	test1_longlong;
typedef CORBA::ULongLong 	test1_ulonglong;
typedef CORBA::LongDouble	test1_longdouble;

typedef CORBA::Object		test1;
typedef test1			*test1_ptr;

#define	DECL_TEST(typename, truetype) \
    truetype \
    test1_test_ ## typename ( \
	test1_ptr		target, \
	truetype		in_a1, \
	truetype		&out_a2, \
	truetype		&inout_a3, \
	CORBA::Environment	&env \
    )

void test_illegal (test1_ptr target, CORBA::Environment &env);
void test1_test_void (test1_ptr target, CORBA::Environment &env);

DECL_TEST (short, CORBA::Short);
DECL_TEST (long, CORBA::Long);
DECL_TEST (ushort, CORBA::UShort);
DECL_TEST (ulong, CORBA::ULong);
DECL_TEST (float, CORBA::Float);
DECL_TEST (double, CORBA::Double);
DECL_TEST (boolean, CORBA::Boolean);
DECL_TEST (char, CORBA::Char);
DECL_TEST (octet, CORBA::Octet);

CORBA::Any *
test1_test_any (
    test1_ptr		target,
    const CORBA::Any	&in_a1,
    CORBA::Any		*&out_a2,
    CORBA::Any		&inout_a3,
    CORBA::Environment	&env
);

DECL_TEST (TypeCode, CORBA::TypeCode_ptr);
DECL_TEST (Principal, CORBA::Principal_ptr);
DECL_TEST (Object, CORBA::Object_ptr);

// NOTE:  CORBA C++ mapping says the "in" string is const
DECL_TEST (string, CORBA::String);

DECL_TEST (longlong, CORBA::LongLong);
DECL_TEST (ulonglong, CORBA::ULongLong);
DECL_TEST (wchar, CORBA::WChar);

// NOTE:  CORBA C++ mapping says the "in" string is const
DECL_TEST (wstring, CORBA::WString);

DECL_TEST (longdouble, CORBA::LongDouble);

#undef	DECL_TEST

extern CORBA::TypeCode_ptr	_tc_test1_x1;

class test1_x1 : public CORBA::UserException {
  public:
    CORBA::Long	case_num;
			
			test1_x1 (CORBA::Long n)
			: CORBA::UserException (_tc_test1_x1), case_num (n)
			{ }
};

extern CORBA::TypeCode_ptr	_tc_test1_x2;

class test1_x2 : public CORBA::UserException {
  public:
    CORBA::Object_ptr	obj;
    CORBA::Long	case_num;

			test1_x2 (CORBA::Object_ptr obj1,
				CORBA::Long n)
			: CORBA::UserException (_tc_test1_x2),
			    obj (obj1), case_num (n) { }

			~test1_x2 ()
			{ CORBA::release (obj); }
};

void
test1_test_throw (
    test1_ptr		target,
    CORBA::Long	case_num,
    CORBA::Environment	&env		// throw (x1, x2)
);

void
test1_please_exit (
    test1_ptr		target,
    CORBA::Environment	&env
);

#ifdef	_MSC_VER
#pragma	pack (pop)			// VC++, go back to other padding rules
#endif	// VC++

#endif	// _TEST1_HH

