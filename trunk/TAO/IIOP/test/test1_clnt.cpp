// @(#)test1_clnt.cpp	1.5 95/09/24
// Copyright 1995 by Sun Microsystems, Inc.
// All Rights Reserved
//
// TEST		client driver for "test1"
//

#ifdef	USE_IOSTREAM
#include <iostream.h>
#endif

#include <stdio.h>
#include <string.h>

#if	unix
#	include	<unistd.h>

#else	// windows
#	include	"getopt.h"		// e.g. GNU's version

#endif	// unix

#include "test1.hh"
#include "../lib/runtime/debug.hh"


extern char 	*optarg;	// missing on some platforms

extern void
print_exception (const CORBA_Exception *, const char *, FILE *f=stdout);


//
// All tests are specified so that the return value and "out" (second)
// parameters are easily tested functions of the "in" (first) parameter,
// and the "inout" (third) parameter is the same function the its
// initial value.   Caller is expected to specify two different "in"
// values.   This helps make the marshaled bits vary, and turn up a class
// of potential problems that'd be hidden if parameter order had no
// effect on the test.
//
// PERFORM_TEST calls a test for a given type, reporting in cases where
// the ORB or operation failed.  The COMPARE symbol can be redefined
// to achieve different functions -- e.g. cubing numbers, identity,
// negation, etc.  It should return true iff the correct result was
// returned.
//
// BAD_COMPARE_VALUES is to produce diagnostics when a test fails,
// showing the actual and expected values of returned parameters.
// This helps diagnose specific porting problems.
//
// The RELEASE symbol may be defined to free memory, eliminating client
// side memory leaks in the test.
//
#define	RELEASE(X)		// NOP by default
#define	PERFORM_TEST(name,type,value1,value2) \
    { \
	CORBA_ ## type	v1, v2, v3; \
 	\
	v1 = (CORBA_ ## type)(value1); \
	v2 = 0; \
	v3 = (CORBA_ ## type)(value2); \
 	\
	test_count++; \
	v1 = test1_test_ ## name (target, v1, v2, v3, env); \
	if (env.exception () != 0) { \
	    print_exception (env.exception (), "perform test_" #name); \
	    error_count++; \
	} else if (!COMPARE (CORBA_ ## type, v1, value1) \
		|| !COMPARE (CORBA_ ## type, v2, value1) \
		|| !COMPARE (CORBA_ ## type, v3, value2) ) { \
	    fprintf (stderr, "bad comparison, test_" #name "\n"); \
	    BAD_COMPARE_VALUES(type,v1,v2,v3,value1,value2) \
	    error_count++; \
	} \
	RELEASE (v1); RELEASE (v2); RELEASE (v3); \
    }

//
// This messing about is because the ostream op << cannot always
// be used with all kinds of data ... e.g. LongDouble, TypeCode.
//
#ifdef	USE_IOSTREAM
#	define	DO_IO(x) x
#else
#	define	DO_IO(x)
#endif
#define BAD_COMPARE_VALUE_OUT(type,v1,v2,v3,value1,value2) \
	    DO_IO( cerr << " v1=" << v1; \
            cerr << " expecting " << EXPVAL(CORBA_ ## type, value1) << "\n"; \
	    cerr << " v2=" << v2; \
            cerr << " expecting " << EXPVAL(CORBA_ ## type, value1) << "\n"; \
	    cerr << " v3=" << v3; \
	    cerr << " expecting " << EXPVAL(CORBA_ ## type, value2) << "\n");
#define BAD_COMPARE_VALUES(type,v1,v2,v3,value1,value2) \
    BAD_COMPARE_VALUE_OUT(type,v1,v2,v3,value1,value2)


//
// test utility -- should be able to just use is_equivalent() directly
// but this implementation uses null pointers for nil, so this must
// check for nulls first.  (May be noncompliant with C++ mapping!)
//
static CORBA_Boolean
compare_objrefs (
    CORBA_Object_ptr	v1,
    CORBA_Object_ptr	v2
)
{
    CORBA_Boolean	temp;
    CORBA_Environment	env;

    if (v1 == v2)
	return CORBA_B_TRUE;

    if (CORBA_is_nil (v1))
	return CORBA_is_nil (v2);

    temp = v1->_is_equivalent (v2, env);
    if (env.exception () != 0) {
	print_exception (env.exception (), "compare objref");
	return CORBA_B_FALSE;
    }
    return temp;
}


//
// Helper routine to help sure math (especially for floating point)
// gets done correctly.
//
template <class Type>
Type cube (Type arg)
{
    Type temp = arg;

    temp = temp * arg;
    temp = temp * arg;
    return temp;
}


static int	skip_longdouble = 0;

//
// This just performs the tests ...
//
void
do_tests (
    test1_ptr	target,
    unsigned	loop_count,
    unsigned	&test_count,
    unsigned	&error_count
)
{
    int		count;

    for (count = 0; count < loop_count; count++) {
	CORBA_Environment	env;
	CORBA_Environment	env2;	// XXX

	//
	// test_void
	//
	test_count++;
	test1_test_void (target, env);
	if (env.exception () != 0) { 
	    print_exception (env.exception (), "perform test_void");
	    error_count++;
	}

	//
	// Numeric tests ... try some variety in computation, no real
	// rationale in the choice of initial parameter values except
	// not to use the same values all the time.
	//
#define EXPVAL(type,original_value) ((type) cube((type)(original_value)))
#define COMPARE(type,retval,original_value) \
	((retval) == EXPVAL(type, original_value))

	PERFORM_TEST (octet, Octet, count + 29, count - 22);

	PERFORM_TEST (short, Short, count - 23, count + 19);
	PERFORM_TEST (ushort, UShort, count + 23, count - 19);

    	PERFORM_TEST (long, Long, count - 17, count + 20);
    	PERFORM_TEST (ulong, ULong, count + 17, count - 20);

#if	!defined (NONNATIVE_LONGLONG)
	// don't try this on platforms that don't support
	// math on longlongs ...
	PERFORM_TEST (longlong, LongLong, count - 177, count + 3);
	PERFORM_TEST (ulonglong, ULongLong, count + 177, count - 3);
#endif	// !NONNATIVE_LONGLONG

#if	!defined (i386)
	//
	// XXX not sure what's wrong with the COMPARE macro with respect to
	// floating point on x86, since fprintf shows the values basically
	// look correct ... but these float/double tests fail.
	//
	PERFORM_TEST (float, Float, count - 0.29, count + 3.14159);
	PERFORM_TEST (double, Double, count * 1.77, count * 2.71);
#endif	// !defined (i386)


#ifndef	NONNATIVE_LONGDOUBLE
#undef	BAD_COMPARE_VALUES
#define BAD_COMPARE_VALUES(type,v1,v2,v3,value1,value2) // NOP

	//
	// don't try this between two platforms that don't provide
	// arithmetic support for LongDouble values ...
	//
	if (!skip_longdouble) {
	    PERFORM_TEST (longdouble, LongDouble,
		    count - 2.33, count * 3.14159);
	}

#  undef BAD_COMPARE_VALUES
#  define BAD_COMPARE_VALUES(type,v1,v2,v3,value1,value2) \
    BAD_COMPARE_VALUE_OUT(type,v1,v2,v3,value1,value2)
#endif	// !NONNATIVE_LONGDOUBLE

#undef	COMPARE
#undef	EXPVAL


	//
	// Boolean -- negation
	//
#define EXPVAL(type,original_value) (!(type)(original_value))
#define COMPARE(type,retval,original_value) \
		(((type)(retval)) == EXPVAL(type,original_value))

	PERFORM_TEST (boolean, Boolean,
		(count & 0x01) != 0, (count & 0x01) == 0);
#undef	COMPARE
#undef	EXPVAL

	//
	// Char, WChar -- identity
	//
#define EXPVAL(type,original_value) ((type)(original_value))
#define COMPARE(type,retval,original_value) \
		(((type)(retval)) == EXPVAL(type,original_value))

	PERFORM_TEST (char, Char, count + 26, count - 5);
	PERFORM_TEST (wchar, WChar, count, count + 25);
#undef	COMPARE
#undef	EXPVAL

	//
	// Object_ptr -- identity (special comparision)
	//
#define EXPVAL(type,original_value) ((type)(original_value))
#define COMPARE(type,retval,original_value) \
	    (compare_objrefs (retval, original_value) == CORBA_B_TRUE)
#undef	RELEASE
#define	RELEASE(obj) \
	    { CORBA_release (obj); }

	PERFORM_TEST (Object, Object_ptr, target, CORBA_Object::_nil ());
#undef	COMPARE
#undef	EXPVAL

	//
	// TypeCode_ptr -- identity (special comparision), and verifies
	// that most of the built-in typecode constants exist.
	//
	// XXX should try this on all standard and system exceptions!
	//
	// XXX  this currently tests only marshaling/unmarshaling of
	// "no parameter" TypeCodes ... complex ones (objref, struct,
	// union, enum, sequence, array, alias, exception) are _NOT_
	// currently attempted.
	//
	// XXX  for the simple typecodes (string, wstring), the single
	// "bound" parameter isn't exercised/verified as it should be.
	//
#define EXPVAL(type,original_value) ((type)(original_value))
#define	COMPARE(type,retval,original) \
	    (((retval) == (original)) || \
		((retval)->_kind == (original)->_kind))

#undef	RELEASE
#define	RELEASE(tc) \
	    { CORBA_release (tc); }

#undef	BAD_COMPARE_VALUES
#define BAD_COMPARE_VALUES(type,v1,v2,v3,value1,value2) // NOP

	{
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_Null, _tc_CORBA_Void);

	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_Short, _tc_CORBA_UShort);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_Long, _tc_CORBA_ULong);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_LongLong, _tc_CORBA_ULongLong);

	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_Float, _tc_CORBA_Double);

	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_Boolean, _tc_CORBA_Octet);

	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_Any, _tc_CORBA_TypeCode);

	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_LongDouble, _tc_CORBA_Principal);

	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_Char, _tc_CORBA_String);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_WChar, _tc_CORBA_WString);

	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_LongDouble, _tc_CORBA_Octet);

	    //
	    // Try all of the standard exception typecodes.
	    //
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_UNKNOWN, _tc_CORBA_BAD_PARAM);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_NO_MEMORY, _tc_CORBA_IMP_LIMIT);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_COMM_FAILURE, _tc_CORBA_INV_OBJREF);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_OBJECT_NOT_EXIST, _tc_CORBA_NO_PERMISSION);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_INTERNAL, _tc_CORBA_MARSHAL);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_INITIALIZE, _tc_CORBA_NO_IMPLEMENT);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_BAD_TYPECODE, _tc_CORBA_BAD_OPERATION);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_NO_RESOURCES, _tc_CORBA_NO_RESPONSE);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_PERSIST_STORE, _tc_CORBA_BAD_INV_ORDER);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_TRANSIENT, _tc_CORBA_FREE_MEM);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_INV_IDENT, _tc_CORBA_INV_FLAG);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_INTF_REPOS, _tc_CORBA_BAD_CONTEXT);
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_OBJ_ADAPTER, _tc_CORBA_DATA_CONVERSION);

	    //
	    // All the built-in "user defined" system exceptions.
	    //
	    PERFORM_TEST (TypeCode, TypeCode_ptr,
		    _tc_CORBA_BadKind, _tc_CORBA_Bounds);
	}
#undef	COMPARE
#undef	RELEASE

	//
	// string (unbounded)-- identity (special comparision)
	//
#define COMPARE(type,retval,original_value) \
	    (strcmp ((char *)(retval), (char*)(original_value)) == 0)
#undef	RELEASE
#define	RELEASE(obj) CORBA_string_free(obj)
	static const CORBA_Char str1 [] = "small";
	static const CORBA_Char str2 [] =
			"relatively long string, constructed"
			" with the aid of C++ implicit string"
			" catenation, which simplifies much stuff";
	PERFORM_TEST (string, String, str1, str2);
#undef	COMPARE
#undef	RELEASE


	// XXX any, principal, wstring ... output _is_ input


	//
	// Three test cases involve throwing user-defined exceptions.
	//
	{
	    CORBA_Exception	*xp;

	    //
	    // Case one:  with parameter <= zero, must throw
	    // an "x1" exception whose "case_num" is that parameter
	    //
	    test_count++;

	    test1_test_throw (target, -5, env);
	    if (env.exception () != 0) {
		xp = env.exception ();

		if (strcmp ((char *)xp->id (),
			(char *) _tc_test1_x1->id (env2)) != 0) {
		    error_count++;
		    fprintf (stderr, "test_throw case 1, "
			    "wrong exception thrown (id = '%s')\n",
			    xp->id ());
		} else {
		    test1_x1	*xp2 = (test1_x1 *) xp;

		    if (xp2->case_num != -5) {
			error_count++;
			fprintf (stderr, "test_throw case 1, "
				"wrong value (case_num = '%ld')\n",
				xp2->case_num);
		    }
		    
		    // else, right exception was thrown
		}
	    } else {
		error_count++;
		fprintf (stderr, "test_throw case 1, "
			"no exception thrown !!\n");
	    }


	    //
	    // Case two:  with positive odd parameter, must throw
	    // an "x2" exception whose obj is null, and whose case_num
	    // is that parameter
	    //
	    test_count++;

	    test1_test_throw (target, 101, env);
	    if (env.exception () != 0) {
		xp = env.exception ();

		if (strcmp ((char *)xp->id (),
			(char *) _tc_test1_x2->id (env2)) != 0) {
		    error_count++;
		    fprintf (stderr, "test_throw case 2, "
			    "wrong exception thrown (id = '%s')\n",
			    xp->id ());
		} else {
		    test1_x2	*xp2 = (test1_x2 *) xp;

		    if (xp2->case_num != 101) {
			error_count++;
			fprintf (stderr, "test_throw case 2, "
				"wrong value (case_num = '%ld')\n",
				xp2->case_num);
		    }

		    if (!CORBA_is_nil (xp2->obj)) {
			error_count++;
			fprintf (stderr, "test_throw case 2, "
				"non-null objref thrown\n");
		    }
		    
		    // else, right exception was thrown
		}
	    } else {
		error_count++;
		fprintf (stderr, "test_throw case 2, "
			"no exception thrown !!\n");
	    }


#if	0
// 18-sept-95:  commented this out.  Work remains to be done in
// this area:  (a) ServerRequest::target operation unimplemented,
// so for the servers of current interest this test is no help;
// (b) probable pointer leak in exceptions holding data.

	    //
	    // Case three:  with positive even parameter, must throw
	    // an "x2" exception whose obj is the target, and whose
	    // case_num is that parameter
	    //
	    test_count++;

	    test1_test_throw (target, 42, env);
	    if (env.exception () != 0) {
		xp = env.exception ();

		if (strcmp ((char *)xp->id (),
			(char *) _tc_test1_x2->id (env2)) != 0) {
		    error_count++;
		    fprintf (stderr, "test_throw case 3, "
			    "wrong exception thrown (id = '%s')\n",
			    xp->id ());
		} else {
		    test1_x2	*xp2 = (test1_x2 *) xp;

		    if (xp2->case_num != 42) {
			error_count++;
			fprintf (stderr, "test_throw case 3, "
				"wrong value (case_num = '%ld')\n",
				xp2->case_num);
		    }

		    CORBA_Boolean	status;

		    status = target->_is_equivalent (xp2->obj, env);

		    if (env.exception () != 0) {
			error_count++;
			print_exception (env.exception (),
				"test_throw/3 call to is_equivalent");
		    } else if (status != CORBA_B_TRUE) {
			error_count++;
			fprintf (stderr, "test_throw case 3, "
				"non-equivalent objref thrown\n");
		    }
		    
		    // else, right exception was thrown
		}
	    } else {
		error_count++;
		fprintf (stderr, "test_throw case 3, "
			"no exception thrown !!\n");
	    }

	    env.clear ();
#endif	// 0

	}

	//
	// test_illegal -- generate a BAD_OPERATION system exception
	// from the remote process
	//
	test_count++;
	test_illegal (target, env);
	if (env.exception () == 0
		|| strcmp ((char *) env.exception()->id(),
			(char *) _tc_CORBA_BAD_OPERATION->id (env2)) != 0) { 
	    fprintf (stderr, "couldn't generate BAD_OPERATION exception\n");
	    error_count++;
	}
    }
}


int
main (
    int		argc,
    char	*const *argv
)
{
    CORBA_ORB_ptr	orb_ptr;
    CORBA_Environment	env;
    CORBA_Object_ptr	objref = CORBA_Object::_nil();
    unsigned		loop_count = 1;
    unsigned		tests = 0, errors = 0;
    int			exit_later = 0;

    orb_ptr = CORBA_ORB_init (argc, argv, "internet", env);
    if (env.exception () != 0) {
	dexc (env, "ORB initialisation");
	return 1;
    }

    //
    // Parse and verify parameters.
    //
    int			c;

    while ((c = getopt (argc, argv, "dln:O:x")) != EOF)
	switch (c) {
	  case 'd':			// debug flag
	    debug_level++;
	    continue;

	  case 'l':			// skip "long double" test
	    skip_longdouble++;
	    continue;

	  case 'n':			// loop count
	    loop_count = (unsigned) atoi (optarg);
	    continue;

	  case 'O':			// stringified objref
	    {
		objref = orb_ptr->string_to_object (
			(CORBA_String)optarg, env);
		if (env.exception () != 0) {
		    dexc (env, "string2object");
		    return 1;
		}
	    }
	    continue;

	  case 'x':
	    exit_later++;
	    continue;

	  case '?':
	  default:
	    fprintf (stderr, "usage:  %s"
			    " [-d]"
			    " [-l]"
			    " [-n loopcount]"
			    " [-O objref]"
			    " [-x]"
			    "\n", argv [0]
			    );
	    return 1;
        }
    
    if (CORBA_is_nil (objref) == CORBA_B_TRUE) {
	fprintf (stderr, "%s:  must identify non-null target objref\n",
		argv [0]);
	return 1;
    }


    do_tests (objref, loop_count, tests, errors);

    char		*progname = strrchr (argv [0], '/');

    if (progname != 0)
	progname += 1;
    else
	progname = argv [0];

    fprintf (stderr, "%s:  %d loops, %d tests (%d errors)\n",
	progname, loop_count, tests, errors);

    if (exit_later) {
	test1_please_exit (objref, env);
	if (env.exception () != 0)
	    print_exception (env.exception (), "test1_please_exit");
    }

    CORBA_release (objref);

    return errors != 0;
}
