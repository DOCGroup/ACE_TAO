// @(#)svr.cpp	1.6 95/10/02
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// TEST:	simple IIOP server for "cubit.idl" interface.
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

#include	"cubit.hh"
#include	<corba/toa.hh>


//
// XXX a general debug/trace facility would be handy
//
#include	<corba/debug.hh>

//
// XXX this stuff is ugly but needed, since this exposes features
// (IIOP forwarding) that TOA doesn't provide.
//
#include	<connmgr.hh>
#include	<tcpoa.hh>


// extern char 	*optarg;	// missing on some platforms

extern void
print_exception (const CORBA_Exception *, const char *, FILE *f=stdout);


static void
is_a_skel (
    CORBA_ServerRequest		&req,
    CORBA_Environment		&env
)
{
    CORBA_NVList_ptr		nvlist;
    CORBA_NamedValue_ptr	nv;
    CORBA_Any			temp_value (_tc_CORBA_String);

    req.orb()->create_list (0, nvlist);
    nv = nvlist->add_value (0, temp_value, CORBA_ARG_IN, env);

    req.params (nvlist, env);
    if (env.exception () != 0) {
	dexc (env, "is_a_skel, get params");
	return;
    }

    CORBA_Boolean *retval;
    CORBA_String value = *(CORBA_String *) nv->value()->value();

    //
    // This object's type "is_a" conformant subtype of the type whose
    // ID ("Repository ID") is passed if it's (a) either the same type
    // as indicated by the ID, or (b) one of the classes from which it
    // inherits is_a subtype, or (c) the ID is for the generic CORBA
    // object.
    //
    // XXX IDs should be compared recognizing that the "IDL" and "DCE"
    // ID types have a minor version code, and that if the minor code
    // (a 16 bit unsigned integer, in ASCII form) of the ID passed is
    // not greater than one of the IDs we know, it's still compatible.
    //
    // XXX "env" should be checked to see if TypeCode::id() reported
    // an exception ...
    //
    if (ACE_OS::strcmp ((char*)value, Cubit__id) == 0
	|| ACE_OS::strcmp ((char *)value, _tc_CORBA_Object->id (env)) == 0)
	retval = new CORBA_Boolean (CORBA_B_TRUE);
    else
	retval = new CORBA_Boolean (CORBA_B_FALSE);

    CORBA_Any *any = new CORBA_Any (_tc_CORBA_Boolean, retval, CORBA_B_TRUE);

    req.result (any, env);
    dexc (env, "_is_a, result");
}


//
// Dispatch to Skeletons
//
// XXX explore packaging most of this as part of the TCP_OA !!
//
static void
tcpoa_dispatch (
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
    // Find a "skeleton" (nyet :-) entry for this operation,
    // then call it with the right per-object state.  (Someday this
    // search will be sped up, e.g. by hashing or binary search.)
    //
    const skel_entry		*entry;
    CORBA_String		opname;

    opname = req.op_name ();

    for (entry = &Cubit_operations [0]; entry->op_descriptor; entry++) {
	if (ACE_OS::strcmp ((char *)opname, entry->op_descriptor->opname) == 0) {
	    entry->impl_skeleton (req, env);
	    return;
	}
    }

    //
    // Try one of the ORB's built-in operations.
    //
    // XXX the rest too:  _non_existent (just return false),
    // _get_interface (needs an interface repository reference for this
    // objref's type), and _get_implementation (needs an implementation
    // repository).
    //
    if (ACE_OS::strcmp ((char *)opname, "_is_a") == 0) {
	is_a_skel (req, env);
	return;
    }

    //
    // No match.  Operation not implemented; say so.
    //
    dmsg1 ("unknown operation, %s", opname);
    env.exception (new CORBA_BAD_OPERATION (COMPLETED_NO));
}

//
// forwarding support
//
static CORBA_Object_ptr	fwd_ref;

static void
tcpoa_forwarder (
    CORBA_OctetSeq	&key,
    CORBA_Object_ptr	&the_ref,
    void		*context,
    CORBA_Environment	&env
)
{
  CORBA_OctetSeq		*obj_key;

  obj_key = (CORBA_OctetSeq *) context;

  if (obj_key->length == key.length
      && ACE_OS::memcmp (obj_key->buffer, key.buffer, key.length) == 0) {
    the_ref = fwd_ref->_duplicate (fwd_ref);
  } else
    env.exception (new CORBA_OBJECT_NOT_EXIST (COMPLETED_NO));
}


//
// Socket-based passive OA entry point
//


//
// Standard command line parsing utilities used.
//
int
main (
    int    argc,
    char   *argv[]
)
{
  CORBA_Environment	env;
  CORBA_ORB_ptr	orb_ptr;
  TCP_OA_ptr		oa_ptr;
  CORBA_Boolean	do_fork = CORBA_B_FALSE;
  CORBA_Boolean	do_threads = CORBA_B_FALSE;
  CORBA_String	key = (CORBA_String) "key0";
  char		*oa_name = 0;
  char		*orb_name = "internet";
  int			idle = -1;

  //
  // Parse the command line, get options
  //
  ACE_Get_Opt opts (argc, argv, "di:fk:o:p:t");
  int			c;

  while ((c = opts()) != -1)
    switch (c) {
    case 'd':			// more debug noise
      debug_level++;
      continue;

    case 'i':			// idle seconds b4 exit
      idle = ACE_OS::atoi (opts.optarg);
      continue;

    case 'f':			// fork child server
      do_fork = CORBA_B_TRUE;
      continue;

    case 'k':			// key (str)
      key = (CORBA_String) opts.optarg;
      continue;

    case 'o':			// orb name
      orb_name = opts.optarg;
      continue;

    case 'p':			// portnum
      oa_name = opts.optarg;
      continue;
	
    case 't':			// create thread-per-request
      do_threads = CORBA_B_TRUE;
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
		       " [-f]"
		       " [-i idle_seconds]"
		       " [-k]"
		       " [-k object_key=key0]"
		       " [-o orb_name=internet]"
		       " [-p portnum=5555]"
		       " [-t]"
		       "\n", argv [0]
		       );
      return 1;
    }

  orb_ptr = CORBA_ORB_init (argc, argv, orb_name, env);
  if (env.exception () != 0) {
    print_exception (env.exception (), "ORB init");
    return 1;
  }

  ACE_INET_Addr svraddr;
  if (oa_name == 0)
    svraddr.set((u_short)0, "watusi");
  else
    svraddr.set(oa_name);

  oa_ptr = TCP_OA::init (orb_ptr, svraddr, env);
  if (env.exception () != 0) {
    print_exception (env.exception (), "OA init");
    return 1;
  }
  // Register the OA with ACE_ROA
  ACE_ROA::oa(oa_ptr);		// Should this be done in TCP_OA's CTOR?

#if 0
  // This is the old call and syntax
  return OA_listen (orb_ptr, oa_ptr, key, idle, do_fork, do_threads);
  int
    OA_listen (
	       CORBA_ORB_ptr	orb_ptr,
	       TCP_OA_ptr		oa_ptr,
	       CORBA_String	key,
	       int			idle,
	       CORBA_Boolean	do_fork,
	       CORBA_Boolean	do_threads
	       )
    {
      return 0;
    }
#endif
  //
  // Create the object we'll be implementing.
  //
  CORBA_OctetSeq	obj_key;
  CORBA_Object_ptr	obj;

  obj_key.buffer = (CORBA_Octet *) key;
  obj_key.length = obj_key.maximum = ACE_OS::strlen ((char *)key);

  env.clear();
  obj = oa_ptr->create (obj_key, (CORBA_String) "", env);
  if (env.exception () != 0) {
    print_exception (env.exception (), "TCP_OA::create");
    return 1;
  }

  //
  // Stringify the objref we'll be implementing, and
  // print it to stdout.  Someone will take that string
  // and give it to some client.  Then release the object.
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
  CORBA_release (obj);
  obj = 0;

  //
  // If we're forking a child server, do so -- read the objref
  // it'll use, and prepare to forward all requests to it.  That
  // objref has a dynamically assigned port. 
  //
  if (do_fork) {
#if	defined (HAVE_POPEN)
    FILE		*f = popen ("exec ./svr -i120 -kbaskerville", "r");
    char		buffer [BUFSIZ];

    if (ACE_OS::fgets (buffer, sizeof buffer, f) != buffer) {
      ACE_OS::fprintf (stderr, "error: can't read from child\n");
      return 1;
    }
    fwd_ref = orb_ptr->string_to_object ((CORBA_String) buffer, env);
    if (env.exception () != 0) {
      print_exception (env.exception (), "string2object");
      return 1;
    }

    //
    // NOTE:  don't fclose("f") since some systems make that the
    // same as pclose("f").  Pclose waits for the child to exit,
    // causing a deadlock since the child won't exit until it's
    // told to do so by a client, but no client can be redirected 
    // to the child until the pclose returns ...
    //
#else
    ACE_OS::fprintf (stderr, "error:  no popen(), can't create child\n");
    env.exception (new CORBA_IMP_LIMIT(COMPLETED_NO));
    return 1;
#endif	// !defined (HAVE_POPEN)
  }

  //
  // Handle requests for this object until we're killed, or one of
  // the methods asks us to exit.
  //
  // NOTE:  for multithreaded environments (e.g. POSIX threads) also
  // want to use threads.  The current notion is to dedicate a thread
  // to a "read" on each client file descriptor, and then when that
  // successfully gets a Request message, to start another thread
  // reading that descriptor while the first one creates the Reply.
  //
  // This will accentuate the need for server-side policies to address
  // resource management, such as shutting down connections that have
  // no requests in progress after they've been idle for some time
  // period (e.g. 10 minutes), and reclaiming the thread used by that
  // connection. 
  //
  int terminationStatus = 0;
#if defined(USE_ACE_EVENT_HANDLING)
  // Set the callbacks for our implementation (cheesy!!!)
  ACE_ROA::upcall(tcpoa_dispatch);
  ACE_ROA::forwarder(fwd_ref ? tcpoa_forwarder : 0);
  ACE_ROA::context(&obj_key);

  while (ACE_ROA::end_reactor_event_loop_ == 0)
    {
      int result = ACE_ROA::reactor()->handle_events ();

      if (result == -1)
	{
	  terminationStatus = -1;
	  break;
	}
    }
#else
  while (oa_ptr->shutting_down () != CORBA_B_TRUE) {
    if (idle == -1)
      oa_ptr->get_request (tcpoa_dispatch,
			   fwd_ref ? tcpoa_forwarder : 0,
			   do_threads, &obj_key, 0, env);
    else {
      timeval		tv;

      tv.tv_sec = idle;
      tv.tv_usec = 0;
      oa_ptr->get_request (tcpoa_dispatch,
			   fwd_ref ? tcpoa_forwarder : 0,
			   do_threads, &obj_key, &tv, env);
    }

    //
    // XXX "env2" should be checked to see if TypeCode::id() reported
    // an exception ...
    //
    CORBA_Environment	env2;

    if (env.exception () != 0
	&& ACE_OS::strcmp ((char *)env.exception ()->id (),
			   _tc_CORBA_INITIALIZE->id (env2)) == 0) {
      print_exception (env.exception (), "TCP_OA::get_request");
      return 1;
    }
    env.clear ();
  }
#endif

  //
  // Shut down the OA -- recycles all underlying resources (e.g. file
  // descriptors, etc).
  //
  oa_ptr->clean_shutdown (env);
  return terminationStatus;
}
