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

#if 0
#include "ace/OS.h"    // WARNING! This MUST come before objbase.h on WIN32!
#include <objbase.h>
#include <initguid.h>

#include "tao/orb.h"
#include "tao/stub.h"
#include "tao/iioporb.h"		// XXX
#include "tao/params.h"
#include "tao/boa.h"
#include "tao/roa.h"
#include "tao/nvlist.h"
#include "tao/debug.h"
#endif /* 0 */

#include "tao/corba.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Repository.h"
#include "tao/tao_internals.h"

extern void __TC_init_table (void);
extern void __TC_init_standard_exceptions (CORBA::Environment &env);

#if defined (SIG_IGN_BROKEN)
#	undef SIG_IGN 
#	define SIG_IGN ((RETSIGTYPE (*) (int))1)
#endif /* NeXT */

// COM's IUnknown support

// {A201E4C6-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_ORB,
	     0xa201e4c6, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

// CJC Why was this commented out?
// {A201E4C7-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_STUB_Object,
	     0xa201e4c7, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

CORBA_ORB::CORBA_ORB (void)
  : open_called_(CORBA::B_FALSE),
    client_factory_ (0),
    client_factory_from_service_config_ (CORBA::B_FALSE),
    server_factory_ (0),
    server_factory_from_service_config_ (CORBA::B_FALSE)
{
  this->refcount_ = 1;
}

CORBA_ORB::~CORBA_ORB (void)
{
  TAO_Internal::close_services ();

  if (!this->client_factory_from_service_config_)
    delete client_factory_;

  if (!this->server_factory_from_service_config_)
    delete server_factory_;

  // This assertion isn't valid because our ORB is a singleton
  // assert (refcount_ == 0);
}

int
CORBA_ORB::open (void)
{
  if (this->open_called_)
    return -1;

  this->open_called_ = CORBA::B_TRUE;
  
  TAO_Server_Strategy_Factory *f = this->server_factory ();

  // Initialize the endpoint ... or try!
  if (this->peer_acceptor_.open (this->params()->addr (),
				 TAO_ORB_Core_instance()->reactor(),
				 f->creation_strategy (),
				 f->accept_strategy (),
				 f->concurrency_strategy (),
				 f->scheduling_strategy ()) == -1)
    // @@ CJC Need to return an error somehow!!  Maybe set do_exit?
    return -1;

  if (this->peer_acceptor_.acceptor ().get_local_addr (this->addr_) == -1)
    return -1;

  return 0;
}

TAO_Client_Strategy_Factory *
CORBA_ORB::client_factory (void)
{
  if (this->client_factory_ == 0)
    {
      // Look in the service repository for an instance.
      this->client_factory_ =
        ACE_Dynamic_Service<TAO_Client_Strategy_Factory>::instance ("Client_Strategy_Factory");
      this->client_factory_from_service_config_ = CORBA::B_TRUE;
    }

  if (this->client_factory_ == 0)
    {
      // Still don't have one, so let's allocate the default.  This
      // will throw an exception if it fails on exception-throwing
      // platforms.  
      ACE_NEW_RETURN (this->client_factory_, TAO_Default_Client_Strategy_Factory, 0);

      this->client_factory_from_service_config_ = CORBA::B_FALSE;
      // @@ At this point we need to register this with the
      // Service_Repository in order to get it cleaned up properly.
      // But, for now we let it leak.
    }
  return this->client_factory_;
}

TAO_Server_Strategy_Factory *
CORBA_ORB::server_factory (void)
{
  if (this->server_factory_ == 0)
    {
      // Look in the service repository for an instance.
      this->server_factory_ =
        ACE_Dynamic_Service<TAO_Server_Strategy_Factory>::instance ("Server_Strategy_Factory");

      this->server_factory_from_service_config_ = CORBA::B_TRUE;
    }

  if (this->server_factory_ == 0)
    {
      // Still don't have one, so let's allocate the default.  This
      // will throw an exception if it fails on exception-throwing
      // platforms.
      ACE_NEW_RETURN (this->server_factory_, TAO_Default_Server_Strategy_Factory, 0);

      this->server_factory_from_service_config_ = CORBA::B_FALSE;
      // @@ At this point we need to register this with the
      // Service_Repository in order to get it cleaned up properly.
      // But, for now we let it leak.
    }
  return this->server_factory_;
}

ULONG __stdcall
CORBA_ORB::Release (void)
{
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, mon, this->lock_, 0));

    ACE_ASSERT (this != 0);

    if (--refcount_ != 0)
      return this->refcount_;
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

// Little convenience function use in parsing arguments
inline static void
argvec_shift (int& argc, char *const *argv, int numslots)
{
  ACE_OS::memmove ((void *) &argv[0],
		   (void *) &argv[numslots],
		   (argc - numslots - 1) * sizeof argv[0]);
  argc -= numslots;
}
  
CORBA::ORB_ptr
CORBA::ORB_init (int &argc,
		 char *const *argv,
		 char *orb_name,
		 CORBA::Environment &env)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, TAO_Internal::orbinit_lock_, 0);

  env.clear ();

  // Verify some of the basic implementation requirements.  This test
  // gets optimized away by a decent compiler (or else the rest of the
  // routine does).
  //
  // NOTE:  we still "just" assume that native floating point is IEEE.

  if (sizeof (CORBA::Short) != 2
      || sizeof (CORBA::Long) != 4
      || sizeof (CORBA::LongLong) != 8
      || sizeof (CORBA::Float) != 4
      || sizeof (CORBA::Double) != 8
      || sizeof (CORBA::LongDouble) != 16
      || sizeof (CORBA::WChar) < 2
      || sizeof (void *) != SIZEOF_VOID_P)
    {
      env.exception (new CORBA::INITIALIZE (CORBA::COMPLETED_NO));
      return 0;
    }

  // Parse arguments to the ORB.  Typically the ORB is passed
  // arguments straight from the command line, so we will simply pass
  // through them and respond to the ones we understand and ignore
  // those we don't.
  //
  // In some instances, we may actually build another vector of
  // arguments and stash it for use initializing other components such
  // as the ACE_Service_Config or the RootPOA.
  //
  // @@ Should we consume arguments we understand or leave all
  // arguments in the vector?

  // Prepare a copy of the argument vector
  // XXXASG - compiler doesn't like this
  char **svc_config_argv; // @@ Should this be a data member?
  // Probably, but there's no object in which to scope it.

  int svc_config_argc = 0;
  ACE_NEW_RETURN (svc_config_argv, char *[argc + 1], 0);

  // Be certain to copy the program name.
  // @@ I'm not sure that this convention makes sense.  Perhaps we
  // @@ should use the ORB's name in place of argv[0]?
  svc_config_argv[svc_config_argc++] = argv[0];

  CORBA::String_var host = CORBA::string_dup ("");
  CORBA::UShort port = 5001;  // some default port -- needs to be a #defined value
  for (int i = 1; i < argc; )
    {
      if (ACE_OS::strcmp (argv[i], "-ORBsvcconf") == 0)
        {
          // Specify the name of the svc.conf file to be used
          svc_config_argv[svc_config_argc++] = "-f";
          
	  if (i + 1 < argc)
            // @@ Should we dup the string before assigning?
	    svc_config_argv[svc_config_argc++] = argv[i + 1];

          argvec_shift (argc, &argv[i], 2);
        }
      else if (ACE_OS::strcmp (argv[i], "-ORBdaemon") == 0)
        {
          // Be a daemon
          svc_config_argv[svc_config_argc++] = "-b";

          argvec_shift (argc, &argv[i], 1);
        }
      else if (ACE_OS::strcmp (argv[i], "-ORBdebug") == 0)
        {
          // Turn on debugging
          svc_config_argv[svc_config_argc++] = "-d";
          argvec_shift (argc, &argv[i], 1);
        }
      else if (ACE_OS::strcmp (argv[i], "-ORBhost") == 0)
	{
          // Specify the name of the host (i.e., interface) on which
          // the server should listen
	  if (i + 1 < argc)
	    host = CORBA::string_dup (argv[i + 1]);

          argvec_shift (argc, &argv[i], 2);
	}
      else if (ACE_OS::strcmp (argv[i], "-ORBport") == 0)
	{
          // Specify the port number/name on which we should listen
	  if (i + 1 < argc)
            // @@ We shouldn't limit this to being specified as an int! --cjc
	    port = ACE_OS::atoi (argv[i + 1]);

          argvec_shift (argc, &argv[i], 2);
	}
      else if (ACE_OS::strcmp (argv[i], "-ORBrcvsock") == 0)
	{
          // Specify the size of the socket's receive buffer
	}
      else if (ACE_OS::strcmp (argv[i], "-ORBsndsock") == 0)
	{
          // Specify the size of the socket's send buffer
	}
      else
        {
          i++;
        }
    }

#if defined (DEBUG)
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

  ACE_INET_Addr rendezvous;
  // @@ Don't use magic #'s like 128.  Final an appropriate symbolic
  // constant.  
  char hbuf[128];

  // Create a INET_Addr.
  if (ACE_OS::strlen (host) == 0)
    {
      // hostname not provided, so use the default
      if (ACE_OS::hostname (hbuf, sizeof(hbuf)-1) == -1)
        ACE_DEBUG ((LM_ERROR, "(%P|%t) %p, unable to obtain host name\n"));

      CORBA::String_var h = CORBA::string_dup(hbuf);
      host = h;
    }

  // The conditional catches errors in hbuf
  if (ACE_OS::strlen (host) > 0)
    rendezvous.set (port, host);
  else
    rendezvous.set (port);
  
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
  CORBA::Boolean use_ior;

  if (orb_name != 0 && ACE_OS::strcmp (orb_name, "internet") == 0)
    use_ior = CORBA::B_FALSE;
  else
    use_ior = CORBA::B_TRUE;

#if defined (SIGPIPE)
  // @@ Is there a better way to deal with this in a portable manner? --cjc
  //
  // Impractical to have each call to the ORB protect against the
  // implementation artifact of potential writes to dead connections,
  // as it'd be way expensive.  Do it here; who cares about SIGPIPE in
  // these kinds of applications, anyway?
  (void) ACE_OS::signal (SIGPIPE, SIG_IGN);
#endif /* SIGPIPE */

  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  // Call various internal initialization routines.
  // @@ Why are these names prefixed with "__"?  Shouldn't they be in
  // a class someplace, or at least have the word "TAO" in front of
  // them? 
  __TC_init_table ();
  TAO_Marshal::initialize ();
  __TC_init_standard_exceptions (env);
  
  if (env.exception () != 0)
    return 0;

  // Initialize the Service Configurator
  TAO_Internal::open_services (svc_config_argc, svc_config_argv);

  // Inititalize the "ORB" pseudo-object now.
  IIOP_ORB_ptr this_orb = 0;
  ACE_NEW_RETURN (this_orb, IIOP_ORB, 0);

  // Install the ORB * into the ORB Core instance.  Note that if we're
  // running with a "thread-per-rate" concurrency model this ORB *
  // will be located in thread-specific storage.
  TAO_ORB_Core_instance ()->orb (this_orb);

  // @@ Seems like the following should happen inside the ORB Core,
  // not at this level.  Do we really need this stuff?  What is the
  // alternative format (other than IOR)?  --cjc
  this_orb->use_omg_ior_format (CORBA::Boolean (use_ior));
  this_orb->params ()->addr (rendezvous);
  
  return this_orb;
}

void
CORBA_ORB::create_list (CORBA::Long count,
                        CORBA::NVList_ptr &retval)
{
  assert (CORBA::ULong (count) <= UINT_MAX);

  retval = new CORBA::NVList;

  if (count != 0) 
    {
      retval->_len = 0;
      retval->_max = (u_int) count;
      retval->_values = (CORBA::NamedValue_ptr) ACE_OS::calloc ((u_int) count,
								sizeof (CORBA::NamedValue));
    }
}

// This is a server-side internal routine; it's not available to any
// portable code except method code, which moreover may not access the
// state variable directly since its implemention may differ between
// ORBs.
//
// XXX it's server-side so should be OA-specific and not in this module

#if 0
CORBA::ORB_ptr
_orb (void)
{
  return TAO_ORB::instance ();
}
#endif

CORBA::BOA_ptr 
CORBA_ORB::BOA_init (int &argc,
		     char **argv,
		     const char *boa_identifier)
{
  // parse the arguments looking for options starting with -OA. After
  // processing these options, move all these to the end of the argv
  // list and decrement argc appropriately.

  TAO_ORB_Core *oc = TAO_ORB_Core_instance();
  TAO_OA_Parameters *params = TAO_ORB_Core_instance()->oa_params();
  CORBA::BOA_ptr rp;
  CORBA::String_var id = boa_identifier;
  CORBA::String_var demux = CORBA::string_dup ("dynamic_hash"); // default, at least for now
  CORBA::ULong tablesize = 0; // default table size for lookup tables
  CORBA::Boolean use_threads = CORBA::B_FALSE;
  CORBA::Environment env;

  for (int i = 0; i < argc; )
    {
      // @@ Can you please add comments describing each of these options? --doug
      // @@ Andy, could you review these since you wrote the code --cjc
      
      if (ACE_OS::strcmp (argv[i], "-OAid") == 0)
        {
          // Specify the name of the OA
	  if (i + 1 < argc)
	    id = CORBA::string_dup (argv[i + 1]);

          argvec_shift (argc, &argv[i], 2);
        }
      else if (ACE_OS::strcmp (argv[i], "-OAobjdemux") == 0)
	{
          // Specify the demultiplexing strategy to be used for object
          // demultiplexing
	  if (i + 1 < argc)
	    demux = CORBA::string_dup (argv[i+1]);

          argvec_shift (argc, &argv[i], 2);
	}
      else if (ACE_OS::strcmp (argv[i], "-OAtablesize") == 0)
	{
          // @@ Specify the table size used for ????
	  if (i + 1 < argc)
	    tablesize = ACE_OS::atoi (argv[i+1]);

          argvec_shift (argc, &argv[i], 2);
	}
      else if (ACE_OS::strcmp (argv[i], "-OAthread") == 0)
	{
          // Specify whether or not threads should be used.
	  use_threads = CORBA::B_TRUE;
          argvec_shift (argc, &argv[i], 1);
	}
      else
	i++;
    }
  
  //    ACE_MT (ACE_GUARD (ACE_Thread_Mutex, roa_mon, lock_));

  if (oc->root_poa ())
    {
      env.exception (new CORBA::INITIALIZE (CORBA::COMPLETED_NO));
      return 0;
    }

  // set all parameters
#if 0
  // subsumed by abstract factory impls?
  params->using_threads (use_threads);
  params->demux_strategy (demux);
  // Moved to different place (on the ORB now)
  params->addr (rendezvous);
  // Doesn't exist any longer
  params->upcall (CORBA::BOA::dispatch);
  params->tablesize (tablesize);
#endif /* 0 */

#if defined (ROA_NEEDS_REQ_KEY)
  (void) ACE_Thread::keycreate (&req_key_);
#endif /* ROA_NEEDS_REQ_KEY */
    
  ACE_NEW_RETURN (rp, ROA (this, env), 0);
  oc->root_poa (rp);

  return rp;
}

int
CORBA_ORB::perform_work (ACE_Time_Value *tv)
{
  return TAO_ORB_Core_instance ()->reactor ()->handle_events (tv);
}

int
CORBA_ORB::run (ACE_Time_Value *tv)
{
  ACE_Reactor *r = TAO_ORB_Core_instance ()->reactor ();

  // This method should only be called by servers, so now we set up
  // for listening!
  if (this->open () == -1)
    return -1;
  
  // Loop "forever" handling client requests.

  for (;;)
    switch (r->handle_events (tv))
      {
      case 0: // Timed out, so we return to caller.
	return 0;
	/* NOTREACHED */
      case -1: // Something else has gone wrong, so return to caller. 
	return -1;
	/* NOTREACHED */
      default: // Some handlers were dispatched, so keep on processing
	       // requests until we're told to shutdown .
#if 0
	// @@ How can we figure out how to get the right POA pointer
	// to shutdown the ORB here?
	if (poa->shutting_down ())
	  return 1;
#endif /* 0 */
	break;
	/* NOTREACHED */
      }

  /* NOTREACHED */
  return 0;
}

#define TAO_HASH_ADDR ACE_Hash_Addr<ACE_INET_Addr, TAO_Client_Connection_Handler>
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Dynamic_Service<TAO_Server_Strategy_Factory>;
template class ACE_Dynamic_Service<TAO_Client_Strategy_Factory>;
template class ACE_Strategy_Acceptor<TAO_OA_Connection_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_RW_MUTEX>;
template class ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Creation_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR>;
//template class TAO_HASH_ADDR;
template class ACE_Hash_Map_Entry<TAO_HASH_ADDR, TAO_Client_Connection_Handler *>;
template class ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Dynamic_Service<TAO_Server_Strategy_Factory>
#pragma instantiate ACE_Dynamic_Service<TAO_Client_Strategy_Factory>
#pragma instantiate ACE_Strategy_Acceptor<TAO_OA_Connection_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Creation_Strategy<TAO_Client_Connection_Handler>
#pragma instantiate ACE_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR>
//#pragma instantiate TAO_HASH_ADDR
#pragma instantiate ACE_Hash_Map_Entry<TAO_HASH_ADDR, TAO_Client_Connection_Handler *>
#pragma instantiate ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
