// $Id$

// @@ as with TAO, this has a strong coupling to the Internet ORB
// (IIOP) code.  We should make it know less about that protocol
// component and have a loose table-driven coupling to ORB/protocol
// library components.

#include "tao/corba.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Repository.h"
#include "ace/Object_Manager.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "tao/TAO_Internal.h"

#if !defined (__ACE_INLINE__)
# include "tao/ORB.i"
#endif /* ! __ACE_INLINE__ */

#include "tao/Timeprobe.h"

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_ORB_Timeprobe_Description[] =
{
  "CORBA_ORB::run - start",
  "CORBA_ORB::run - end",
};

enum
{
  // Timeprobe description table start key 
  TAO_CORBA_ORB_RUN_START = 0,
  TAO_CORBA_ORB_RUN_END
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_ORB_Timeprobe_Description,
                                  TAO_CORBA_ORB_RUN_START);

#endif /* ACE_ENABLE_TIMEPROBES */

// Static initialization.
int CORBA_ORB::orb_init_count_ = 0;
CORBA::Boolean CORBA::B_FALSE = 0;
CORBA::Boolean CORBA::B_TRUE = 1;

// COM's IUnknown support

// {A201E4C7-F258-11ce-9598-0000C07CA898}
TAO_DEFINE_GUID (IID_STUB_Object,
                 0xa201e4c7, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

CORBA::String_var::String_var (char *p)
  : ptr_ (p)
{
  // NOTE: According to the CORBA spec this string must *not* be
  // copied, but it is non-compliant to use it/release it in the
  // calling code.  argument is consumed. p should never be NULL
}

CORBA::String_var::String_var (const CORBA::String_var& r)
{
  this->ptr_ = CORBA::string_dup (r.ptr_);
}

CORBA::String_var::~String_var (void)
{
  if (this->ptr_ != 0)
    {
      CORBA::string_free (this->ptr_);
      this->ptr_ = 0;
    }
}

TAO_Export CORBA::String
CORBA::string_dup (const CORBA::Char *str)
{
  return CORBA::string_copy (str);
}

CORBA_ORB::CORBA_ORB (void)
  : refcount_ (1),
    open_called_(CORBA::B_FALSE),
    shutdown_lock_ (0),
    should_shutdown_(CORBA::B_FALSE),
    name_service_ (CORBA_Object::_nil ()),
    schedule_service_ (CORBA_Object::_nil ()),
    event_service_ (CORBA_Object::_nil ()),
    trading_service_ (CORBA_Object::_nil ())
{
  this->cond_become_leader_ = 
      new ACE_SYNCH_CONDITION (TAO_ORB_Core_instance ()->leader_follower_lock ());
}

CORBA_ORB::~CORBA_ORB (void)
{
  TAO_ORB_Core_instance ()->fini ();

  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, tao_mon, *ACE_Static_Object_Lock::instance ()));

  CORBA_ORB::orb_init_count_--;

  if (CORBA_ORB::orb_init_count_ == 0)
    {
      // free up all the ORB owned Exceptions
      TAO_Exceptions::fini ();

      // free up all the ORB owned TypeCodes
      TAO_TypeCodes::fini ();
    }

  if (this->shutdown_lock_ != 0)
    {
      delete this->shutdown_lock_;
      this->shutdown_lock_ = 0;
    }

  if (!CORBA::is_nil (this->name_service_))
    CORBA::release (this->name_service_);
  if (!CORBA::is_nil (this->schedule_service_))
    CORBA::release (this->schedule_service_);
  if (!CORBA::is_nil (this->event_service_))
    CORBA::release (this->event_service_);
  if (!CORBA::is_nil (this->trading_service_))
    CORBA::release (this->trading_service_);

  if (this->cond_become_leader_ != 0)
    this->cond_become_leader_;
}

// Set up listening endpoints.

int
CORBA_ORB::open (void)
{
  if (this->open_called_ != CORBA::B_FALSE)
    return 1;

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
    // Need to return an error somehow!!  Maybe set do_exit?
    return -1;

  if (ocp->acceptor ()->acceptor ().get_local_addr (ocp->addr ()) == -1)
    return -1;

  ocp->orb_params ()->addr (ocp->addr ());
  ocp->add_to_collocation_table ();

  return 0;
}

void
CORBA_ORB::shutdown (CORBA::Boolean /* wait_for_completion */)
{
  // NOTE: we play some games with this monitor to release the lock
  // while blocked on I/O.
  if (this->shutdown_lock_ != 0)
    {
      ACE_GUARD (ACE_Lock, monitor, *this->shutdown_lock_);
      this->should_shutdown_ = 1;
    }
  else
      this->should_shutdown_ = 1;

  TAO_ORB_Core_instance ()->reactor ()->wakeup_all_threads ();
  return;
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
      retval->max_ = (CORBA::ULong) count;
      for (CORBA::Long i=0; i < count; i++)
        {
          CORBA::NamedValue_ptr nv = new CORBA::NamedValue;
          retval->values_.enqueue_tail (nv);
        }
    }
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
  ACE_FUNCTION_TIMEPROBE (TAO_CORBA_ORB_RUN_START);

  
  ACE_DEBUG ((LM_DEBUG,
              "CORBA_ORB::run: (%d) Trying to become the leader.\n",
              ACE_Thread::self ()));

  {
    ACE_Guard<ACE_SYNCH_MUTEX> g (TAO_ORB_Core_instance ()->leader_follower_lock ());
    
    while (TAO_ORB_Core_instance ()->leader_available ())
      {
	if (TAO_ORB_Core_instance ()->add_follower (this->cond_become_leader_) == -1)
	  ACE_ERROR ((LM_ERROR,
		      "(%P|%t) ORB::run: Failed to add a follower thread\n"));
	this->cond_become_leader_->wait ();     
      }
    TAO_ORB_Core_instance ()->set_leader_thread ();
  }
  
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) CORBA_ORB::run: is the leader.\n"));
  
  if (this->shutdown_lock_ == 0)
    this->shutdown_lock_ =
      TAO_ORB_Core_instance ()->server_factory ()->create_event_loop_lock ();


  if (this->shutdown_lock_ == 0)
    this->shutdown_lock_ =
      TAO_ORB_Core_instance ()->server_factory ()->create_event_loop_lock ();

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

#if 0
  const int max_iterations = 100;
  int counter = 0;
#endif /* 0 */

  // NOTE: we play some games with this monitor to release the lock
  // while blocked on I/O.
  ACE_GUARD_RETURN (ACE_Lock, monitor, *this->shutdown_lock_, -1);

  int result = 1;
  // 1 to detect that nothing went wrong
  
  // Loop "forever" handling client requests.
  while (this->should_shutdown_ == 0)
    {
      if (monitor.release () == -1)
        return -1;
      
#if 0
      counter++;
      if (counter == max_iterations)
	{
          ACE_TIMEPROBE_PRINT;
          ACE_TIMEPROBE_RESET;
          counter = 0;
	}
      
      ACE_FUNCTION_TIMEPROBE (TAO_CORBA_ORB_RUN_START);
#endif /* 0 */
      
      switch (r->handle_events (tv))
	{
        case 0: // Timed out, so we return to caller.
          result = 0;
          break;
          /* NOTREACHED */
        case -1: // Something else has gone wrong, so return to caller.
          result = -1;
          break;
          /* NOTREACHED */
        default: // Some handlers were dispatched, so keep on processing
	  // requests until we're told to shutdown .
          break;
          /* NOTREACHED */
	}
      if (result == 0 || result == -1)
        break;
      
      if (monitor.acquire () == -1)
        return -1;
    }
  
  if (result != -1)
    {
      if (TAO_ORB_Core_instance ()->unset_leader_wake_up_follower () == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "(%P|%t) ORB::run: Failed to add a follower thread\n"),
			  -1);
      return 0;
      // nothing went wrong
    }
  else
    return result;
}

int
CORBA_ORB::run (const ACE_Time_Value &tv)
{
  return this->run ((ACE_Time_Value *) &tv);
}

CORBA_Object_ptr
CORBA_ORB::resolve_root_poa (const char *adapter_name,
                             TAO_POA_Manager *poa_manager,
                             const TAO_POA_Policies *policies,
                             TAO_Object_Table *active_object_map)
{
  CORBA::Environment env;

  TAO_POA *poa = TAO_ORB_Core_instance ()->root_poa (adapter_name,
                                                     poa_manager,
                                                     policies,
                                                     active_object_map);
  
  if (env.exception () != 0)
    return CORBA_Object::_nil ();

  PortableServer::POA_var result = poa->_this (env);

  if (env.exception () != 0)
    return CORBA_Object::_nil ();
  else
    return result._retn ();
}

CORBA_Object_ptr
CORBA_ORB::resolve_poa_current (void)
{
  // Return the pointer to this thread's POACurrent.

  CORBA::Environment env;

  TAO_POA_Current *poa_current = TAO_ORB_Core_instance ()->poa_current ();
  if (poa_current == 0)
    return CORBA_Object::_nil ();

  PortableServer::Current_var result = poa_current->_this (env);
  if (env.exception () != 0)
    return CORBA_Object::_nil ();
  else
    return result._retn ();
}


CORBA_Object_ptr
CORBA_ORB::resolve_name_service (void)
{
  CORBA::Environment env;
  CORBA_Object_ptr return_value = CORBA_Object::_nil ();

  // First check to see if we've already initialized this.
  if (this->name_service_ != CORBA_Object::_nil ())
    // @@ Someone please double-check this ;-)
    return_value = this->name_service_;

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

    }
  else
    {
      // First, determine if the port was supplied on the command line
      u_short port =
        TAO_ORB_Core_instance ()->orb_params ()->name_service_port ();

      if (port == 0)
        {
          // Look for the port among our environment variables.
          const char *port_number = ACE_OS::getenv ("NameServicePort");

          if (port_number != 0)
            port = ACE_OS::atoi (port_number);
          else
            port = TAO_DEFAULT_NAME_SERVER_REQUEST_PORT;
        }

      this->name_service_ =
        this->multicast_to_service (TAO_SERVICEID_NAMESERVICE, port);
    }

  // Return ior.
  return_value = this->name_service_;
  return CORBA_Object::_duplicate (return_value);
}

CORBA_Object_ptr
CORBA_ORB::resolve_trading_service (void)
{
  CORBA::Environment env;
  CORBA_Object_ptr return_value = CORBA_Object::_nil ();

  // First check to see if we've already initialized this.
  if (this->trading_service_ != CORBA_Object::_nil ())
    // @@ Someone please double-check this ;-)
    return_value = this->trading_service_;
  else
    {
      char *trading_service_ior =
        TAO_ORB_Core_instance ()->orb_params ()->trading_service_ior ();

      // Second, check to see if the user has give us a parameter on
      // the command-line.
      if (trading_service_ior == 0)
        // Third, check to see if the user has an environment variable.
        trading_service_ior = ACE_OS::getenv ("TradingService");

      if (trading_service_ior != 0)
        {
          this->trading_service_ =
            this->string_to_object (trading_service_ior, env);

          // check for errors
          if (env.exception () != 0)
            this->trading_service_ = CORBA_Object::_nil ();
        }
      else
        {
          // First, determine if the port was supplied on the command line
          u_short port =
            TAO_ORB_Core_instance ()->orb_params ()->trading_service_port ();

          if (port == 0)
            {
              // Look for the port among our environment variables.
              const char *port_number = ACE_OS::getenv ("TradingServicePort");

              if (port_number != 0)
                port = ACE_OS::atoi (port_number);
              else
                port = TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT;
            }

          this->trading_service_ =
            this->multicast_to_service (TAO_SERVICEID_TRADINGSERVICE, port);
        }
    }

  return_value = this->trading_service_;
  return CORBA_Object::_duplicate (return_value);
}


CORBA_Object_ptr
CORBA_ORB::multicast_to_service (TAO_Service_ID service_id,
                                 u_short port)
{
  CORBA::Environment env;
  // Use UDP multicast to locate the  service.
  CORBA_Object_ptr return_value = CORBA_Object::_nil ();

  // This is the code that implements the multicast
  // Naming Service locator.
  ACE_SOCK_Dgram_Mcast multicast;
  ACE_INET_Addr remote_addr;
  // This starts out initialized to all zeros!
  ACE_INET_Addr multicast_addr (port,
                                ACE_DEFAULT_MULTICAST_ADDR);

  // Subscribe to multicast address.
  if (multicast.subscribe (multicast_addr) == -1)
    return return_value;

  // Prepare connection for the reply.
  ACE_INET_Addr response_addr;
  ACE_SOCK_Dgram response;

  // Choose any local port, we don't really care.
  if (response.open (ACE_Addr::sap_any) == -1)
    {
      ACE_ERROR ((LM_ERROR, "open failed.\n"));
      return return_value;
    }

  if (response.get_local_addr (response_addr) == -1)
    {
      ACE_ERROR ((LM_ERROR, "get_local_addr failed.\n"));
      return return_value;
    }

  struct
  {
    u_short reply_port;
    CORBA::Short service_id;
  } mcast_info;

  // Figure out what port to listen on for server replies,
  // and convert to network byte order.
  mcast_info.reply_port = htons (response_addr.get_port_number ());
  mcast_info.service_id = htons (service_id);

  // Send multicast of one byte, enough to wake up server.
  ssize_t n_bytes =
    multicast.send (&mcast_info, sizeof (mcast_info));

  ACE_DEBUG ((LM_DEBUG, "sent multicast request."));

  // check for errors
  if (n_bytes == -1)
    return return_value;

  ACE_DEBUG ((LM_DEBUG,
              "%s; Sent multicast.  Reply port is %u.  # of bytes sent is %d.\n",
              __FILE__,
              response_addr.get_port_number (),
              n_bytes));


  // Wait for response until TAO_DEFAULT_NAME_SERVER_TIMEOUT.
  ACE_Time_Value timeout (TAO_DEFAULT_NAME_SERVER_TIMEOUT);

  // receive response message
  char buf[ACE_MAX_DGRAM_SIZE];
  n_bytes = response.recv (buf, BUFSIZ, remote_addr, 0, &timeout);

  // Close endpoint for response.
  int retval = response.close ();

  // check for errors
  if (n_bytes == -1 || retval == -1)
    return return_value;

  // null terminate message
  buf[n_bytes] = 0;

  ACE_DEBUG ((LM_DEBUG,
              "%s; Service resolved to ior: '%s'\n",
              __FILE__,
              buf));

  // convert ior to an object reference
  CORBA_Object_ptr objectified_ior =
    this->string_to_object ((CORBA::String) buf, env);

  // check for errors
  if (env.exception () == 0)
    return_value = objectified_ior;

  // Return ior.
  return return_value;
}

CORBA_Object_ptr
CORBA_ORB::resolve_initial_references (CORBA::String name)
{
  if (ACE_OS::strcmp (name, TAO_OBJID_NAMESERVICE) == 0)
    return this->resolve_name_service ();
  if (ACE_OS::strcmp (name, TAO_OBJID_TRADINGSERVICE) == 0)
    return this->resolve_trading_service ();
  else if (ACE_OS::strcmp (name, TAO_OBJID_ROOTPOA) == 0)
    return this->resolve_root_poa ();
  else if (ACE_OS::strcmp (name, TAO_OBJID_POACURRENT) == 0)
    return this->resolve_poa_current ();
  else
    return CORBA_Object::_nil ();
}

int
CORBA_ORB::preconnect (CORBA::String connections)
{
  return TAO_ORB_Core_instance ()->preconnect (connections);
}


// Create an objref

CORBA::Object_ptr
CORBA_ORB::key_to_object (const TAO_ObjectKey &key,
                          const char *type_id,
                          CORBA::Environment &env)
{
  CORBA::String id;
  IIOP_Object *data;

  if (type_id)
    id = CORBA::string_copy (type_id);
  else
    id = 0;

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

  // Create the CORBA level proxy
  CORBA_Object *new_obj = new CORBA_Object (data);

  // Clean up in case of errors.
  if (new_obj == 0)
    {
      data->Release ();
      env.exception (new CORBA::INTERNAL (CORBA::COMPLETED_NO));
    }

  return new_obj;
}

// String utility support; this can need to be integrated with the
// ORB's own memory allocation subsystem.

CORBA::String
CORBA::string_copy (const CORBA::Char *str)
{
  if (!str)
    return 0;

  CORBA::String retval =
    CORBA::string_alloc (ACE_OS::strlen (str));

  // clear the contents of the allocated string
  ACE_OS::memset(retval, '\0', ACE_OS::strlen (str));

  return ACE_OS::strcpy (retval, str);
}

CORBA::String_var &
CORBA::String_var::operator= (char *p)
{
  if (this->ptr_ != p)
    {
      if (this->ptr_ != 0)
        CORBA::string_free (this->ptr_);
      this->ptr_ = p;
    }
  return *this;
}

CORBA::String_var &
CORBA::String_var::operator= (const char *p)
{
  if (this->ptr_ != 0)
    CORBA::string_free (this->ptr_);

  this->ptr_ = CORBA::string_dup (p);
  return *this;
}

CORBA::String_var &
CORBA::String_var::operator= (const CORBA::String_var& r)
{
  if (this != &r)
    {
      if (this->ptr_ != 0)
        CORBA::string_free (this->ptr_);
      this->ptr_ = CORBA::string_dup (r.ptr_);
    }
  return *this;
}

// Wide Character string utility support; this can need to be
// integrated with the ORB's own memory allocation subsystem.

CORBA::WString
CORBA::wstring_alloc (CORBA::ULong len)
{
  return new CORBA::WChar [(size_t) (len + 1)];
}

CORBA::WString
CORBA::wstring_copy (const CORBA::WChar *const str)
{
  if (!str)
    return 0;

  CORBA::WString retval = CORBA::wstring_alloc (ACE_OS::wslen (str));
  return ACE_OS::wscpy (retval, str);
}

void
CORBA::wstring_free (CORBA::WChar *const str)
{
  delete [] str;
}

void
CORBA_ORB::init_orb_globals (CORBA::Environment &env)
{
  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, tao_mon, *ACE_Static_Object_Lock::instance ()));

  // Put these initializations here so that exceptions are enabled
  // immediately.

  if (CORBA_ORB::orb_init_count_ == 0)
    {
      // initialize the system TypeCodes
      TAO_TypeCodes::init ();
      // initialize the factory for marshaling
      TAO_Marshal::init ();
      // initialize the interpreter
      TAO_IIOP_Interpreter::init ();
      // initialize the system exceptions
      TAO_Exceptions::init (env);
    }
  CORBA_ORB::orb_init_count_++;
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
  // Using ACE_Static_Object_Lock::instance() precludes <ORB_init>
  // from being called within a static object CTOR.
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard,
                            *ACE_Static_Object_Lock::instance (), 0));

  env.clear ();

  // Make sure initialization of TAO globals only occurs once.
  CORBA_ORB::init_orb_globals (env);

  if (env.exception () != 0)
    return 0;

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
      || sizeof (void *) != ACE_SIZEOF_VOID_P)
    {
      ACE_DEBUG ((LM_DEBUG, "%s; ERROR: unexpected basic type size; "
                            "s:%d l:%d ll:%d f:%d d:%d ld:%d wc:%d v:%d\n",
                  __FILE__,
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

  // Initialize the ORB Core instance.
  int result = TAO_ORB_Core_instance ()->init (argc, (char **)argv);

  // check for errors and return 0 if error.
  if (result == -1)
    {
      env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
      return 0;
    }

  return TAO_ORB_Core_instance()->orb ();
}

// *************************************************************
// Inline operators for TAO_opaque encoding and decoding
// *************************************************************

TAO_OutputCDR&
operator<<(TAO_OutputCDR& cdr, const TAO_opaque& x)
{
  CORBA::ULong length = x.length ();
  cdr.write_ulong (length);
#if !defined (TAO_NO_COPY_OCTET_SEQUENCES)
  cdr.write_octet_array (x.get_buffer (), length);
#else
  if (x.mb () == 0)
    cdr.write_octet_array (x.get_buffer (), length);
  else
    cdr.write_octet_array_mb (x.mb ());
#endif /* TAO_NO_COPY_OCTET_SEQUENCES */
  return cdr;
}

TAO_InputCDR&
operator>>(TAO_InputCDR& cdr, TAO_opaque& x)
{
  CORBA::ULong length;
  cdr.read_ulong (length);
#if !defined (TAO_NO_COPY_OCTET_SEQUENCES)
  x.length (length);
  cdr.read_octet_array (x.get_buffer (), length);
#else
  x.replace (length, cdr.start ());
  cdr.skip_bytes (length);
#endif /* TAO_NO_COPY_OCTET_SEQUENCES */
  return cdr;
}

// ****************************************************************

#define TAO_HASH_ADDR ACE_Hash_Addr<ACE_INET_Addr>
#define TAO_RECYCLABLE_ADDR ACE_Recyclable<TAO_HASH_ADDR>
#define TAO_HASH_RECYCLABLE_ADDR ACE_Hash_Recyclable<TAO_HASH_ADDR>

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#if !defined (TAO_NO_COPY_OCTET_SEQUENCES)
template class TAO_Unbounded_Sequence<CORBA::Octet>;
#endif /* defined (TAO_NO_COPY_OCTET_SEQUENCES) */

template class ACE_Dynamic_Service<TAO_Server_Strategy_Factory>;
template class ACE_Dynamic_Service<TAO_Client_Strategy_Factory>;
template class ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR, ACE_SYNCH_MUTEX>;
template class ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Atomic_Op<ACE_SYNCH_MUTEX, u_int>;
template class ACE_Hash_Map_Entry<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *>;
template class ACE_Hash_Map_Manager<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator_Base<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#if !defined (TAO_NO_COPY_OCTET_SEQUENCES)
#pragma instantiate TAO_Unbounded_Sequence<CORBA::Octet>
#endif /* defined (TAO_NO_COPY_OCTET_SEQUENCES) */

#pragma instantiate ACE_Dynamic_Service<TAO_Server_Strategy_Factory>
#pragma instantiate ACE_Dynamic_Service<TAO_Client_Strategy_Factory>
#pragma instantiate ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Atomic_Op<ACE_SYNCH_MUTEX, u_int>
//#pragma instantiate TAO_HASH_RECYCLABLE_ADDR
#pragma instantiate ACE_Hash_Map_Entry<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *>
#pragma instantiate ACE_Hash_Map_Manager<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_HASH_RECYCLABLE_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_NULL_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
