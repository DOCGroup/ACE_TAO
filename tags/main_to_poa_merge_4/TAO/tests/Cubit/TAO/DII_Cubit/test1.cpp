// @(#)test1.cpp	1.4 95/09/28
// Copyright 1995 by Sun Microsystems, Inc.
// All Rights Reserved
//
// TEST		stubs for "test1"
//

#include <stdio.h>

#include "test1.h"


//
// Define all the stubs ... it's a lot less error prone to do it with
// macros than by hand!
//
// NOTE:  the "calldata" is exported for use by the skeletons.  At some
// point skeletons will probably be fully abstracted; for now they aren't.
//
// Also, for some reason, name mangling is changed by the explicit
// declaration as "extern" -- if it's not done, linking fails.
//
#define	DEFINE_TEST3(typename, truetype, truetypename) \
    static const TAO_Param_Data test1_ ## typename ## _paramdata [4] = { \
    { CORBA::_tc_ ## truetypename, PARAM_RETURN, 0 }, \
    { CORBA::_tc_ ## truetypename, PARAM_IN, 0 }, \
    { CORBA::_tc_ ## truetypename, PARAM_OUT, 0 }, \
    { CORBA::_tc_ ## truetypename, PARAM_INOUT, 0 }, \
    }; \
    \
    extern const TAO_Call_Data test1_ ## typename ## _calldata; \
    \
    const TAO_Call_Data test1_ ## typename ## _calldata = { \
	"test_" #typename, CORBA::B_TRUE, \
	4, &test1_ ## typename ## _paramdata [0], \
	0, 0, \
    }; \
    \
    CORBA:: ## truetype \
    test1_test_ ## typename (test1_ptr target, \
		CORBA:: ## truetype in_a1, \
		CORBA:: ## truetype &out_a2, \
		CORBA:: ## truetype &inout_a3, \
		CORBA::Environment &env) { \
	CORBA:: ## truetype _retval; \
	STUB_Object	*_obj; \
	if (target->QueryInterface (IID_STUB_Object, (void **)&_obj) \
		!= NOERROR) \
	    env.exception (new CORBA::INV_OBJREF (COMPLETED_NO)); \
	else { \
	    _obj->do_call (env, &test1_ ## typename ## _calldata, \
		&_retval, &in_a1, &out_a2, &inout_a3); \
	    _obj->Release (); \
	} \
	return _retval; \
    }

//  changed extern const calldata test1_ ## typename ## _calldata;
//  to     const calldata test1_ ## typename ## _calldata; \

//	changed CORBA:: ## truetype _retval = 0; to changed CORBA:: ## truetype _retval;

#define	DEFINE_TEST(typename, truetype) \
	DEFINE_TEST3(typename, truetype, truetype)


//
// Generate a system exception, passing an operation ID that's
// not allowed by IIOP (much less this interface) and verifying
// that the server returns some kind of system exception.
//
static const TAO_Call_Data illegal_calldata = {
    "+_illegal", CORBA::B_TRUE,
    0, 0,
    0, 0
};

void
test_illegal (test1_ptr target, CORBA::Environment &env)
{
    STUB_Object		*data;

    if (target->QueryInterface (IID_STUB_Object, (void **)&data)
	    != NOERROR)
	env.exception (new CORBA::INV_OBJREF (COMPLETED_NO));
    else {
	data->do_call (env, &illegal_calldata
	    );
	data->Release ();
    }
}


extern const TAO_Call_Data test1_void_calldata;
const TAO_Call_Data test1_void_calldata = {
    "test_void", CORBA::B_TRUE,
    0, 0,
    0, 0
};

void
test1_test_void (test1_ptr target, CORBA::Environment &env)
{
    STUB_Object		*data;

    if (target->QueryInterface (IID_STUB_Object, (void **)&data)
	    != NOERROR)
	env.exception (new CORBA::INV_OBJREF (COMPLETED_NO));
    else {
	data->do_call (env, &test1_void_calldata
	    );
	data->Release ();
    }
}

DEFINE_TEST (short, Short);
DEFINE_TEST (long, Long);
DEFINE_TEST (ushort, UShort);
DEFINE_TEST (ulong, ULong);

#if	defined(MIPS)
//
// NOTE:  C/C++ compilers as a rule pass a "float" in the space that
// a "double" takes up.  Conversions are evidently optional; portability
// forces the following "explicit temporary" hack to work on at least
// one MIPS platform, which converts the parameter to "double" and
// hence changes the binary representation.  (Even if that is a compiler
// bug, it's probably required by now for binary compatibility!)
//
// A "-k ansi" compiler flag may be needed to get correct behaviour;
// passing the "in" parameters by reference apparently works too.  At
// this time, none of these solutions is used by default.
//
// This stub-level hackery seems like it could be replaced inside of the
// stub interpreter, which could just manually convert "float" parameters
// (all of them) as special cases.  But of course, that would slow the
// interpreter down on _every_ call, not just the ones that require it
// (such as this one).  Tradeoffs!
//
static const TAO_Param_Data test1_float_paramdata [4] = {
    { CORBA::_tc_float , PARAM_RETURN , 0 },
    { CORBA::_tc_float , PARAM_IN , 0 },
    { CORBA::_tc_float , PARAM_OUT , 0 },
    { CORBA::_tc_float , PARAM_INOUT , 0 }
};

extern const TAO_Call_Data test1_float_calldata;

const TAO_Call_Data test1_float_calldata = {
    "test_float", CORBA::B_TRUE,
    4 , &test1_float_paramdata [0],
    0 , 0
};

CORBA::Float
test1_test_float (
    test1_ptr		target,
    CORBA::Float	in_a1,
    CORBA::Float	&out_a2,
    CORBA::Float	&inout_a3,
    CORBA::Environment	&env
)
{
    CORBA::Float	_retval;

    // These three temporaries required due to MIPS compiler bug
    CORBA::Float	_in_a1 = in_a1;
    CORBA::Float	_out_a2 = out_a2;
    CORBA::Float	_inout_a3 = inout_a3;

    target -> data -> do_call (env,
	&test1_float_calldata,
	&_retval,
	&_in_a1,
	&_out_a2,
	&_inout_a3);
    return _retval;
}

#else
DEFINE_TEST (float, Float);
#endif	// MIPS


DEFINE_TEST (double, Double);
DEFINE_TEST (boolean, Boolean);
DEFINE_TEST (char, Char);
DEFINE_TEST (octet, Octet);

/*
CORBA::Any *
test1_test_any (test1_ptr target,
	const CORBA::Any &in_a1,
	CORBA::Any *&out_a2,
	CORBA::Any &inout_a3,
	CORBA::Environment &env)
{
    // XXX implement this stub! ... annoying that C++ mapping
    // calls for so much special casing
}
*/


DEFINE_TEST3 (TypeCode, TypeCode_ptr, TypeCode);
DEFINE_TEST3 (Principal, Principal_ptr, Principal);
DEFINE_TEST3 (Object, Object_ptr, Object);

// NOTE:  C++ mapping has "in" strings as "const", which doesn't
// show up in this macro ...
DEFINE_TEST3 (string, Char *, String);

DEFINE_TEST (longlong, LongLong);
DEFINE_TEST (ulonglong, ULongLong);
DEFINE_TEST (wchar, WChar);

// NOTE:  C++ mapping has "in" strings as "const", which doesn't
// show up in this macro ...
DEFINE_TEST3 (wstring, WChar *, WString);

#if !defined(NONNATIVE_LONGDOUBLE)
DEFINE_TEST (longdouble, LongDouble);
#endif

#undef	DEFINE_TEST


//
// Utility macros used to construct octet codes that are aligned
// on longword boundaries, and with a known byte order.  This
// happens to use big endian encoding since it was convenient.
// (Longword alignment is a happy accident of the specification
// of OMG-IDL ... it could have been much worse!)
//
// It'd be much simpler to lay out such data in assembler!
//

#if	defined (WORDS_BIGENDIAN)
#	define	MAKE_BIG_LONG(a,b,c,d) \
	    ((((a) & 0xff) << 24) | (((b) & 0xff) << 16) \
	     | (((c) & 0xff) <<  8) | ((d) & 0xff))
#	define	BIG_ENDIAN_LONG(x)	(x)

#else	// LITTLE_ENDIAN
#	define	MAKE_BIG_LONG(a,b,c,d) \
	    ((((d) & 0xff) << 24) | (((c) & 0xff) << 16) \
	     | (((b) & 0xff) <<  8) | ((a) & 0xff))
#	define BYTE_FROM(n,integer)    (((integer)>>(8*(n)))&0xff)
#	define BIG_ENDIAN_LONG(integer) \
	    MAKE_BIG_LONG (BYTE_FROM(3,integer), BYTE_FROM(2,integer),\
		BYTE_FROM(1,integer), BYTE_FROM(0,integer))
#endif



//
// "x1" exception typecode ... must be longword aligned
//
static CORBA::Long oc_x1 [] = {
    0,					// big endian flag + padding
    BIG_ENDIAN_LONG (29),		// length of ID string + NUL
    MAKE_BIG_LONG ('I', 'D', 'L', ':'),	// bytes of ID string
    MAKE_BIG_LONG ('E', 'n', 'g', '.'),
    MAKE_BIG_LONG ('S', 'U', 'N', '.'),
    MAKE_BIG_LONG ('C', 'O', 'M', '/'),
    MAKE_BIG_LONG ('t', 'e', 's', 't'),
    MAKE_BIG_LONG ('1', '/', 'x', '1'),
    MAKE_BIG_LONG (':', '1', '.', '0'),
    0,
    BIG_ENDIAN_LONG (1),		// (empty) namelen + NUL
    0,
    BIG_ENDIAN_LONG (1),		// only one struct member
    BIG_ENDIAN_LONG (1),		// (empty) member name + NUL
    0,
    BIG_ENDIAN_LONG (tk_long)
};
static CORBA::TypeCode tc_x1 (tk_except, sizeof oc_x1,
	    (unsigned char *)&oc_x1, CORBA::B_FALSE);
CORBA::TypeCode_ptr	_tc_test1_x1 = &tc_x1;



//
// "x2" exception typecode ... must be longword aligned
//
static CORBA::Long oc_x2 [] = {
    0,					// big endian flag + padding
    BIG_ENDIAN_LONG (29),		// length of ID string + NUL
    MAKE_BIG_LONG ('I', 'D', 'L', ':'),	// bytes of ID string
    MAKE_BIG_LONG ('E', 'n', 'g', '.'),
    MAKE_BIG_LONG ('S', 'U', 'N', '.'),
    MAKE_BIG_LONG ('C', 'O', 'M', '/'),
    MAKE_BIG_LONG ('t', 'e', 's', 't'),
    MAKE_BIG_LONG ('1', '/', 'x', '2'),
    MAKE_BIG_LONG (':', '1', '.', '0'),
    0,
    BIG_ENDIAN_LONG (1),		// (empty) namelen + NUL
    0,
    BIG_ENDIAN_LONG (2),		// two struct members

    BIG_ENDIAN_LONG (1),		// (empty) member name + NUL
    0,
    BIG_ENDIAN_LONG (tk_objref),
    BIG_ENDIAN_LONG (29),		// type ID + NUL
    MAKE_BIG_LONG ('I', 'D', 'L', ':'),	// bytes of ID string
    MAKE_BIG_LONG ('o', 'm', 'g', '.'),
    MAKE_BIG_LONG ('o', 'r', 'g', '/'),
    MAKE_BIG_LONG ('C', 'O', 'R', 'B'),
    MAKE_BIG_LONG ('A', '/', 'O', 'b'),
    MAKE_BIG_LONG ('j', 'e', 'c', 't'),
    MAKE_BIG_LONG (':', '1', '.', '0'),
    0,

    BIG_ENDIAN_LONG (1),		// (empty) member name + NUL
    0,
    BIG_ENDIAN_LONG (tk_long)
};
static CORBA::TypeCode tc_x2 (tk_except, sizeof oc_x2,
	    (unsigned char *)&oc_x2, CORBA::B_FALSE);
CORBA::TypeCode_ptr	_tc_test1_x2 = &tc_x2;


//
// parameter, exception, and call descriptions for "test_throw"
//
static const TAO_Param_Data test1_test_throw_paramdata [1] = {
    { CORBA::_tc_long, PARAM_IN, 0 }
};

static CORBA::TypeCode_ptr test1_test_throw_excepts [2] = {
    &tc_x1, &tc_x2
};

extern const TAO_Call_Data test1_test_throw_calldata;

const TAO_Call_Data test1_test_throw_calldata = {
    "test_throw", CORBA::B_TRUE,
    1, &test1_test_throw_paramdata [0],
    2, &test1_test_throw_excepts [0]
};

//
// "test_throw" stub
//
void
test1_test_throw (
    test1_ptr		target,
    CORBA::Long	case_num,
    CORBA::Environment	&env		// throw (x1, x2)
)
{
    STUB_Object		*data;

    if (target->QueryInterface (IID_STUB_Object, (void **)&data)
	    != NOERROR)
	env.exception (new CORBA::INV_OBJREF (COMPLETED_NO));
    else {
	data->do_call (env, &test1_test_throw_calldata,
	    &case_num);
	data->Release ();
    }
}

//
// PLEASE EXIT
//

static const TAO_Call_Data test1_please_exit_calldata = {
    "please_exit", CORBA::B_FALSE,
    0, 0,
    0, 0
};

void
test1_please_exit (
    test1_ptr		target,
    CORBA::Environment	&env
)
{
    STUB_Object		*data;

    if (target->QueryInterface (IID_STUB_Object, (void **)&data)
	    != NOERROR)
	env.exception (new CORBA::INV_OBJREF (COMPLETED_NO));
    else {
	data->do_call (env, &test1_please_exit_calldata
	    );
	data->Release ();
    }
}

