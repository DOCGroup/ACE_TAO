// @(#)test1_svr.cpp	1.7 95/09/25
// Copyright 1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// TEST:	simple IIOP server for "test1.idl" interface.
//
// Starts up, builds an objref, prints its string, listens for
// messages, responds to them.
//

#include	<stdio.h>
#include	<string.h>
#include <ace/Get_Opt.h>

#if	unix
#	include	<unistd.h>		// for getopt on some systems

#else	// windows

#endif

#include	"test1.hh"
#include	<corba/toa.hh>

#include	<corba/debug.hh>



#if !defined (_WIN32)
extern char 	*optarg;	// missing on some platforms
#endif

//
// Skeleton code ... just a macro for a bunch of DSI-based method code,
// in lieu of having an IDL compmiler generate static skeletons.  Static
// skeletons would be more efficient; most mallocation could go away.
//
// Use by:  defining OPERATION macro, call DEFINE_SKEL3 as needed, then
// undef OPERATION.
//
// NOTE:  "v1_copy" below is needed to work around a bug with the
// HP9000 G++ 2.6.3 compiler, with "LongLong".
//
// XXX this could probably be a template ... or could even be merged
// directly into the Dynamic Implementation Routine below.
//
// XXX we must currently use IN_COPY_VALUE since the memory consumed
// by the parameters must be deallocated by the ORB.  When we get an
// updated version of DSI which provides "send it now" semantics,
// these should preallocate the values and not use IN_COPY_VALUE.  A
// net decrease in malloc overhead can be had that way.  (NVList should
// also get a public constructor, and a way to provide the buffer.)
//
#define	DEFINE_SKEL3(name,truetype,truetypename) \
    TEMPLATE_DECL(CORBA_ ## truetype); \
    static void \
    _test1_test_ ## name ( \
	CORBA_ServerRequest	&req, \
	CORBA_Environment	&env \
    ) \
    { \
	CORBA_NVList_ptr	nvlist; \
	CORBA_ ## truetype	scratch = 0; \
	CORBA_Any		temp_value (_tc_CORBA_ ## truetypename, \
				    &scratch, CORBA_B_FALSE); \
 	\
	req.orb ()->create_list (3, nvlist); \
	(void) nvlist->add_value (0, temp_value, \
		CORBA_IN_COPY_VALUE|CORBA_ARG_IN, env); \
	(void) nvlist->add_value (0, temp_value, \
		CORBA_IN_COPY_VALUE|CORBA_ARG_OUT, env); \
	(void) nvlist->add_value (0, temp_value, \
		CORBA_IN_COPY_VALUE|CORBA_ARG_INOUT, env); \
	 \
	req.params (nvlist, env); \
	if (env.exception () != 0) { \
	    dexc (env, "test1_test_" # name "skeleton, req.params"); \
	    return; \
	} \
 	\
	CORBA_ ## truetype 	*v1, *v2, *retval; \
 	\
	v1 = (CORBA_ ## truetype *) nvlist->item (0)->value ()->value (); \
	v2 = new CORBA_ ## truetype; \
	*v2 = (CORBA_ ## truetype) OPERATION (*v1); \
	retval = new CORBA_ ## truetype; \
	*retval = (CORBA_ ## truetype) OPERATION (*v1); \
 	\
	CORBA_Any_ptr		any_val; \
 	\
	any_val = nvlist->item (1)->value (); \
	any_val->replace (any_val->type (), v2, CORBA_B_TRUE, env); \
	if (env.exception () != 0) { \
	    dexc (env, "test1_test_" # name "skeleton, val2 replace"); \
	    return; \
	} \
 	\
	v1 = (CORBA_ ## truetype *) nvlist->item (2)->value ()->value (); \
    	CORBA_ ## truetype v1copy = *v1; \
	*v1 = (CORBA_ ## truetype) OPERATION (v1copy); \
 	\
	any_val = new CORBA_Any (_tc_CORBA_ ## truetypename, \
		retval, CORBA_B_TRUE); \
	req.result (any_val, env); \
	if (env.exception () != 0) { \
	    dexc (env, "test1_test_" # name "skeleton, result"); \
	    return; \
	} \
    } \
    extern calldata test1_ ## name ## _calldata;


extern const calldata test1_void_calldata;

static void
_test1_test_void (
    CORBA_ServerRequest		&req,
    CORBA_Environment		&env
)
{
    CORBA_NVList_ptr		nvlist;

    req.orb ()->create_list (0, nvlist);
    req.params (nvlist, env);

    if (env.exception () != 0)
	dexc (env, "test_throw, get params");
}


//
// Dynamic Skeleton methods for numeric types ... these all just
// cube their parameters in various permutations
//
template <class Type>
Type cube (Type arg)
{
    Type temp = arg;

    temp = temp * arg;
    temp = temp * arg;
    return temp;
}

#define	OPERATION(n)	cube(n)
#if defined(ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
#  define TEMPLATE_DECL(t) template t cube(t)
#else
#  define TEMPLATE_DECL(t)
#endif

DEFINE_SKEL3 (octet, Octet, Octet)

DEFINE_SKEL3 (short, Short, Short)
DEFINE_SKEL3 (ushort, UShort, UShort)

DEFINE_SKEL3 (long, Long, Long)
DEFINE_SKEL3 (ulong, ULong, ULong)

#if	!defined (NONNATIVE_LONGLONG)
	// don't try this on platforms that don't support
	// math on longlongs ...
DEFINE_SKEL3 (longlong, LongLong, LongLong)
DEFINE_SKEL3 (ulonglong, ULongLong, ULongLong)
#endif	// !NONNATIVE_LONGLONG

DEFINE_SKEL3 (float, Float, Float)
DEFINE_SKEL3 (double, Double, Double)

#if	!defined (NONNATIVE_LONGDOUBLE)
	// don't try this on platforms that don't support
	// math on long doubles ...
DEFINE_SKEL3 (longdouble, LongDouble, LongDouble)
#endif	// !NONNATIVE_LONGDOUBLE

#undef	OPERATION

//
// Dynamic Skeleton methods for Boolean type ... just negates its
// parameters 
//
#define	OPERATION(x)	(!(x))
#undef TEMPLATE_DECL
#define TEMPLATE_DECL(t)
DEFINE_SKEL3 (boolean, Boolean, Boolean)
#undef	OPERATION

//
// For character types, output is same as input, no magic needed.
//
#define	OPERATION(x)	(x)
DEFINE_SKEL3 (char, Char, Char)
DEFINE_SKEL3 (wchar, WChar, WChar)
#undef	OPERATION

//
// For objref, typecode, output is same as input but duplication
// is needed
//
#define	OPERATION(x)	((x) ? (x)->AddRef() : 0, x)
DEFINE_SKEL3 (Object, Object_ptr, Object)
DEFINE_SKEL3 (TypeCode, TypeCode_ptr, TypeCode)
#undef	OPERATION

//
// For string, output is copy of input
//
#define	OPERATION(x)	(CORBA_string_copy(x))
DEFINE_SKEL3 (string, String, String)
#undef	OPERATION

//
// For wstring, output is copy of input
//
#define	OPERATION(x)	(CORBA_wstring_copy(x))
DEFINE_SKEL3 (wstring, WString, WString)
#undef	OPERATION

//
// XXX IMPLEMENT THE REST OF THE DATA TYPES
//
// any, principal -- out is in
//


//
// All cases, "case_num" in the exception is the same as the 'in' param
//	* negative or zero, throws x1
//	* positive even cases, throws x2 with obj = null objref
//	* positive odd cases, throws x2 with obj = target objref
//
// exception x1 { long case_num; };
// exception x2 { Object obj; long case_num; };
// 
// void test_throw (in long case_num) raises (x1, x2);
//

extern const calldata test1_test_throw_calldata;

static void
_test1_test_throw (
    CORBA_ServerRequest		&req,
    CORBA_Environment		&env
)
{
    CORBA_NVList_ptr		nvlist;
    CORBA_NamedValue_ptr	nv;
    CORBA_Any			temp_value (_tc_CORBA_Long);
    CORBA_Long			value;

    req.orb ()->create_list (0, nvlist);
    nv = nvlist->add_value (0, temp_value, CORBA_ARG_IN, env);

    req.params (nvlist, env);
    if (env.exception () != 0) {
	dexc (env, "test_throw, get params");
	return;
    }

    value = *(CORBA_Long *)nv->value ()->value ();
    if (env.exception () != 0) {
	dexc (env, "test_throw, param value");
	return;
    }

    CORBA_Any_ptr		any;

    if (value <= 0) {
	test1_x1		*x;

	x = new test1_x1 (value);
	any = new CORBA_Any (_tc_test1_x1, x, CORBA_B_TRUE);

    } else if (value & 0x01) {
	test1_x2		*x;

	x = new test1_x2 (CORBA_Object::_nil (), value);
	any = new CORBA_Any (_tc_test1_x2, x, CORBA_B_TRUE);

    } else {
#if	0
	test1_x2		*x;

	x = new test1_x2 (req.oa()->target (), value);
	any = new CORBA_Any (_tc_test1_x2, x, CORBA_B_TRUE);
#else
	//
	// XXX right now, we don't have a target() operation on the
	// TOA ... needs to be added.  Verify the client side memory
	// leak of pointers embedded in user exceptions is fixed, too.
	//
	env.exception (new CORBA_IMP_LIMIT (COMPLETED_NO));
	return;
#endif
    }

    req.exception (USER_EXCEPTION, any, env);
}


//
// This table is used to associate operation names with the Dynamic
// Skeleton method ... someday it could return static skeletons.
//
#define	DECL_SKEL(name) \
	{ & test1_ ## name ## _calldata, _test1_test_ ## name }

static const skel_entry test1_operations [] = {
    DECL_SKEL (void),

    DECL_SKEL (octet),
    DECL_SKEL (char),

    DECL_SKEL (wchar),

    DECL_SKEL (short),
    DECL_SKEL (ushort),
    DECL_SKEL (long),
    DECL_SKEL (ulong),

#if	!defined (NONNATIVE_LONGLONG)
    // don't try this on platforms that don't support
    // math on longlongs ...
    DECL_SKEL (longlong),
    DECL_SKEL (ulonglong),
#endif

    DECL_SKEL (float),
    DECL_SKEL (double),

#if	!defined (NONNATIVE_LONGDOUBLE)
	// don't try this on platforms that don't support
	// math on long doubles ...
    DECL_SKEL (longdouble),
#endif	// !NONNATIVE_LONGDOUBLE

    DECL_SKEL (boolean),

    DECL_SKEL (Object),
    DECL_SKEL (TypeCode),

    DECL_SKEL (string),
    DECL_SKEL (wstring),

    { & test1_test_throw_calldata, _test1_test_throw },

    { 0, 0 }
};


//
// Dispatch to method code ...
//
// Knows how to interpret "context" to get target objref, and where to
// get second level skeletons for that target.  Both of those operations
// will generally be abstracted (into library and stub code) so the main
// body of this routine would be invisible to most applications.
//
// However, there are applications that need to do this stuff themselves
// (like bridging between environments, e.g. different ORBs, languages, or
// other object systems).  Everything needed to work without using an IDL
// compiler is a public, supported API.
//
static void
level1_skeleton (
    CORBA_OctetSeq		&key,
    CORBA_ServerRequest		&req,
    void			*context,
    CORBA_Environment		&env
)
{
    //
    // Verify that the target object and "this" object have the
    // same key.  Normally, this would be used to figure out
    // which object was the target, and hence which operations
    // vector to dispatch the request.
    //
    CORBA_OctetSeq		*obj_key;

    obj_key = (CORBA_OctetSeq *) context;
    if (obj_key->length != key.length
	    || ACE_OS::memcmp (obj_key->buffer, key.buffer,
		    obj_key->length) != 0) {
	env.exception (new CORBA_OBJECT_NOT_EXIST (COMPLETED_NO));
#ifdef	DEBUG
	if (debug_level)
	    dmsg_opaque ("request to nonexistent object, key = ",
		    key.buffer, key.length);
#endif
	return;
    }

    //
    // Find a "level 2 skeleton" for this operation, then
    // call it with the right per-object state.
    //
    const skel_entry		*entry;
    CORBA_String		opname;

    opname = req.op_name ();
    for (entry = &test1_operations [0]; entry->op_descriptor; entry++) {
	if (strcmp ((char *)opname, entry->op_descriptor->opname) == 0) {
	    entry->impl_skeleton (req, env);
	    return;
	}
    }

    //
    // XXX True top-level skeleton code would also have to understand
    // the built-in operations:
    //
    // * _is_a (for narrowing tests) ... can be derived by searching an
    //	 appropriately structured graph of level2 skeletons.
    //
    // * _non_existent ... if the level1 skeleton consults a module
    //	 which understands object lifespans, this should be simple.
    // 
    // * _get_interface ... could either (a) fail; (b) return the ref
    //   from some IFR; or most interestingly (c) return a ref to some
    //	 code in this process that can answer all IFR queries from the
    //	 skeleton data structures.
    //
    // * _get_implementation  ... return some administrative hook to
    //   the object implementation:
    //
    // No other legal operations start with a character that's not an
    // ASCII alphanumeric, for what it's worth.
    //
    // The skeleton might want to use data in the object key to find
    // the objref's type; if it's integrated with object creation, and
    // an per-process implementation repository, this should be easy.
    //

    //
    // bypass level 2 skeletons for this one ...
    //
    if (strcmp ((char *) opname, "please_exit") == 0) {
	dmsg ("I've been asked to shut down...");
	req.oa ()->please_shutdown (env);
	dexc (env, "please_exit, please_shutdown");
	return;
    }

    //
    // No match.  Operation not implemented; say so.
    //
    dmsg1 ("unknown operation, %s", opname);
    env.exception (new CORBA_BAD_OPERATION (COMPLETED_NO));
}


//
// Create and print the objref, listen for calls on it until done.
//
extern void
print_exception (const CORBA_Exception *, const char *, FILE *f=stdout);

int
OA_listen (
    CORBA_ORB_ptr	orb_ptr,
    TOA_ptr		oa_ptr,
    CORBA_String	key,
    int			idle
)
{
    //
    // Create the object we'll be implementing.
    //
    CORBA_OctetSeq	obj_key;
    CORBA_Object_ptr	obj;
    CORBA_Environment	env;

    obj_key.buffer = (CORBA_Octet *) key;
    obj_key.length = obj_key.maximum = ACE_OS::strlen (key);

    obj = oa_ptr->create (obj_key, (CORBA_String) "", env);
    if (env.exception () != 0) {
	print_exception (env.exception (), "TOA::create");
	return 1;
    }

    //
    // Stringify the objref we'll be implementing, and
    // print it to stdout.  Someone will take that string
    // and give it to some client.
    //
    CORBA_String	str;

    str = orb_ptr->object_to_string (obj, env);
    if (env.exception () != 0) {
	print_exception (env.exception (), "object2string");
	return 1;
    }
    ACE_OS::puts ((char *)str);
    ACE_OS::fflush (stdout);
    dmsg1 ("listening as object '%s'", str);

    //
    // Clean up -- "key" is sufficient to dispatch all requests.
    //
    CORBA_release (obj);
    CORBA_string_free (str);
    env.clear ();

    //
    // Handle requests for this object until we're killed, or one of
    // the methods makes us exit.
    //
    // NOTE:  apart from registering the top level skeleton, the rest
    // of this loop is exactly what TOA::run() does.  It's here to
    // show there's no magic.
    //
    oa_ptr->register_dir (level1_skeleton, &obj_key, env);
    if (env.exception () != 0) {
	print_exception (env.exception (), "register_dir");
	return 1;
    }

    for (;;) {
	if (idle == -1)
	    oa_ptr->get_request (CORBA_B_FALSE, 0, env);
	else {
	    timeval		tv;

	    tv.tv_sec = idle;
	    tv.tv_usec = 0;
	    oa_ptr->get_request (CORBA_B_FALSE, &tv, env);
	}

	CORBA_Exception_ptr	xp;

	if ((xp = env.exception ()) != 0) {
	    CORBA_Environment	env2;	// XXX
	    char		*id;

	    id = env.exception ()->id ();

	    //
	    // We get BAD_INV_ORDER if we call get_request() after
	    // shutdown was initiated.  Simpler to rely on that
	    // than to arrange any handshaking in this simple app.
	    //
	    if (strcmp (id, _tc_CORBA_BAD_INV_ORDER->id (env2)) == 0) {
		break;

	    //
	    // Other exceptions are errors.
	    //
	    } else {
		print_exception (env.exception (), "TOA::get_request");
		return 1;
	    }
	}
	env.clear ();
    }

    //
    // Shut down the OA -- recycles all underlying resources (e.g. file
    // descriptors, etc).
    //
    // XXX shutdown is not quite the same as release, unless we want mem
    // leaks to cause some rude failure modes.  TOA just hasn't been
    // updated yet to have any handshake about this though.
    //
    oa_ptr->Release ();
    return 0;
}


//
// Standard command line parsing utilities used.
//
int
main (int    argc, char   *argv[])
{
    CORBA_Environment	env;
    CORBA_ORB_ptr	orb_ptr;
    TOA_ptr		oa_ptr;
    CORBA_String	key = (CORBA_String) "elvis";
    char		*oa_name = 0;
    char		*orb_name = "internet";
    int			idle = -1;

    //
    // Parse the command line, get options
    //
    ACE_Get_Opt get_opt (argc, argv, "dln:O:x");
    int			c;

    while ((c = get_opt ()) != -1)
      switch (c)
	{
	case 'd':			// more debug noise
	  debug_level++;
	  continue;

	case 'i':			// idle seconds b4 exit
	  idle = ACE_OS::atoi (get_opt.optarg);
	  continue;

	case 'k':			// key (str)
	  key = (CORBA_String) get_opt.optarg;
	  continue;

	case 'o':			// orb name
	  orb_name = get_opt.optarg;
	  continue;

	case 'p':			// portnum
	  oa_name = get_opt.optarg;
	  continue;

	  // XXX set debug filters ...

	  //
	  // XXX ignore OMG-specified options ... hope nobody ever tries
	  // to use that "-ORB* param" and "-OA* param" syntax, it flies
	  // in the face of standard command parsing algorithms which
	  // require single-character option specifiers.
	  //


	case '?':
	default:
	  ACE_OS::fprintf (stderr, "usage:  %s"
			   " [-d]"
			   " [-i idle_seconds]"
			   " [-k object_key=elvis]"
			   " [-o orbname=internet]"
			   " [-p oa_name]"
			   "\n", argv [0]
			   );
	  return 1;
	}

    orb_ptr = CORBA_ORB_init (argc, argv, orb_name, env);
    if (env.exception () != 0) {
	print_exception (env.exception (), "ORB init");
	return 1;
    }

    //
    // The TOA may or may not actually be named ...
    //
    oa_ptr = TOA::get_named_toa (orb_ptr, oa_name, env);
    if (env.exception () != 0) {
	print_exception (env.exception (), "OA init");
	return 1;
    }

    return OA_listen (orb_ptr, oa_ptr, key, idle);
}

