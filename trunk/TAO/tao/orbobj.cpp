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
#include "tao/poa.h"
#include "tao/nvlist.h"
#include "tao/debug.h"
#endif /* 0 */

#include "tao/corba.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Repository.h"
#include "tao/tao_internals.h"

extern void __TC_init_table (void);
extern void __TC_init_standard_exceptions (CORBA::Environment &env);

// COM's IUnknown support

// {A201E4C6-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_ORB,
	     0xa201e4c6, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

// {A201E4C7-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_STUB_Object,
	     0xa201e4c7, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

CORBA_ORB::CORBA_ORB (void)
  : refcount_ (1),
    open_called_(CORBA::B_FALSE),
    client_factory_ (0),
    client_factory_from_service_config_ (CORBA::B_FALSE),
    server_factory_ (0),
    server_factory_from_service_config_ (CORBA::B_FALSE),
    should_shutdown_(CORBA::B_FALSE),
    name_service_ (CORBA_Object::_nil ())
{
}

CORBA_ORB::~CORBA_ORB (void)
{
  TAO_ORB_Core_instance ()->fini ();
  
  if (!this->client_factory_from_service_config_)
    delete client_factory_;

  if (!this->server_factory_from_service_config_)
    delete server_factory_;

  // This assertion isn't valid because our ORB is a singleton
  // assert (refcount_ == 0);
}

// Set up listening endpoints.
int
CORBA_ORB::open (void)
{
  if (this->open_called_ != CORBA::B_FALSE)
    return -1;

  this->open_called_ = CORBA::B_TRUE;
  
  TAO_Server_Strategy_Factory *f = this->server_factory ();

  // Initialize the endpoint ... or try!
  TAO_ORB_Core *ocp = TAO_ORB_Core_instance ();
  
  if (ocp->acceptor ()->open (ocp->orb_params ()->addr (),
                              ocp->reactor(),
                              f->creation_strategy (),
                              f->accept_strategy (),
                              f->concurrency_strategy (),
                              f->scheduling_strategy ()) == -1)
    // @@ CJC Need to return an error somehow!!  Maybe set do_exit?
    return -1;

  if (ocp->acceptor ()->acceptor ().get_local_addr (ocp->addr ()) == -1)
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

CORBA::ORB_ptr
CORBA::ORB_init (int &argc,
		 char *const *argv,
		 char * /* orb_name */,
		 CORBA::Environment &env)
{
  // Using ACE_Static_Object_Lock::instance() precludes ORB_init from being called
  // within a static object CTOR.
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, guard,
                    *ACE_Static_Object_Lock::instance (), 0));

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

  TAO_ORB_Core_instance ()->init (argc, argv);
  
  // Call various internal initialization routines.
  // @@ Why are these names prefixed with "__"?  Shouldn't they be in
  // a class someplace, or at least have the word "TAO" in front of
  // them?
  // 
  // @@ (CJC) Far more important that the name is whether or not it's
  // OK to call these multiple times.  Andy, can you address this?
  __TC_init_table ();
  TAO_Marshal::initialize ();
  __TC_init_standard_exceptions (env);
  
  if (env.exception () != 0)
    return 0;

  return TAO_ORB_Core_instance()->orb();
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

CORBA::POA_ptr 
CORBA_ORB::POA_init (int &argc,
		     char **argv,
		     const char *poa_identifier)
{
  // Parse the arguments looking for options starting with -OA. After
  // processing these options, move all these to the end of the argv
  // list and decrement argc appropriately.

  TAO_ORB_Core *oc = TAO_ORB_Core_instance ();
  CORBA::POA_ptr rp;
  CORBA::String_var id = poa_identifier;
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
          i++;
	  if (i < argc)
            id = CORBA::string_dup (argv[i++]);
        }
      else if (ACE_OS::strcmp (argv[i], "-OAobjdemux") == 0)
	{
          // Specify the demultiplexing strategy to be used for object
          // demultiplexing
          i++;
	  if (i < argc)
	    demux = CORBA::string_dup (argv[i++]);
	}
      else if (ACE_OS::strcmp (argv[i], "-OAtablesize") == 0)
	{
          // Specify the size of the table used for object demultiplexing
          i++;
	  if (i < argc)
	    tablesize = ACE_OS::atoi (argv[i++]);
	}
      else if (ACE_OS::strcmp (argv[i], "-OAthread") == 0)
	{
          // Specify whether or not threads should be used.
	  use_threads = CORBA::B_TRUE;
          i++;
	}
      else
	i++;
    }
  
  if (oc->root_poa ())
    {
      env.exception (new CORBA::INITIALIZE (CORBA::COMPLETED_NO));
      return 0;
    }

#if defined (ROA_NEEDS_REQ_KEY)
  (void) ACE_Thread::keycreate (&req_key_);
#endif /* ROA_NEEDS_REQ_KEY */
    
  ACE_NEW_RETURN (rp, CORBA::POA (this, env), 0);

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

  while (this->should_shutdown_ == 0)
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
	break;
	/* NOTREACHED */
      }

  /* NOTREACHED */
  return 0;
}

CORBA_Object_ptr 
CORBA_ORB::resolve_poa (void)
{
  ACE_NOTSUP_RETURN (CORBA_Object::_nil ());
}

CORBA_Object_ptr 
CORBA_ORB::resolve_name_service (void)
{
  // First check to see if we've already initialized this.
  if (this->name_service_ != CORBA_Object::_nil ())
    // @@ Someone please double-check this ;-)
    return CORBA_Object::_duplicate (this->name_service_);

  char *name_service_ior =
    TAO_ORB_Core_instance ()->orb_params ()->name_service_ior ();

  // Second, check to see if the user has give us a parameter on
  // the command-line.
  if (name_service_ior == 0)
    // Third, check to see if the user has an environment variable.
    name_service_ior = ACE_OS::getenv ("NameService");

  if (name_service_ior != 0)
    {
      CORBA::Environment env;
      this->name_service_ =
        this->string_to_object (name_service_ior, env);
      return this->name_service_;
    }

  // Fourth, use UDP multicast to locate the naming service.
  else
    {
      // First, see if the user has given us a multicast port number
      // for the name service on the command-line;
      u_short port = TAO_ORB_Core_instance ()->orb_params ()->name_service_port ();

      if (port == 0)
        {
          const char *port_number = ACE_OS::getenv ("NameServicePort");

          if (port_number != 0)
            port = ACE_OS::atoi (port_number);
        }

      if (port == 0)
        port = TAO_DEFAULT_NAME_SERVER_PORT;

      // This is where the code must go to implement the multicast
      // Naming Service locator.
      ACE_NOTSUP_RETURN (CORBA_Object::_nil ());
    }
}

CORBA_Object_ptr 
CORBA_ORB::resolve_initial_references (CORBA::String name)
{
  if (ACE_OS::strcmp (name, "NameService") == 0)
    return this->resolve_name_service ();
  else if (ACE_OS::strcmp (name, "RootPOA") == 0)
    return this->resolve_poa ();
  else
    return CORBA_Object::_nil ();
}

#define TAO_HASH_ADDR ACE_Hash_Addr<ACE_INET_Addr>
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Dynamic_Service<TAO_Server_Strategy_Factory>;
template class ACE_Dynamic_Service<TAO_Client_Strategy_Factory>;
template class ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_RW_MUTEX>;
template class ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_NULL_MUTEX>;
//template class TAO_HASH_ADDR;
template class ACE_Hash_Map_Entry<TAO_HASH_ADDR, TAO_Client_Connection_Handler *>;
template class ACE_Hash_Map_Iterator<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Dynamic_Service<TAO_Server_Strategy_Factory>
#pragma instantiate ACE_Dynamic_Service<TAO_Client_Strategy_Factory>
#pragma instantiate ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_NULL_MUTEX>
//#pragma instantiate TAO_HASH_ADDR
#pragma instantiate ACE_Hash_Map_Entry<TAO_HASH_ADDR, TAO_Client_Connection_Handler *>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
