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

#include "tao/corba.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Repository.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "tao/tao_internals.h"
#include "tao/Timeprobe.h"

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
    should_shutdown_(CORBA::B_FALSE),
    name_service_ (CORBA_Object::_nil ()),
    schedule_service_ (CORBA_Object::_nil ()),
    event_service_ (CORBA_Object::_nil ())
{
}

CORBA_ORB::~CORBA_ORB (void)
{
  TAO_ORB_Core_instance ()->fini ();

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

  TAO_ORB_Core *ocp = TAO_ORB_Core_instance ();
  TAO_Server_Strategy_Factory *f = ocp->server_factory ();

  // Initialize the endpoint ... or try!

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

  ocp->orb_params ()->addr (ocp->addr ());

  return 0;
}

ULONG
CORBA_ORB::Release (void)
{
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, mon, this->lock_, 0));

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
		 const char * /* orb_name */,
		 CORBA::Environment &env)
{
  // Using ACE_Static_Object_Lock::instance() precludes ORB_init from being called
  // within a static object CTOR.
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard,
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
      ACE_DEBUG ((LM_DEBUG, "s:%d l:%d ll:%d f:%d d:%d ld:%d wc:%d v:%d\n",
		  sizeof (CORBA::Short),
		  sizeof (CORBA::Long),
		  sizeof (CORBA::LongLong),
		  sizeof (CORBA::Float),
		  sizeof (CORBA::Double),
		  sizeof (CORBA::LongDouble),
		  sizeof (CORBA::WChar),
		  sizeof (void *)));

      env.exception (new CORBA::INITIALIZE (CORBA::COMPLETED_NO));
      return 0;
    }

  TAO_ORB_Core_instance ()->init (argc, (char **)argv);

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

  // create an empty list
  retval = new CORBA::NVList;

  // if count is greater than 0, create a list of NamedValues
  if (count != 0)
    {
      retval->len_ = 0;
      retval->max_ = (CORBA::ULong) count;
      retval->values_ = (CORBA::NamedValue_ptr) ACE_OS::calloc ((u_int) count,
								sizeof (CORBA::NamedValue));
      for (CORBA::Long i=0; i < count; i++)
        {
          // initilaize the NamedValue
          (void) new (&retval->values_[i]) CORBA::NamedValue;
        }
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
      else
	i++;
    }

  if (oc->root_poa ())
    {
      env.exception (new CORBA::INITIALIZE (CORBA::COMPLETED_NO));
      return 0;
    }

#if defined (POA_NEEDS_REQ_KEY)
  (void) ACE_Thread::keycreate (&req_key_);
#endif /* POA_NEEDS_REQ_KEY */

  ACE_NEW_RETURN (rp, CORBA::POA (this, env), 0);

  return rp;
}

int
CORBA_ORB::perform_work (ACE_Time_Value *tv)
{
  ACE_Reactor *r = TAO_ORB_Core_instance ()->reactor ();
  
  // Set the owning thread of the Reactor to the one which we're
  // currently in.  This is necessary b/c it's possible that the
  // application is calling us from a thread other than that in which
  // the Reactor's CTOR (which sets the owner) was called.
  r->owner (ACE_Thread::self ());

  return r->handle_events (tv);
}

int
CORBA_ORB::run (ACE_Time_Value *tv)
{
  ACE_Reactor *r = TAO_ORB_Core_instance ()->reactor ();

  // Set the owning thread of the Reactor to the one which we're
  // currently in.  This is necessary b/c it's possible that the
  // application is calling us from a thread other than that in which
  // the Reactor's CTOR (which sets the owner) was called.
  r->owner (ACE_Thread::self ());

  // This method should only be called by servers, so now we set up
  // for listening!
  if (this->open () == -1)
    return -1;

  // Loop "forever" handling client requests.

  while (this->should_shutdown_ == 0)
    {
      ACE_TIMEPROBE ("  -> CORBA_ORB::run handling events");
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
	  ACE_TIMEPROBE ("  -> CORBA_ORB::run events handled");
	  break;
	  /* NOTREACHED */
	}
    }
  /* NOTREACHED */
  return 0;
}

CORBA_Object_ptr
CORBA_ORB::resolve_poa (void)
{
  // Need to do double-checked locking here to cover the case of
  // multiple threads using a global resource policy.
  if (TAO_ORB_Core_instance->root_poa() == 0)
    {
      // Construct a new POA
      // Irfan fill this in properly
      POA* newpoa = new POA ();

      // set the poa in the orbcore instance
      TAO_ORB_Core_instance->root_poa (newpoa);
    }
  
  return TAO_ORB_Core_instance()->root_poa();
}

CORBA_Object_ptr
CORBA_ORB::resolve_name_service (void)
{
  CORBA::Environment env;

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
      this->name_service_ =
        this->string_to_object (name_service_ior, env);

      // check for errors
      if (env.exception () != 0)
	this->name_service_ = CORBA_Object::_nil ();

      // Return ior.
      return CORBA_Object::_duplicate (this->name_service_);
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
        port = TAO_DEFAULT_NAME_SERVER_REQUEST_PORT;

      // This is the code that implements the multicast
      // Naming Service locator.
      ACE_SOCK_Dgram_Mcast multicast;

      ACE_INET_Addr remote_addr;
      // This starts out initialized to all zeros!
      ACE_INET_Addr multicast_addr (port,
                                    ACE_DEFAULT_MULTICAST_ADDR);

      // Subscribe to multicast address.
      if (multicast.subscribe (multicast_addr) == -1)
	return CORBA_Object::_nil ();

      // Prepare connection for the reply.
      ACE_INET_Addr response_addr;
      ACE_SOCK_Dgram response;

      // Choose any local port, we don't really care.
      if (response.open (ACE_Addr::sap_any) == -1)
        {
          ACE_ERROR ((LM_ERROR, "open failed.\n"));
          return CORBA_Object::_nil ();
        }

      if (response.get_local_addr (response_addr) == -1)
        {
          ACE_ERROR ((LM_ERROR, "get_local_addr failed.\n"));
          return CORBA_Object::_nil ();
        }

      // Figure out what port to listen on for server replies,
      // and convert to network byte order.
      CORBA::Short reply_port = htons (response_addr.get_port_number ());

      // Send multicast of one byte, enough to wake up server.
      ssize_t n_bytes = multicast.send ((char *) &reply_port,
                                        sizeof reply_port);

      // check for errors
      if (n_bytes == -1)
	return CORBA_Object::_nil ();

      ACE_DEBUG ((LM_DEBUG,
		  "Sent multicast.  Reply port is %u.  # of bytes sent is %d.\n",
		  response_addr.get_port_number (),
		  n_bytes));

      char buf[BUFSIZ];
      // Wait for response until TAO_DEFAULT_NAME_SERVER_TIMEOUT.
      ACE_Time_Value timeout (TAO_DEFAULT_NAME_SERVER_TIMEOUT);

      // receive response message
      n_bytes = response.recv (buf,
			       BUFSIZ,
			       remote_addr,
			       0,
			       &timeout);

      // Close endpoint for response.
      int retval = response.close ();

      // check for errors
      if (n_bytes == -1 || retval == -1)
        return CORBA_Object::_nil ();

      // null terminate message
      buf[n_bytes] = 0;

      ACE_DEBUG ((LM_DEBUG,
                  "Naming service resolved to ior: '%s'\n",
                  buf));

      // convert ior to an object reference
      this->name_service_ =
	this->string_to_object ((CORBA::String) buf, env);

      // check for errors
      if (env.exception () != 0)
	this->name_service_ = CORBA_Object::_nil ();

      // Return ior.
      return CORBA_Object::_duplicate (this->name_service_);
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

// Create an objref

CORBA::Object_ptr
CORBA_ORB::key_to_object (const TAO::ObjectKey &key,
                          const char *type_id,
                          CORBA::Environment &env)
{
  CORBA::String id;
  IIOP_Object *data;

  if (type_id)
    id = CORBA::string_copy (type_id);
  else
    id = 0;

  // @@ (IRFAN) This is the most likely chunk of code to break because
  // of this evil cast.  Unfortunately, the generated code for
  // sequences doesn't give access to the underlying buffer, so I
  // don't have a way to construct a new OctetSeq instance in the
  // appropriate manner.  Fortunately, in order for ObjectKey to be
  // useable internally, we need the same capabilities, and so right
  // around the time that this conversion could be done properly it
  // won't have to be done at all.
  // @@ I (coryan@cs) modified the ORB core to use
  // PortableServer::ObjectId instead of CORBA::OctetSeq as object
  // identifiers, if this prove to be wrong I'll take it back.
  // CORBA::OctetSeq *internal_key = (CORBA::OctetSeq *)key;
  data = new IIOP_Object (id,
                          IIOP::Profile (TAO_ORB_Core_instance ()->orb_params ()->addr (),
                                         key));
  if (data != 0)
    env.clear ();
  else
    {
      env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
      return 0;
    }

  // Return the CORBA::Object_ptr interface to this objref.
  CORBA::Object_ptr new_obj;

  if (data->QueryInterface (IID_CORBA_Object,
			    (void **) &new_obj) != TAO_NOERROR)
    env.exception (new CORBA::INTERNAL (CORBA::COMPLETED_NO));

  data->Release ();
  return new_obj;
}


#define TAO_HASH_ADDR ACE_Hash_Addr<ACE_INET_Addr>
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Dynamic_Service<TAO_Server_Strategy_Factory>;
template class ACE_Dynamic_Service<TAO_Client_Strategy_Factory>;
template class ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_RW_MUTEX>;
template class ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Atomic_Op<ACE_SYNCH_MUTEX, CORBA::Boolean>;
template class ACE_Hash_Map_Entry<TAO_HASH_ADDR, TAO_Client_Connection_Handler *>;
template class ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator_Base<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator_Base<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Dynamic_Service<TAO_Server_Strategy_Factory>
#pragma instantiate ACE_Dynamic_Service<TAO_Client_Strategy_Factory>
#pragma instantiate ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Atomic_Op<ACE_SYNCH_MUTEX, CORBA::Boolean>
//#pragma instantiate TAO_HASH_ADDR
#pragma instantiate ACE_Hash_Map_Entry<TAO_HASH_ADDR, TAO_Client_Connection_Handler *>
#pragma instantiate ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
