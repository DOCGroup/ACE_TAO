// @(#)test1.hh	1.2 95/09/12
// Copyright 1995 by Sun Microsystems, Inc.
// All Rights Reserved
//
// TEST		interface for "test1"
//

#ifndef	_TEST1_HH
#define	_TEST1_HH

#include <corba/orb.hh>
#include <corba/stub.hh>


#ifdef	_MSC_VER
#pragma	pack (push, 1)			// VC++, known padding rules
#endif	// VC++

typedef CORBA_UShort		test1_ushort;
typedef CORBA_ULong		test1_ulong;

typedef CORBA_LongLong	test1_longlong;
typedef CORBA_ULongLong 	test1_ulonglong;
typedef CORBA_LongDouble	test1_longdouble;

typedef CORBA_Object		test1;
typedef test1			*test1_ptr;

#define	DECL_TEST(typename, truetype) \
    truetype \
    test1_test_ ## typename ( \
	test1_ptr		target, \
	truetype		in_a1, \
	truetype		&out_a2, \
	truetype		&inout_a3, \
	CORBA_Environment	&env \
    )

void test_illegal (test1_ptr target, CORBA_Environment &env);
void test1_test_void (test1_ptr target, CORBA_Environment &env);

DECL_TEST (short, CORBA_Short);
DECL_TEST (long, CORBA_Long);
DECL_TEST (ushort, CORBA_UShort);
DECL_TEST (ulong, CORBA_ULong);
DECL_TEST (float, CORBA_Float);
DECL_TEST (double, CORBA_Double);
DECL_TEST (boolean, CORBA_Boolean);
DECL_TEST (char, CORBA_Char);
DECL_TEST (octet, CORBA_Octet);

CORBA_Any *
test1_test_any (
    test1_ptr		target,
    const CORBA_Any	&in_a1,
    CORBA_Any		*&out_a2,
    CORBA_Any		&inout_a3,
    CORBA_Environment	&env
);

DECL_TEST (TypeCode, CORBA_TypeCode_ptr);
DECL_TEST (Principal, CORBA_Principal_ptr);
DECL_TEST (Object, CORBA_Object_ptr);

// NOTE:  CORBA C++ mapping says the "in" string is const
DECL_TEST (string, CORBA_String);

DECL_TEST (longlong, CORBA_LongLong);
DECL_TEST (ulonglong, CORBA_ULongLong);
DECL_TEST (wchar, CORBA_WChar);

// NOTE:  CORBA C++ mapping says the "in" string is const
DECL_TEST (wstring, CORBA_WString);

DECL_TEST (longdouble, CORBA_LongDouble);

#undef	DECL_TEST

extern CORBA_TypeCode_ptr	_tc_test1_x1;

class test1_x1 : public CORBA_UserException {
  public:
    CORBA_Long	case_num;
			
			test1_x1 (CORBA_Long n)
			: CORBA_UserException (_tc_test1_x1), case_num (n)
			{ }
};

extern CORBA_TypeCode_ptr	_tc_test1_x2;

class test1_x2 : public CORBA_UserException {
  public:
    CORBA_Object_ptr	obj;
    CORBA_Long	case_num;

			test1_x2 (CORBA_Object_ptr obj1,
				CORBA_Long n)
			: CORBA_UserException (_tc_test1_x2),
			    obj (obj1), case_num (n) { }

			~test1_x2 ()
			{ CORBA_release (obj); }
};

void
test1_test_throw (
    test1_ptr		target,
    CORBA_Long	case_num,
    CORBA_Environment	&env		// throw (x1, x2)
);

void
test1_please_exit (
    test1_ptr		target,
    CORBA_Environment	&env
);

#ifdef	_MSC_VER
#pragma	pack (pop)			// VC++, go back to other padding rules
#endif	// VC++

#endif	// _TEST1_HH

