// $Id$

#include "tao/corba.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Repository.h"
#include "ace/Select_Reactor.h"
#include "ace/Env_Value_T.h"
#include "ace/Arg_Shifter.h"
#include "tao/TAO_Internal.h"

#if !defined (__ACE_INLINE__)
# include "tao/ORB_Core.i"
#endif /* ! __ACE_INLINE__ */

typedef ACE_TSS_Singleton<TAO_ORB_Core, ACE_SYNCH_MUTEX>
        TAO_ORB_CORE;

TAO_Collocation_Table_Lock::TAO_Collocation_Table_Lock (void)
{
    this->lock_ =TAO_ORB_Core_instance ()->server_factory ()->create_coltbl_lock ();
  // We don't need to worry about the race condition here because this
  // is called from within the ctor of Hash_Map_Manager which is
  // placed inside a ACE_Singleton.
}

TAO_Collocation_Table_Lock::~TAO_Collocation_Table_Lock (void)
{
  delete this->lock_;
  this->lock_ = 0;
}


TAO_ORB_Core::TAO_ORB_Core (void)
  : reactor_ (0),
    thr_mgr_ (0),
    connector_ (0),
    orb_ (0),
    root_poa_ (0),
    oa_params_ (0),
    orb_params_ (0),
    addr_ (0),
    acceptor_ (0),
    poa_current_ (0),
    resource_factory_ (0),
    resource_factory_from_service_config_ (CORBA::B_FALSE),
    client_factory_ (0),
    client_factory_from_service_config_ (CORBA::B_FALSE),
    server_factory_ (0),
    server_factory_from_service_config_ (CORBA::B_FALSE),
    opt_for_collocation_ (CORBA::B_TRUE),
    preconnections_ (0)
{    
}

ACE_SYNCH_MUTEX  TAO_ORB_Core::leader_follower_lock_; 

ACE_Unbounded_Set<ACE_SYNCH_CONDITION*> TAO_ORB_Core::follower_set_;

int TAO_ORB_Core::leaders_ = 0; 

ACE_thread_t TAO_ORB_Core::leader_thread_ID_ = 0;

TAO_ORB_Core::~TAO_ORB_Core (void)
{
  // This should probably be changed to use the allocator internal to
  // here once that chunk is actually implemented.
  if (preconnections_)
    ACE_OS::free (preconnections_);
}

TAO_Default_Reactor::TAO_Default_Reactor (void)
  : ACE_Reactor (reactor_impl_ = new ACE_Select_Reactor ())
{
}

TAO_Default_Reactor::~TAO_Default_Reactor (void)
{
  delete reactor_impl_;
}

#define quote(x) #x

int
TAO_ORB_Core::init (int& argc, char** argv)
{
  // Right now, this code expects to begin parsing in argv[1] rather
  // than argv[0].  I don't think that's wise.  I think we need to
  // change that convention to argv[0] and let the initializing code
  // make any necessary shifts.
  //
  // Parse arguments to the ORB.  Typically the ORB is passed
  // arguments straight from the command line, so we will simply pass
  // through them and respond to the ones we understand and ignore
  // those we don't.
  //
  // In some instances, we may actually build another vector of
  // arguments and stash it for use initializing other components such
  // as the ACE_Service_Config or the RootPOA.
  //
  // Prepare a copy of the argument vector.

  char **svc_config_argv;

  int svc_config_argc = 0;
  ACE_NEW_RETURN (svc_config_argv, char *[argc + 1], 0);

  // Be certain to copy the program name so that service configurator
  // has something to skip!
  ACE_Arg_Shifter arg_shifter (argc, argv);
  svc_config_argv[svc_config_argc++] = argv[0];

  ACE_Env_Value<int> defport (quote (TAO_DEFAULT_SERVER_PORT),
                              TAO_DEFAULT_SERVER_PORT);
  CORBA::String_var host = CORBA::string_dup ("");
  CORBA::UShort port = defport;
  CORBA::Boolean use_ior = CORBA::B_TRUE;
  int cdr_tradeoff = TAO_DEFAULT_CDR_MEMCPY_TRADEOFF;
  // The following things should be changed to use the ACE_Env_Value<>
  // template sometime.

  // Name Service IOR string.
  char *ns_ior = 0;

  // Name Service port #.
  u_short ns_port = 0;

  // Trading Service IOR string.
  char *ts_ior = 0;

  // Trading Service port #.
  u_short ts_port = 0;

  // Buffer sizes for kernel socket buffers
  size_t rcv_sock_size = 0;
  size_t snd_sock_size = 0;

  char *preconnections = 0;

  // Should debugging be on (1) or off (0)?
  int debugging = 0;

  while (arg_shifter.is_anything_left ())
    {
      char *current_arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (current_arg, "-ORBsvcconf") == 0)
        {
          // Specify the name of the svc.conf file to be used.
          svc_config_argv[svc_config_argc++] =
            CORBA::string_dup ("-f");
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              svc_config_argv[svc_config_argc++] =
                CORBA::string_dup (arg_shifter.get_current ());
              arg_shifter.consume_arg();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBdaemon") == 0)
        {
          // Be a daemon
          svc_config_argv[svc_config_argc++] =
            CORBA::string_dup ("-b");
          arg_shifter.consume_arg ();
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBdebug") == 0)
        {
          // Turn on debugging
          debugging = 1;
          TAO_orbdebug = 1;
          arg_shifter.consume_arg ();
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBhost") == 0)
        {
          // Specify the name of the host (i.e., interface) on which
          // the server should listen.
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next())
            {
              host =
                CORBA::string_dup (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBnameserviceior") == 0)
        {
          // Specify the IOR of the NameService.

          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              ns_ior = CORBA::string_dup (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBnameserviceport") == 0)
        {
          // Specify the port number for the NameService.

          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              ns_port = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBtradingserviceior") == 0)
        {
          // Specify the IOR of the NameService.

          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              ts_ior = CORBA::string_dup (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBtradingserviceport") == 0)
        {
          // Specify the port number for the NameService.

          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              ts_port = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBport") == 0)
        {
          // Specify the port number/name on which we should listen
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              // We really shouldn't limit this to being specified as
              // an int, but oh well for now.
              port = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBrcvsock") == 0)
        {
          arg_shifter.consume_arg ();
          // Specify the size of the socket's receive buffer

          if (arg_shifter.is_parameter_next ())
            {
              rcv_sock_size = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBsndsock") == 0)
        {
          arg_shifter.consume_arg ();
          // Specify the size of the socket's send buffer
          if (arg_shifter.is_parameter_next ())
            {
              snd_sock_size = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBobjrefstyle") == 0)
        {
          // Specifies the style of printed objrefs: URL or IOR
          //
          // BEGIN COMMENTS FROM IIOP-1.4 On Win32, we should be
          // collecting information from the Registry such as what
          // ORBs are configured, specific configuration details like
          // whether they generate IOR or URL style stringified
          // objrefs and which addresses they listen to (e.g. allowing
          // multihomed hosts to implement firewalls), user-meaningful
          // orb names (they will normally indicate domains), and
          // more.
          //
          // On UNIX, we should collect that from some private config
          // file.
          //
          // Instead, this just treats the "internet" ORB name
          // specially and makes it always use URL-style stringified
          // objrefs, where the hostname and TCP port number are
          // explicit (and the whole objref is readable by mortals).
          // BEGIN COMMENTS FROM IIOP-1.4
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "URL") == 0)
                use_ior = CORBA::B_FALSE;

              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBcollocation") == 0)
        // Specify whether we want to optimize against collocation
        // objects.  Valid arguments are: "yes" and "no".  Default is
        // yes.
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char *opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "YES") == 0)
                this->opt_for_collocation_ = CORBA::B_TRUE;
              else if (ACE_OS::strcasecmp (opt, "NO") == 0)
                this->opt_for_collocation_ = CORBA::B_FALSE;

              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBpreconnect") == 0)
        {
          arg_shifter.consume_arg ();
          // Get a string which describes the host/port of connections
          // we want to cache up-front, thus reducing the latency of
          // the first call.  It is specified as a comma-separated
          // list of host:port specifications, and if multiple
          // connections to the same port are desired, they must be
          // specified multiple times.  For example, the following
          // connects to tango:10015 twice, and watusi:10016 once:
          //
          //    -ORBpreconnect tango:10015,tango:10015,watusi:10016
          if (arg_shifter.is_parameter_next ())
            {
              preconnections = arg_shifter.get_current ();
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBCDRtradeoff") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              cdr_tradeoff = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
       else if (ACE_OS::strcmp (current_arg, "-ORBsvcconfdirective") == 0)
         {
           // This is used to pass arguments to the Service
           // Configurator using the "command line" to provide
           // configuration information rather than using a svc.conf
           // file.  Pass the "-S" to the service configurator.
           svc_config_argv[svc_config_argc++] =
             CORBA::string_dup ("-S");
           arg_shifter.consume_arg ();

           if (arg_shifter.is_parameter_next ())
             {
               // Pass the next argument.
               svc_config_argv[svc_config_argc++] =
                 CORBA::string_dup (arg_shifter.get_current ());
               arg_shifter.consume_arg ();
             }
         }
     else
       arg_shifter.ignore_arg ();
   }

   if (debugging == 0)
     svc_config_argv[svc_config_argc++] = CORBA::string_dup ("-d");


#if defined (DEBUG)
  // Make it a little easier to debug programs using this code.
  {
    // This should use ACE_Env_Value<>
    TAO_debug_level = ACE_Env_Value<u_int> ("TAO_ORB_DEBUG", 0);

    char *value = ACE_OS::getenv ("TAO_ORB_DEBUG");

    if (value != 0)
      {
        TAO_debug_level = ACE_OS::atoi (value);
        if (TAO_debug_level <= 0)
          TAO_debug_level = 1;
        dmsg1 ("TAO_debug_level == %d", TAO_debug_level);
      }
  }
#endif  /* DEBUG */

  ACE_INET_Addr rendezvous;

  // No host specified; find it
  if (ACE_OS::strlen (host) == 0)
    {
      char buffer[MAXHOSTNAMELEN + 1];
      if (rendezvous.get_host_name (buffer, sizeof (buffer)) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) TAO_ORB_Core::init failed to resolve local host %p.\n"), -1);

      host = CORBA::string_dup (buffer);
    }

  if (rendezvous.set (port, (char *) host) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) TAO_ORB_Core::init failed to resolve host %s, %p.\n",
                       (char*) host,
                       "reason"),
                      -1);

#if defined (SIGPIPE) && !defined (ACE_LACKS_UNIX_SIGNALS)
  // There's really no way to deal with this in a portable manner, so
  // we just have to suck it up and get preprocessor conditional and
  // ugly.
  //
  // Impractical to have each call to the ORB protect against the
  // implementation artifact of potential writes to dead connections,
  // as it'd be way expensive.  Do it here; who cares about SIGPIPE in
  // these kinds of applications, anyway?
  (void) ACE_OS::signal (SIGPIPE, SIG_IGN);
#endif /* SIGPIPE */

  // Initialize the Service Configurator -check for return values.
  int result = TAO_Internal::open_services (svc_config_argc,
                                            svc_config_argv);
  // Make sure to free up all the dynamically allocated memory.  If we
  // decide we don't need to allocate this stuff dynamically then we
  // can remove this.
  for (int i = 0; i < svc_config_argc; i++)
    CORBA::string_free (svc_config_argv[i]);

  delete [] svc_config_argv;

  // Check for errors returned from <TAO_Internal::open_services>.
  if (result != 0 && errno != ENOENT)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "ORB Core unable to initialize the Service Configurator"),
                      -1);

  // Initialize the pointers to resources in the ORB Core instance,
  // e.g., reactor, connector, etc.  Must do this after we open
  // services because we'll load the factory from there.
  TAO_Resource_Factory *trf = this->resource_factory ();

  if (trf == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "ORB Core unable to find a Resource Factory instance"),
                      -1);

  this->reactor (trf->get_reactor ());
  this->thr_mgr (trf->get_thr_mgr ());
  this->connector (trf->get_connector ());
  this->acceptor (trf->get_acceptor ());

  TAO_Server_Strategy_Factory *ssf = this->server_factory ();

  if (ssf == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "ORB Core unable to find a Server Strategy Factory instance"),
                      -1);

  ssf->open ();

  // Inititalize the "ORB" pseudo-object now.
  IIOP_ORB_ptr this_orb = 0;
  ACE_NEW_RETURN (this_orb, IIOP_ORB, 0);

  // Install the ORB * into the ORB Core instance.  Note that if we're
  // running with a "thread-per-rate" concurrency model this ORB *
  // will be located in thread-specific storage.
  this->orb (this_orb);

  // This should probably move into the ORB Core someday rather then
  // being done at this level.
  this_orb->_use_omg_ior_format (use_ior);
  this_orb->_optimize_collocation_objects (this->opt_for_collocation_);

  // @@ Michael: I don't know if this is the best spot, 
  // we might have to discuss that.
  //this->leader_follower_lock_ptr_ =  this->client_factory ()
  //                                       ->create_leader_follower_lock ();  



  // Set all kinds of orb parameters whose setting needed to be
  // deferred until after the service config entries had been
  // determined.

  this->orb_params ()->addr (rendezvous);
  if (ns_ior)
    this->orb_params ()->name_service_ior (ns_ior);
  if (ns_port != 0)
    this->orb_params ()->name_service_port (ns_port);
  if (ts_ior)
    this->orb_params ()->trading_service_ior (ts_ior);
  if (ts_port != 0)
    this->orb_params ()->trading_service_port (ts_port);
  if (rcv_sock_size != 0)
    this->orb_params ()->sock_rcvbuf_size (rcv_sock_size);
  if (snd_sock_size != 0)
    this->orb_params ()->sock_rcvbuf_size (snd_sock_size);
  if (cdr_tradeoff >= 0)
    this->orb_params ()->cdr_memcpy_tradeoff (cdr_tradeoff);

  // Open the <Strategy_Connector>.
  if (this->connector ()->open (this->reactor(),
                                &this->null_creation_strategy_,
                                &this->caching_connect_strategy_,
#if defined (TAO_HAS_CLIENT_CONCURRENCY)
                                this->concurrency_strategy_ ()
#else
                                &this->null_activation_strategy_
#endif /* TAO_HAS_CLIENT_CONCURRENCY */
                                ) != 0)
    return -1;

  if (preconnections)
    this->preconnect (preconnections);

  // Port not specified: find one for the user
  if (port == 0)
    return this_orb->open ();
  else
    return 0;
}

int
TAO_ORB_Core::preconnect (const char* the_preconnections)
{
  // It would be good to use auto_ptr<> to guard against premature
  // termination and, thus, leaks.
  char *preconnections = ACE_OS::strdup (the_preconnections);

#if 0
  if (preconnections)
    {
      ACE_INET_Addr dest;
      TAO_Client_Connection_Handler *handler;
      ACE_Unbounded_Stack<TAO_Client_Connection_Handler *> handlers;

      char *nextptr = 0;
      char *where = 0;
      for (where = ACE::strsplit_r (preconnections, ",", nextptr);
           where != 0;
           where = ACE::strsplit_r (0, ",", nextptr))
        {
          char *tport = 0;
          char *thost = where;
          char *sep = ACE_OS::strchr (where, ':');

          if (sep)
            {
              *sep = '\0';
              tport = sep + 1;

              dest.set (atoi(tport), thost);

              // Try to establish the connection
              handler = 0;
              if (this->connector ()->connect (handler, dest) == 0)
                {
                  // Save it for later so we can mark it as idle
                  handlers.push (handler);
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "(%P|%t) Unable to preconnect to host '%s', port %d.\n",
                              dest.get_host_name (), dest.get_port_number ()));
                }
            }
          else
            {
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) Yow!  Couldn't find a ':' separator in '%s' spec.\n", where));
            }
        }

      // Walk the stack of handlers and mark each one as idle now.
      handler = 0;
      while (handlers.pop (handler) == 0)
        handler->idle ();

    }
#else
  int successes = 0;
  if (preconnections)
    {
      ACE_INET_Addr dest;
      ACE_Unbounded_Stack<ACE_INET_Addr> dests;

      char *nextptr = 0;
      char *where = 0;
      for (where = ACE::strsplit_r (preconnections, ",", nextptr);
           where != 0;
           where = ACE::strsplit_r (0, ",", nextptr))
        {
          char *tport = 0;
          char *thost = where;
          char *sep = ACE_OS::strchr (where, ':');

          if (sep)
            {
              *sep = '\0';
              tport = sep + 1;

              dest.set (atoi(tport), thost);
              dests.push (dest);
            }
          else
            ACE_ERROR ((LM_ERROR,
                        "(%P|%t) Yow!  Couldn't find a ':' separator in '%s' spec.\n", where));
        }

      // Create an array of addresses from the stack, as well as an
      // array of eventual handlers.
      size_t num_connections = dests.size ();
      ACE_INET_Addr *remote_addrs = 0;
      TAO_Client_Connection_Handler **handlers = 0;
      char *failures = 0;

      ACE_NEW_RETURN (remote_addrs,
                      ACE_INET_Addr[num_connections],
                      -1);
      ACE_NEW_RETURN (handlers,
                      TAO_Client_Connection_Handler*[num_connections],
                      -1);
      ACE_NEW_RETURN (failures,
                      char[num_connections],
                      -1);

      // Fill in the remote address array
      size_t index = 0;
      while (dests.pop (remote_addrs[index]) == 0)
        handlers[index++] = 0;

      // Finally, try to connect.
      this->connector ()->connect_n (num_connections,
                                     handlers,
                                     remote_addrs,
                                     failures);
      // Loop over all the failures and set the handlers that
      // succeeded to idle state.
      for (index = 0; index < num_connections; index++)
        {
          if (! failures[index])
            {
              handlers[index]->idle ();
              successes++;
            }
        }
    }
#endif /* 0 */
  ACE_OS::free (preconnections);

  return successes;
}

int
TAO_ORB_Core::fini (void)
{
  this->connector ()->close ();

  TAO_Internal::close_services ();

  if (!this->resource_factory_from_service_config_)
    delete resource_factory_;

  if (!this->client_factory_from_service_config_)
    delete client_factory_;

  if (!this->server_factory_from_service_config_)
    delete server_factory_;

  return 0;
}

TAO_Resource_Factory *
TAO_ORB_Core::resource_factory (void)
{
  if (this->resource_factory_ == 0)
    {
      // Look in the service repository for an instance.
      this->resource_factory_ =
        ACE_Dynamic_Service<TAO_Resource_Factory>::instance ("Resource_Factory");
      this->resource_factory_from_service_config_ = CORBA::B_TRUE;
    }

  if (this->resource_factory_ == 0)
    {
      // Still don't have one, so let's allocate the default.  This
      // will throw an exception if it fails on exception-throwing
      // platforms.
      ACE_ERROR ((LM_WARNING,
                  "(%P|%t) WARNING - No Resource Factory found in Service Repository."
                  "  Using default instance with GLOBAL resource source specifier.\n"));

      ACE_NEW_RETURN (this->resource_factory_,
                      TAO_Resource_Factory,
                      0);

      this->resource_factory_from_service_config_ =
        CORBA::B_FALSE;
      this->resource_factory_->resource_source (TAO_Resource_Factory::TAO_GLOBAL);

      // At this point we need to register this with the
      // Service_Repository in order to get it cleaned up properly.
      // But, for now we let it leak.
    }
  return this->resource_factory_;
}

TAO_Client_Strategy_Factory *
TAO_ORB_Core::client_factory (void)
{
  if (this->client_factory_ == 0)
    {
      // Look in the service repository for an instance.
      this->client_factory_ =
        ACE_Dynamic_Service<TAO_Client_Strategy_Factory>::instance ("Client_Strategy_Factory");
      this->client_factory_from_service_config_ =
        CORBA::B_TRUE;
    }

  if (this->client_factory_ == 0)
    {
      // Still don't have one, so let's allocate the default.  This
      // will throw an exception if it fails on exception-throwing
      // platforms.
      ACE_ERROR ((LM_WARNING,
                  "(%P|%t) WARNING - No Client Strategy Factory found in Service Repository."
                  "  Using default instance.\n"));

      ACE_NEW_RETURN (this->client_factory_,
                      TAO_Default_Client_Strategy_Factory,
                      0);

      this->client_factory_from_service_config_ = CORBA::B_FALSE;
      // At this point we need to register this with the
      // Service_Repository in order to get it cleaned up properly.
      // But, for now we let it leak.
    }
  return this->client_factory_;
}

TAO_Server_Strategy_Factory *
TAO_ORB_Core::server_factory (void)
{
  if (this->server_factory_ == 0)
    {
      // Look in the service repository for an instance.
      this->server_factory_ =
        ACE_Dynamic_Service<TAO_Server_Strategy_Factory>::instance
          ("Server_Strategy_Factory");
      this->server_factory_from_service_config_ = CORBA::B_TRUE;
    }

  // If the <server_factory_> isn't found it's usually because the ORB
  // hasn't been intialized correctly...
  if (this->server_factory_ == 0)
    {
      // Still don't have one, so let's allocate the default.
      ACE_ERROR ((LM_WARNING,
                  "(%P|%t) WARNING - No %s found in Service Repository."
                  "  Using default instance.\n",
                  "Server Strategy Factory"));

      ACE_NEW_RETURN (this->server_factory_,
                      TAO_Default_Server_Strategy_Factory,
                      0);

      this->server_factory_from_service_config_ = CORBA::B_FALSE;
      // At this point we need to register this with the
      // <Service_Repository> to get it cleaned up properly.  But, for
      // now we let it leak.
    }

  return this->server_factory_;
}

CORBA::ORB_ptr
TAO_ORB_Core::orb (CORBA::ORB_ptr op)
{
  // Shouldn't need to check for ptr validity at this point b/c we
  // already did in ::init()
  this->resource_factory ()->set_orb (op);
  CORBA::ORB_ptr old_orb = this->orb_;
  this->orb_ = op;
  return old_orb;
}

TAO_POA *
TAO_ORB_Core::root_poa (TAO_POA *np)
{
  // Shouldn't need to check for ptr validity at this point b/c
  // we already did in ::init()
  this->resource_factory ()->set_root_poa (np);
  TAO_POA *old_poa = this->root_poa_;
  this->root_poa_ = np;
  return old_poa;
}

int
TAO_ORB_Core::inherit_from_parent_thread (TAO_ORB_Core *p)
{
  // Inherit properties/objects used in ORB_Core from the
  // parent thread.  Stuff inherited here must already exist
  // in the "parent" orbcore.

  this->orb (p->orb ());
  // We'll use the spawning thread's ORB.

  this->root_poa (p->root_poa ());
  // And its root_poa.

  this->orb_params_ = p->orb_params ();
  // We also need its ORB_Params.

  this->oa_params_ = p->oa_params ();
  // And, of course, the POA params.

  this->thr_mgr (p->thr_mgr ());
  // We should use the same thread_manager.

  this->resource_factory_ = p->resource_factory ();
  this->client_factory_ = p->client_factory ();
  this->server_factory_ = p->server_factory ();
  // Inherit the factories.  Notice that they will not be destroyed by
  // this orb_core because *_facotry_from_service_config_'s all default
  // to FALSE.

  this->using_collocation (p->using_collocation ());

  // @@ We shouldn't share the same reactor with the spawning thread.
  //    But what about connector and acceptor????

  return 0;
}

void
TAO_ORB_Core::create_and_set_root_poa (const char *adapter_name,
                                       TAO_POA_Manager *poa_manager,
                                       const TAO_POA_Policies *policies,
                                       TAO_Object_Table *active_object_map)
{
  CORBA::Environment env;
  TAO_POA *poa = 0;
  int delete_policies = 0;

  // Need to do double-checked locking here to cover the case of
  // multiple threads using a global resource policy.
  if (poa_manager == 0)
    poa_manager = new TAO_POA_Manager;

  TAO_POA_Policies *root_poa_policies = 0;
  if (policies == 0)
    {
      root_poa_policies = new TAO_POA_Policies;
      // RootPOA policies defined in spec
      root_poa_policies->implicit_activation (PortableServer::IMPLICIT_ACTIVATION);

      delete_policies = 1;
      policies = root_poa_policies;
    }

  if (active_object_map == 0)
    {
      // Construct a new POA without passing active object map
      poa = new TAO_POA (adapter_name,
                         *poa_manager,
                         *policies,
                         0,
                         env);
    }
  else
    {
      // Construct a new POA passing active object map
      poa = new TAO_POA (adapter_name,
                         *poa_manager,
                         *policies,
                         0,
                         *active_object_map,
                         env);
    }

  if (delete_policies)
    delete root_poa_policies;

  if (env.exception () == 0)
    // set the poa in the orbcore instance
    this->root_poa (poa);
}

int
TAO_ORB_Core::add_to_collocation_table (void)
{
  if (this->using_collocation ())
    {
      TAO_GLOBAL_Collocation_Table *coltbl = this->resource_factory ()->get_global_collocation_table ();
      if (coltbl != 0)
        return coltbl->bind (this->orb_params ()->addr (), this->root_poa ());
    }
  return 0;
}

TAO_POA *
TAO_ORB_Core::get_collocated_poa (ACE_INET_Addr &addr)
{
  if (this->using_collocation ())
    {
      TAO_GLOBAL_Collocation_Table *coltbl = this->resource_factory ()->get_global_collocation_table ();
      if (coltbl != 0)
        {
          TAO_POA *poa;
          if (coltbl->find (addr, poa) == 0)
            return poa;
        }
      else
        {
          if (addr == this->orb_params ()->addr ())
            return this->root_poa ();
        }
    }
  return 0;
}


int 
TAO_ORB_Core::leader_available (void) 
  // returns the value of the flag indicating if a leader
  // is available in the leader-follower model
{ 
  return this->leaders_; 
}

int
TAO_ORB_Core::I_am_the_leader_thread (void)
  // returns 1 if we are the leader thread,
  // else 0
{ 
  if (this->leaders_)
    return (this->leader_thread_ID_ == ACE_Thread::self ()); 
  else
    return 0;
}

void 
TAO_ORB_Core::set_leader_thread (void) 
  // sets the thread ID of the leader thread in the leader-follower
  // model
{ 
  ACE_ASSERT ((this->leaders_ >= 1 && this->leader_thread_ID_ == ACE_Thread::self ())
	      || this->leaders_ == 0);
  this->leaders_++; 
  this->leader_thread_ID_ = ACE_Thread::self (); 
}

int 
TAO_ORB_Core::unset_leader_wake_up_follower (void)
  // sets the leader_available flag to false and tries to wake up a follower
{
  ACE_Guard <ACE_SYNCH_MUTEX> g (TAO_ORB_Core_instance ()->leader_follower_lock ());

  this->unset_leader_thread ();
  
  if (TAO_ORB_Core_instance ()->follower_available () 
      && !this->leader_available ())
    // do it only if a follower is available and no leader is available
    {
      ACE_SYNCH_CONDITION* condition_ptr = this->get_next_follower ();
      if (this->remove_follower (condition_ptr) == -1)
	return -1;
      condition_ptr->signal ();
    }
  return 0;
}


void 
TAO_ORB_Core::unset_leader_thread (void) 
  // sets the flag in the leader-follower model to false
{ 
  ACE_ASSERT ((this->leaders_ > 1 && this->leader_thread_ID_ == ACE_Thread::self ())
              || this->leaders_ == 1);
  this->leaders_--; 
}


ACE_SYNCH_MUTEX &
TAO_ORB_Core::leader_follower_lock (void)
  // returns the leader-follower lock
{ 
  return this->leader_follower_lock_; 
}

int
TAO_ORB_Core::add_follower (ACE_SYNCH_CONDITION *follower_ptr)
  // adds the a follower to the set of followers in the leader-
  // follower model
  // returns 0 on success, -1 on failure
{
  if (this->follower_set_.insert (follower_ptr) != 0)
    return -1;
  return 0;
}

int
TAO_ORB_Core::follower_available (void)
// checks for the availablity of a follower
  // returns 1 on available, 0 else
{
  return !this->follower_set_.is_empty ();
}

int 
TAO_ORB_Core::remove_follower (ACE_SYNCH_CONDITION *follower_ptr)
  // removes a follower from the leader-follower set
  // returns 0 on success, -1 on failure
{
  return this->follower_set_.remove (follower_ptr);
}

ACE_SYNCH_CONDITION* 
TAO_ORB_Core::get_next_follower (void)
  // returns randomly a follower from the leader-follower set
  // returns follower on success, else 0
{
  ACE_Unbounded_Set_Iterator<ACE_SYNCH_CONDITION *> iterator (this->follower_set_);
  if (iterator.first () == 0) 
    // means set is empty
    return 0;
  return *iterator;
}


TAO_Resource_Factory::TAO_Resource_Factory (void)
  : resource_source_ (TAO_GLOBAL),
    poa_source_ (TAO_GLOBAL),
    coltbl_source_ (TAO_GLOBAL)
{
}

TAO_Resource_Factory::~TAO_Resource_Factory (void)
{
}

void
TAO_Resource_Factory::resource_source (int which_source)
{
  resource_source_ = which_source;
}

int
TAO_Resource_Factory::resource_source (void)
{
  return resource_source_;
}

void
TAO_Resource_Factory::poa_source (int which_source)
{
  poa_source_ = which_source;
}

int
TAO_Resource_Factory::poa_source (void)
{
  return poa_source_;
}

int
TAO_Resource_Factory::init (int argc, char *argv[])
{
  return this->parse_args (argc, argv);
}

int
TAO_Resource_Factory::parse_args (int argc, char **argv)
{
  ACE_TRACE ("TAO_Default_Server_Strategy_Factory::parse_args");
  // This table shows the arguments that are parsed with their valid
  // combinations.
  //
  //   ORB      POA    comments
  // +-------+-------+-----------------+
  // | TSS   | TSS   | if ORB==TSS     |
  // |       |       | then POA=TSS    |
  // |       |       | as def.value.   |
  // +-------+-------+-----------------+
  // | TSS   | GLOBAL| ok.             |
  // +-------+-------+-----------------+
  // | GLOBAL| GLOBAL| if ORB==Global  |
  // |       |       | then POA=Global |
  // |       |       | as def.value.   |
  // +-------+-------+-----------------+
  // | GLOBAL| TSS   | *NOT VALID*     |
  // +-------+-------+-----------------+

  int local_poa_source      = -1;
  int local_resource_source = -1;

  for (int curarg = 0; curarg < argc; curarg++)
    if (ACE_OS::strcmp (argv[curarg], "-ORBresources") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "global") == 0)
              local_resource_source = TAO_GLOBAL;
            else if (ACE_OS::strcasecmp (name, "tss") == 0)
              local_resource_source = TAO_TSS;
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBpoa") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "global") == 0)
              local_poa_source = TAO_GLOBAL;
            else if (ACE_OS::strcasecmp (name, "tss") == 0)
              local_poa_source = TAO_TSS;
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBcoltable") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "global") == 0)
              coltbl_source_ = TAO_GLOBAL;
            else if (ACE_OS::strcasecmp (name, "orb") == 0)
              coltbl_source_ = TAO_TSS;
          }
      }

  // Don't allow a global ORB and a tss POA.
  if ( (local_resource_source == TAO_GLOBAL) &&
       (local_poa_source == TAO_TSS) )
    return -1;

  // make poa=tss the default, if ORB is tss and the user didn't
  // specify a value.
  if ( (local_resource_source == TAO_TSS) &&
       (local_poa_source      == -1) )
    local_poa_source = TAO_TSS;

  // update the object data members.
  if (local_resource_source != -1)
    this->resource_source_ = local_resource_source;
  if (local_poa_source != -1)
    this->poa_source_      = local_poa_source;

  // Don't allow a global ORB and a tss POA.
  if ( (this->resource_source_ == TAO_GLOBAL) &&
       (this->poa_source_      == TAO_TSS) )
    return -1;

  return 0;
}

#define IMPLEMENT_PRE_GET_METHOD(methodname,rtype,membername)\
rtype TAO_Resource_Factory::methodname(void)\
{\
  switch (resource_source_)\
    {\
    case TAO_GLOBAL:\
      return &GLOBAL_PRE_ALLOCATED::instance ()->membername;\
    case TAO_TSS:\
      return &TSS_PRE_ALLOCATED::instance ()->membername;\
    }\
  return 0;\
}

#define IMPLEMENT_APP_GET_METHOD(methodname,rtype,membername)\
rtype TAO_Resource_Factory::methodname(void)\
{\
  switch (resource_source_)\
    {\
    case TAO_GLOBAL:\
      return GLOBAL_APP_ALLOCATED::instance ()->membername;\
    case TAO_TSS:\
      return TSS_APP_ALLOCATED::instance ()->membername;\
    }\
  return 0;\
}

IMPLEMENT_PRE_GET_METHOD(get_reactor, ACE_Reactor *, r_)
IMPLEMENT_PRE_GET_METHOD(get_thr_mgr, ACE_Thread_Manager *, tm_)
IMPLEMENT_PRE_GET_METHOD(get_acceptor, TAO_ACCEPTOR *, a_)
IMPLEMENT_PRE_GET_METHOD(get_connector, TAO_CONNECTOR *, c_)
IMPLEMENT_APP_GET_METHOD(get_orb, CORBA_ORB_ptr, orb_)
IMPLEMENT_PRE_GET_METHOD(get_orb_params, TAO_ORB_Parameters *, orbparams_)
IMPLEMENT_PRE_GET_METHOD(get_oa_params, TAO_OA_Parameters *, oaparams_)
IMPLEMENT_PRE_GET_METHOD(get_addr, ACE_INET_Addr *, addr_)

TAO_POA *
TAO_Resource_Factory::get_root_poa (void)
{
  switch (poa_source_)
    {
    case TAO_GLOBAL:
      return GLOBAL_APP_ALLOCATED::instance ()->poa_;
    case TAO_TSS:\
      return TSS_APP_ALLOCATED::instance ()->poa_;
    }
  return 0;
}

void
TAO_Resource_Factory::set_root_poa (TAO_POA *pp)
{
  switch (poa_source_)
    {
    case TAO_GLOBAL:
      GLOBAL_APP_ALLOCATED::instance ()->poa_ = pp;
      break;
    case TAO_TSS:
      TSS_APP_ALLOCATED::instance ()->poa_ = pp;
      break;
    }
}

void
TAO_Resource_Factory::set_orb (CORBA::ORB_ptr op)
{
  switch (resource_source_)
    {
    case TAO_GLOBAL:
      GLOBAL_APP_ALLOCATED::instance ()->orb_ = op;
      break;
    case TAO_TSS:
      TSS_APP_ALLOCATED::instance ()->orb_ = op;
      break;
    }
}

void
TAO_Resource_Factory::set_allocator (ACE_Allocator *alloc)
{
  switch (resource_source_)
    {
    case TAO_GLOBAL:
      GLOBAL_APP_ALLOCATED::instance ()->alloc_ = alloc;
      break;
    case TAO_TSS:
      TSS_APP_ALLOCATED::instance ()->alloc_ = alloc;
      break;
    }
}

ACE_Allocator *
TAO_Resource_Factory::get_allocator (void)
{
  switch (resource_source_)
    {
    case TAO_GLOBAL:
      return GLOBAL_APP_ALLOCATED::instance ()->alloc_;
    case TAO_TSS:
      return TSS_APP_ALLOCATED::instance ()->alloc_;
    }
  return 0;
}

TAO_GLOBAL_Collocation_Table *
TAO_Resource_Factory::get_global_collocation_table (void)
{
  return (coltbl_source_ == TAO_GLOBAL ? GLOBAL_Collocation_Table::instance () : 0);
}

// This function exists because of Win32's proclivity for expanding
// templates at link time.  Since DLLs are just executables, templates
// get expanded and instantiated at link time.  Thus, if there are
// references to the same template in an application AND in a DLL,
// you're screwed.  Using this function, we workaround this by
// insuring that everybody ALWAYS accesses the same instantiation.
//
// There's room for optimizations by making this inline for the ORB
// core and non-inlined elsewhere, but that can be done later--after
// it works.

TAO_Export TAO_ORB_Core *
TAO_ORB_Core_instance (void)
{
  // Depending on whether ACE_HAS_TSS_ORBCORE is defined or not, the
  // instance() method call below either returns a pointer to the
  // process-wide ORB Core Singleton or it returns a pointer to the
  // thread-specific ORB Core Singleton.
  return TAO_ORB_CORE::instance ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
# if defined (__GNUG__)
    template void ACE_Convert (const char *, u_int &);
# endif /* __GNUG__ */

template class ACE_Env_Value<int>;
template class ACE_Env_Value<u_int>;
template class ACE_Strategy_Acceptor<TAO_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>;
template class ACE_Creation_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Connect_Strategy<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR>;
template class ACE_Strategy_Connector<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR>;
template class ACE_NOOP_Creation_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Concurrency_Strategy<TAO_Client_Connection_Handler>;
template class ACE_NOOP_Concurrency_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Recycling_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Connector<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR>;
#if 0
template class ACE_Unbounded_Stack<TAO_Client_Connection_Handler *>;
#else
template class ACE_Unbounded_Stack<ACE_INET_Addr>;
template class ACE_Node<ACE_INET_Addr>;
#endif
template class ACE_Node<TAO_Client_Connection_Handler *>;
template class ACE_TSS_Singleton<TAO_ORB_Core, ACE_SYNCH_MUTEX>;
template class ACE_TSS<TAO_ORB_Core>;
template class ACE_Dynamic_Service<TAO_Resource_Factory>;

template class ACE_Singleton<TAO_Resource_Factory::Pre_Allocated, ACE_SYNCH_MUTEX>;
template class ACE_TSS_Singleton<TAO_Resource_Factory::Pre_Allocated, ACE_SYNCH_MUTEX>;
template class ACE_TSS<TAO_Resource_Factory::Pre_Allocated>;
template class ACE_Singleton<TAO_Resource_Factory::App_Allocated, ACE_SYNCH_MUTEX>;
template class ACE_TSS_Singleton<TAO_Resource_Factory::App_Allocated, ACE_SYNCH_MUTEX>;
template class ACE_TSS<TAO_Resource_Factory::App_Allocated>;
template class ACE_Hash_Map_Manager<ACE_Hash_Addr<ACE_INET_Addr>, TAO_POA *, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Entry<ACE_Hash_Addr<ACE_INET_Addr>, TAO_POA *>;
template class ACE_Hash_Map_Iterator_Base<ACE_Hash_Addr<ACE_INET_Addr>, TAO_POA *, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Iterator<ACE_Hash_Addr<ACE_INET_Addr>, TAO_POA *, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_Hash_Addr<ACE_INET_Addr>, TAO_POA *, TAO_Collocation_Table_Lock>;
template class ACE_Guard<TAO_Collocation_Table_Lock>;
template class ACE_Read_Guard<TAO_Collocation_Table_Lock>;
template class ACE_Write_Guard<TAO_Collocation_Table_Lock>;
template class ACE_Guard<ACE_SYNCH_MUTEX>;
template class ACE_Read_Guard<ACE_SYNCH_MUTEX>;
template class ACE_Write_Guard<ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TAO_GLOBAL_Collocation_Table, ACE_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Env_Value<int>
#pragma instantiate ACE_Env_Value<u_int>
#pragma instantiate ACE_Strategy_Acceptor<TAO_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>
#pragma instantiate ACE_Creation_Strategy<TAO_Client_Connection_Handler>
#pragma instantiate ACE_Connect_Strategy<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR>
#pragma instantiate ACE_Strategy_Connector<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR>
#pragma instantiate ACE_NOOP_Creation_Strategy<TAO_Client_Connection_Handler>
#pragma instantiate ACE_Concurrency_Strategy<TAO_Client_Connection_Handler>
#pragma instantiate ACE_NOOP_Concurrency_Strategy<TAO_Client_Connection_Handler>
#pragma instantiate ACE_Recycling_Strategy<TAO_Client_Connection_Handler>
#pragma instantiate ACE_Connector<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR>
#if 0
#pragma instantiate ACE_Unbounded_Stack<TAO_Client_Connection_Handler *>
#else
#pragma instantiate ACE_Unbounded_Stack<ACE_INET_Addr>
#pragma instantiate ACE_Node<ACE_INET_Addr>
#endif
#pragma instantiate ACE_Node<TAO_Client_Connection_Handler *>
#pragma instantiate ACE_TSS_Singleton<TAO_ORB_Core, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_TSS<TAO_ORB_Core>
#pragma instantiate ACE_Dynamic_Service<TAO_Resource_Factory>

#pragma instantiate ACE_Singleton<TAO_Resource_Factory::Pre_Allocated, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_TSS_Singleton<TAO_Resource_Factory::Pre_Allocated, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_TSS<TAO_Resource_Factory::Pre_Allocated>
#pragma instantiate ACE_Singleton<TAO_Resource_Factory::App_Allocated, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_TSS_Singleton<TAO_Resource_Factory::App_Allocated, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_TSS<TAO_Resource_Factory::App_Allocated>
#pragma instantiate ACE_Hash_Map_Manager<ACE_Hash_Addr<ACE_INET_Addr>, TAO_POA *, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Entry<ACE_Hash_Addr<ACE_INET_Addr>, TAO_POA *>
#pragma instantiate ACE_Hash_Map_Iterator_Base<ACE_Hash_Addr<ACE_INET_Addr>, TAO_POA *, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_Hash_Addr<ACE_INET_Addr>, TAO_POA *, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_Hash_Addr<ACE_INET_Addr>, TAO_POA *, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Guard<TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Read_Guard<TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Write_Guard<TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Guard<ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Read_Guard<ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Write_Guard<ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<TAO_GLOBAL_Collocation_Table, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

ACE_FACTORY_DEFINE (TAO, TAO_Resource_Factory)
