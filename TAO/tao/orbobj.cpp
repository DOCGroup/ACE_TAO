// @ (#)orbobj.cpp	1.8 95/09/24
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:		CORBA::ORB operations
//
// XXX as with TAO, this has a strong coupling to the Internet ORB
// (IIOP) code.  We should make it know less about that protocol
// component and have a loose table-driven coupling to ORB/protocol
// library components.

#include	"orb.h"
#include	"stub.h"
#include	"iioporb.h"		// XXX

#include        "params.h"
#include        "boa.h"
#include        "roa.h"
#include	<initguid.h>

extern void __TC_init_table (void);
extern void __TC_init_standard_exceptions (CORBA_Environment &env);

#if defined (SIG_IGN_BROKEN)
#	undef SIG_IGN 
#	define SIG_IGN ((RETSIGTYPE (*) (int))1)
#endif	// NeXT

// COM's IUnknown support

// {A201E4C6-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_ORB,
0xa201e4c6, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

#if 0
// {A201E4C7-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_STUB_Object,
0xa201e4c7, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);
#endif

ULONG __stdcall
CORBA_ORB::Release (void)
{
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, mon, this->lock_, 0));

    ACE_ASSERT (this != 0);

    if (--refcount_ != 0)
      return refcount_;
  }

  delete this;
  return 0;
}

// ORB initialisation, per OMG document 94-9-46.
//
// XXX in addition to the "built in" Internet ORB, there will be ORBs
// which are added separately, e.g. through a DLL listed in the
// registry.  Registry will be used to assign orb names and to
// establish which is the default.

CORBA_ORB_ptr
CORBA_ORB_init (int &/* argc */,
		char *const /* argv */,
		char *orb_name,
		CORBA_Environment &env)
{
#if defined (ACE_HAS_THREADS)
  // @@ This use of a static is evil.  Please fix...
  static ACE_Thread_Mutex lock;
  ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock, 0);
#endif /* ACE_HAS_THREADS */

  env.clear ();

  // Verify some of the basic implementation requirements.  This test
  // gets optimized away by a decent compiler (or else the rest of the
  // routine does).
  //
  // NOTE:  we still "just" assume that native floating point is IEEE.

  if (sizeof (CORBA_Short) != 2
      || sizeof (CORBA_Long) != 4
      || sizeof (CORBA_LongLong) != 8
      || sizeof (CORBA_Float) != 4
      || sizeof (CORBA_Double) != 8
      || sizeof (CORBA_LongDouble) != 16
      || sizeof (CORBA_WChar) < 2
      || sizeof (void *) != SIZEOF_VOID_P)
    {
      env.exception (new CORBA_INITIALIZE (COMPLETED_NO));
      return 0;
    }

  // ignoring argc, argv for now -- no arguments we care about
  //
  // XXX should remove any of the "-ORB*" arguments that we know about
  // ... and report errors for the rest.
  //
  // Parsing of these arguments should set values in a
  // TAO_ORB_Parameters instance, likely the one contained in the
  // newly-created ORB.

#ifdef	DEBUG
  // Make it a little easier to debug programs using this code.
  {
    char *value = ACE_OS::getenv ("TAO_ORB_DEBUG");

    if (value != 0) 
      {
	TAO_debug_level = ACE_OS::atoi (value);
	if (TAO_debug_level <= 0)
	  TAO_debug_level = 1;
	dmsg1 ("TAO_debug_level == %d", TAO_debug_level);
      }
  }
#endif	/* DEBUG */

  // On Win32, we should be collecting information from the Registry
  // such as what ORBs are configured, specific configuration details
  // like whether they generate IOR or URL style stringified objrefs
  // and which addresses they listen to (e.g. allowing multihomed
  // hosts to implement firewalls), user-meaningful orb names (they
  // will normally indicate domains), and more.
  //
  // On UNIX, we should collect that from some private config file.
  //
  // Instead, this just treats the "internet" ORB name specially and
  // makes it always use URL-style stringified objrefs, where the
  // hostname and TCP port number are explicit (and the whole objref
  // is readable by mortals).
  CORBA_Boolean	use_ior;

  if (orb_name != 0 && ACE_OS::strcmp (orb_name, "internet") == 0)
    use_ior = CORBA_B_FALSE;
  else
    use_ior = CORBA_B_TRUE;

#ifdef	SIGPIPE
  // @@ Is there a better way to deal with this in a portable manner? --cjc
  //
  // Impractical to have each call to the ORB protect against the
  // implementation artifact of potential writes to dead connections,
  // as it'd be way expensive.  Do it here; who cares about SIGPIPE in
  // these kinds of applications, anyway?
 (void) ACE_OS::signal (SIGPIPE, SIG_IGN);
#endif	// SIGPIPE

  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  // Call various internal initialization routines.
  __TC_init_table ();
  __TC_init_standard_exceptions (env);
  if (env.exception () != 0)
    return 0;

  // Inititalize the "ORB" pseudo-object now.
  IIOP_ORB_ptr the_orb = TAO_ORB::instance ();
  the_orb->use_omg_ior_format (CORBA_Boolean (use_ior));
  
  return the_orb;
}

void
CORBA_ORB::create_list (CORBA_Long count,
                        CORBA_NVList_ptr &retval)
{
  assert (count <= UINT_MAX);

  retval = new CORBA_NVList;

  if (count != 0) 
    {
      retval->_len = 0;
      retval->_max = (u_int) count;
      retval->_values = (CORBA_NamedValue_ptr) ACE_OS::calloc ((u_int) count,
							       sizeof (CORBA_NamedValue));
    }
}

// This is a server-side internal routine; it's not available to any
// portable code except method code, which moreover may not access the
// state variable directly since its implemention may differ between
// ORBs.
//
// XXX it's server-side so should be OA-specific and not in this module

CORBA_ORB_ptr
_orb (void)
{
  return TAO_ORB::instance ();
}

CORBA_BOA_ptr CORBA_ORB::BOA_init (int &argc,
				   char **argv,
				   const char *boa_identifier)
{
  // parse the arguments looking for options starting with -OA. After
  // processing these options, move all these to the end of the argv
  // list and decrement argc appropriately.

  TAO_OA_Parameters *params = TAO_OA_PARAMS::instance ();  //should have been BOA_Parameters
  CORBA_BOA_ptr rp;
  CORBA_String_var id = boa_identifier;
  CORBA_String_var host = CORBA_string_dup ("");
  CORBA_String_var demux = CORBA_string_dup ("dynamic_hash"); // default, at least for now
  CORBA_UShort port = 5001;  // some default port -- needs to be a #defined value
  CORBA_ULong tablesize = 0; // default table size for lookup tables
  CORBA_Boolean use_threads = CORBA_B_FALSE;
  ACE_INET_Addr rendezvous;
  CORBA_Environment env;

  for (int i = 0; i < argc; )
    {
      // @@ Can you please add comments describing each of these options? --doug
      // @@ Andy, could you review these since you wrote the code --cjc
      
      if (ACE_OS::strcmp (argv[i], "-OAid") == 0)
        {
          // Specify the name of the OA
	  if (i + 1 < argc)
	    id = CORBA_string_dup (argv[i + 1]);

	  for (int j = i ; j + 2 < argc ; j++)
	    argv[j] = argv[j + 2];

	  argc -= 2;
        }
      else if (ACE_OS::strcmp (argv[i], "-OAhost") == 0)
	{
          // Specify the name of the host (i.e., interface) on which
          // the server should listen
	  if (i + 1 < argc)
	    host = CORBA_string_dup (argv[i + 1]);

	  for (int j = i ; j + 2 < argc ; j++)
	    argv[j] = argv[j + 2];

	  argc -= 2;
	}
      else if (ACE_OS::strcmp (argv[i], "-OAport") == 0)
	{
          // Specify the port number/name on which we should listen
	  if (i + 1 < argc)
            // @@ We shouldn't limit this to being specified as an int! --cjc
	    port = ACE_OS::atoi (argv[i + 1]);

	  for (int j = i ; j + 2 < argc ; j++)
	    argv[j] = argv[j + 2];

	  argc -= 2;
	}
      else if (ACE_OS::strcmp (argv[i], "-OAobjdemux") == 0)
	{
          // Specify the demultiplexing strategy to be used for object
          // demultiplexing
	  if (i + 1 < argc)
	    demux = CORBA_string_dup (argv[i+1]);

	  for (int j = i ; j + 2 < argc ; j++)
	    argv[j] = argv[j + 2];

	  argc -= 2;
	}
      else if (ACE_OS::strcmp (argv[i], "-OAtablesize") == 0)
	{
          // @@ Specify the table size used for ????
	  if (i + 1 < argc)
	    tablesize = ACE_OS::atoi (argv[i+1]);

	  for (int j = i ; j + 2 < argc ; j++)
	    argv[j] = argv[j + 2];

	  argc -= 2;
	}
      else if (ACE_OS::strcmp (argv[i], "-OArcvsock") == 0)
	{
          // Specify the size of the socket's receive buffer
	}
      else if (ACE_OS::strcmp (argv[i], "-OAsndsock") == 0)
	{
          // Specify the size of the socket's send buffer
	}
      else if (ACE_OS::strcmp (argv[i], "-OAthread") == 0)
	{
          // Specify whether or not threads should be used.
	  use_threads = CORBA_B_TRUE;
	  for (int j = i ; j + 1 < argc ; j++)
	    argv[j] = argv[j + 1];

	  argc -= 1;
	}
      else
	i++;
    }
  
  // create a INET_Addr
  if (ACE_OS::strlen (host) > 0)
    rendezvous.set (port, host);
  else 
    rendezvous.set (port);
  
  //    ACE_MT (ACE_GUARD (ACE_Thread_Mutex, roa_mon, lock_));

  if (params->oa ())
    {
      env.exception (new CORBA_INITIALIZE (COMPLETED_NO));
      return 0;
    }

  // set all parameters
  params->using_threads (use_threads);
  params->demux_strategy (demux);
  params->addr (rendezvous);
  params->upcall (CORBA_BOA::dispatch);
  params->tablesize (tablesize);

#if defined (ROA_NEEDS_REQ_KEY)
 (void) ACE_Thread::keycreate (&req_key_);
#endif
    
  ACE_NEW_RETURN (rp, ROA (this, env), 0);

  return rp;
}

#if !defined (__ACE_INLINE__)
#  include "orbobj.i"
#endif
