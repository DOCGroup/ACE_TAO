// @(#)test1_svr.cpp	1.7 95/09/25
// Copyright 1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// TEST:	simple IIOP server for "test1.idl" interface.
//
// Starts up, builds an objref, prints its string, listens for
// messages, responds to them.
//

#include <stdio.h>
#include <string.h>
#include <ace/Get_Opt.h>

#if	unix
#	include	<unistd.h>		// for getopt on some systems

#else	// windows

#endif

#include "test1.h"
#include "tao/corba.h"


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
    static void \
    _test1_test_ ## name ( \
	CORBA::ServerRequest	&req, \
        CORBA::Object_ptr        obj, \
	CORBA::Environment	&env \
    ) \
    { \
	CORBA::NVList_ptr	nvlist; \
	CORBA:: ## truetype	scratch = 0; \
	CORBA::Any		temp_value (CORBA::_tc_ ## truetypename, \
				    &scratch, CORBA::B_FALSE); \
 	\
	req.orb ()->create_list (3, nvlist); \
	(void) nvlist->add_value (0, temp_value, \
		CORBA::IN_COPY_VALUE|CORBA::ARG_IN, env); \
	(void) nvlist->add_value (0, temp_value, \
		CORBA::IN_COPY_VALUE|CORBA::ARG_OUT, env); \
	(void) nvlist->add_value (0, temp_value, \
		CORBA::IN_COPY_VALUE|CORBA::ARG_INOUT, env); \
	 \
	req.params (nvlist, env); \
	if (env.exception () != 0) { \
	    dexc (env, "test1_test_" # name "skeleton, req.params"); \
	    return; \
	} \
 	\
	CORBA:: ## truetype 	*v1, *v2, *retval; \
 	\
	v1 = (CORBA:: ## truetype *) nvlist->item (0)->value ()->value (); \
	v2 = new CORBA:: ## truetype; \
	*v2 = (CORBA:: ## truetype) OPERATION (*v1); \
	retval = new CORBA:: ## truetype; \
	*retval = (CORBA:: ## truetype) OPERATION (*v1); \
 	\
	CORBA::Any_ptr		any_val; \
 	\
	any_val = nvlist->item (1)->value (); \
	any_val->replace (any_val->type (), v2, CORBA::B_TRUE, env); \
	if (env.exception () != 0) { \
	    dexc (env, "test1_test_" # name "skeleton, val2 replace"); \
	    return; \
	} \
 	\
	v1 = (CORBA:: ## truetype *) nvlist->item (2)->value ()->value (); \
    	CORBA:: ## truetype v1copy = *v1; \
	*v1 = (CORBA:: ## truetype) OPERATION (v1copy); \
 	\
	any_val = new CORBA::Any (CORBA::_tc_ ## truetypename, \
		retval, CORBA::B_TRUE); \
	req.result (any_val, env); \
	if (env.exception () != 0) { \
	    dexc (env, "test1_test_" # name "skeleton, result"); \
	    return; \
	} \
    } \
    extern TAO_Call_Data test1_ ## name ## _calldata;


extern const TAO_Call_Data test1_void_calldata;

static void
_test1_test_void (CORBA::ServerRequest &req,
		  CORBA::Object_ptr obj,
		  CORBA::Environment &env)
{
  CORBA::NVList_ptr nvlist;

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
#define	OPERATION(x)	(CORBA::string_copy(x))
DEFINE_SKEL3 (string, String, String)
#undef	OPERATION

//
// For wstring, output is copy of input
//
#define	OPERATION(x)	(CORBA::wstring_copy(x))
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

extern const TAO_Call_Data test1_test_throw_calldata;

static void
_test1_test_throw (CORBA::ServerRequest &req,
		   CORBA::Object_ptr obj,
		   CORBA::Environment &env)
{
  CORBA::NVList_ptr nvlist;
  CORBA::NamedValue_ptr nv;
  CORBA::Any temp_value (CORBA::_tc_long);
  CORBA::Long value;

  req.orb ()->create_list (0, nvlist);
  nv = nvlist->add_value (0, temp_value, CORBA::ARG_IN, env);

  req.params (nvlist, env);
  if (env.exception () != 0)
    {
      dexc (env, "test_throw, get params");
      return;
    }

  value = *(CORBA::Long *)nv->value ()->value ();
  if (env.exception () != 0)
    {
      dexc (env, "test_throw, param value");
      return;
    }

  CORBA::Any_ptr any;

  if (value <= 0) 
    {
      test1_x1		*x;

      x = new test1_x1 (value);
      any = new CORBA::Any (_tc_test1_x1, x, CORBA::B_TRUE);

    }
  else if (value & 0x01)
    {
      test1_x2		*x;

      x = new test1_x2 (CORBA::Object::_nil (), value);
      any = new CORBA::Any (_tc_test1_x2, x, CORBA::B_TRUE);

    } 
  else
    {
#if	0
      test1_x2		*x;

      x = new test1_x2 (req.oa()->target (), value);
      any = new CORBA::Any (_tc_test1_x2, x, CORBA::B_TRUE);
#else
      //
      // XXX right now, we don't have a target() operation on the
      // BOA ... needs to be added.  Verify the client side memory
      // leak of pointers embedded in user exceptions is fixed, too.
      //
      env.exception (new CORBA::IMP_LIMIT (COMPLETED_NO));
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

static const TAO_Skel_Entry test1_operations [] = {
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
    CORBA::OctetSeq		&key,
    CORBA::ServerRequest		&req,
    void			*context,
    CORBA::Environment		&env
)
{
  //
  // Verify that the target object and "this" object have the
  // same key.  Normally, this would be used to figure out
  // which object was the target, and hence which operations
  // vector to dispatch the request.
  //
  CORBA::OctetSeq		*obj_key;

  obj_key = (CORBA::OctetSeq *) context;
  if (obj_key->length != key.length
      || ACE_OS::memcmp (obj_key->buffer, key.buffer,
			 obj_key->length) != 0)
    {
    env.exception (new CORBA::OBJECT_NOT_EXIST (COMPLETED_NO));
#ifdef	DEBUG
    if (TAO_debug_level)
      dmsg_opaque ("request to nonexistent object, key = ",
		   key.buffer, key.length);
#endif
    return;
  }

  //
  // Find a "level 2 skeleton" for this operation, then
  // call it with the right per-object state.
  //
  const TAO_Skel_Entry	*entry;
  CORBA::String		opname;

  opname = req.op_name ();
  for (entry = &test1_operations [0]; entry->op_descriptor; entry++)
    {
    if (strcmp ((char *)opname, entry->op_descriptor->opname) == 0)
      {
      // We pass a bogus object because this version of the cubit example is
      // so tightly coupled that it doesn't use it.  Future modifications will,
      // hopefully!
      CORBA::Object_ptr bogus_object = 0;
      entry->impl_skeleton (req, bogus_object, env);
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
  if (strcmp ((char *) opname, "please_exit") == 0)
    {
    dmsg ("I've been asked to shut down...");
    req.oa ()->please_shutdown (env);
    dexc (env, "please_exit, please_shutdown");
    return;
  }

  //
  // No match.  Operation not implemented; say so.
  //
  dmsg1 ("unknown operation, %s", opname);
  env.exception (new CORBA::BAD_OPERATION (COMPLETED_NO));
}


//
// Create and print the objref, listen for calls on it until done.
//
extern void
print_exception (const CORBA::Exception *, const char *, FILE *f=stdout);

int
OA_listen (CORBA::ORB_ptr orb_ptr,
	   CORBA::BOA_ptr oa_ptr,
	   CORBA::String key,
	   int idle)
{
  //
  // Create the object we'll be implementing.
  //
  CORBA::OctetSeq	obj_key;
  CORBA::Object_ptr	obj;
  CORBA::Environment	env;

  obj_key.buffer = (CORBA::Octet *) key;
  obj_key.length = obj_key.maximum = ACE_OS::strlen (key);

  obj = oa_ptr->create (obj_key, (CORBA::String) "", env);
  if (env.exception () != 0)
    {
      print_exception (env.exception (), "BOA::create");
      return 1;
    }

  //
  // Stringify the objref we'll be implementing, and
  // print it to stdout.  Someone will take that string
  // and give it to some client.
  //
  CORBA::String	str;

  str = orb_ptr->object_to_string (obj, env);
  if (env.exception () != 0)
    {
      print_exception (env.exception (), "object2string");
      return 1;
    }
  ACE_OS::puts ((char *)str);
  ACE_OS::fflush (stdout);
  dmsg1 ("listening as object '%s'", str);

  //
  // Clean up -- "key" is sufficient to dispatch all requests.
  //
  CORBA::release (obj);
  CORBA::string_free (str);
  env.clear ();

  //
  // Handle requests for this object until we're killed, or one of
  // the methods makes us exit.
  //
  // NOTE:  apart from registering the top level skeleton, the rest
  // of this loop is exactly what BOA::run() does.  It's here to
  // show there's no magic.
  //
#if 0
  oa_ptr->register_dir (level1_skeleton, &obj_key, env);
#else
  oa_ptr->register_dir (oa_ptr->dispatch, &obj_key, env);
#endif
  if (env.exception () != 0)
    {
      print_exception (env.exception (), "register_dir");
      return 1;
    }

  for (;;) {
    if (idle == -1)
      oa_ptr->get_request (CORBA::B_FALSE, 0, env);
    else
      {
	timeval		tv;

	tv.tv_sec = idle;
	tv.tv_usec = 0;
	oa_ptr->get_request (CORBA::B_FALSE, &tv, env);
      }

    CORBA::Exception_ptr	xp;

    if ((xp = env.exception ()) != 0)
      {
	CORBA::Environment env2;	// XXX
	char *id;

	id = env.exception ()->id ();

	//
	// We get BAD_INV_ORDER if we call get_request() after
	// shutdown was initiated.  Simpler to rely on that
	// than to arrange any handshaking in this simple app.
	//
	if (strcmp (id, CORBA::_tc_bAD_INV_ORDER->id (env2)) == 0)
	  {
	    break;

	    //
	    // Other exceptions are errors.
	    //
	  }
	else
	  {
	    print_exception (env.exception (), "BOA::get_request");
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
  // leaks to cause some rude failure modes.  BOA just hasn't been
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
  CORBA::Environment	env;
  CORBA::ORB_ptr	orb_ptr;
  CORBA::BOA_ptr		oa_ptr;
  CORBA::String	key = (CORBA::String) "elvis";
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
	TAO_debug_level++;
	continue;

      case 'i':			// idle seconds b4 exit
	idle = ACE_OS::atoi (get_opt.optarg);
	continue;

      case 'k':			// key (str)
	key = (CORBA::String) get_opt.optarg;
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
  // The BOA may or may not actually be named ...
  //
  oa_ptr = CORBA::BOA::get_named_boa (orb_ptr, oa_name, env);
  if (env.exception () != 0) {
    print_exception (env.exception (), "OA init");
    return 1;
  }

  return OA_listen (orb_ptr, oa_ptr, key, idle);
}

