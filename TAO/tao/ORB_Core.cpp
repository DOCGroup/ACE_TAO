// $Id$

#include "tao/ORB_Core.h"

#include "ace/Env_Value_T.h"
#include "ace/Arg_Shifter.h"

#include "tao/TAO_Internal.h"
#include "tao/default_client.h"
#include "tao/default_server.h"
#include "tao/default_resource.h"
#include "tao/debug.h"
#include "tao/IOR_LookupTable.h"

#if !defined (__ACE_INLINE__)
# include "tao/ORB_Core.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, ORB_Core, "$Id$")

typedef ACE_TSS_Singleton<TAO_ORB_Core_TSS_Resources, ACE_SYNCH_MUTEX>
        TAO_ORB_CORE_TSS_RESOURCES;

// ****************************************************************

CORBA_Environment&
CORBA::default_environment ()
{
  // @@ This is a slight violation of layering, we should use
  //    TAO_ORB_Core_instance(), but that breaks during startup.
  return *TAO_ORB_CORE_TSS_RESOURCES::instance ()->default_environment_;
}

// ****************************************************************

TAO_ORB_Core::TAO_ORB_Core (void)
  : thr_mgr_ (0),
    connector_registry_ (0),
    root_poa_ (0),
    orb_params_ (0),
    acceptor_ (0),
    resource_factory_ (0),
    resource_factory_from_service_config_ (0),
    client_factory_ (0),
    client_factory_from_service_config_ (0),
    server_factory_ (0),
    server_factory_from_service_config_ (0),
    opt_for_collocation_ (1),
#if defined (TAO_ARL_USES_SAME_CONNECTOR_PORT)
    arl_same_port_connect_ (0),
#endif /* TAO_ARL_USES_SAME_CONNECTOR_PORT */
    preconnections_ (0)
{
}

TAO_ORB_Core::~TAO_ORB_Core (void)
{
  // This should probably be changed to use the allocator internal to
  // here once that chunk is actually implemented.
  if (preconnections_)
    ACE_OS::free (preconnections_);

  // Allocated in init()
  delete this->orb_params_;
}

int
TAO_ORB_Core::add_to_ior_table (ACE_CString init_ref, TAO_IOR_LookupTable &table)
{
  int index = 0;
  if ((index = init_ref.find ("=")) == ACE_CString::npos)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "Unable to parse -ORBInitRef parameter\n"),
		      -1);

  ACE_CString object_id = init_ref.substr (0,index);
  ACE_CString ior = init_ref.substr (index+1);

  // Add the objectID-IOR to the table and return the status.
  return table.add_ior (object_id, ior);

}

int
TAO_ORB_Core::init (int &argc, char *argv[])
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
  svc_config_argv[svc_config_argc++] = CORBA::string_dup (argv[0]);

  ACE_Env_Value<int> defport ("TAO_DEFAULT_SERVER_PORT",
                              TAO_DEFAULT_SERVER_PORT);
  ACE_CString host;
  CORBA::UShort port = defport;
  CORBA::Boolean use_ior = 1;
  int cdr_tradeoff = ACE_DEFAULT_CDR_MEMCPY_TRADEOFF;

  int giop_lite = 0;

  // The following things should be changed to use the ACE_Env_Value<>
  // template sometime.

  // Name Service IOR string.
  ACE_CString ns_ior;

  // New <ObjectID>:<IOR> mapping that is used by the
  // resolve_initial_references ()
  ACE_CString init_ref;

  // Table for <ObjectID>:<IOR> mapping specified on commandline
  // using ORBInitRef.
  TAO_IOR_LookupTable *ior_lookup_table;

  ACE_NEW_RETURN (ior_lookup_table,
		  TAO_IOR_LookupTable,
		  -1);

  // List of comma separated prefixes from ORBDefaultInitRef.
  ACE_CString default_init_ref;

  // Name Service port #.
  u_short ns_port = 0;

  // Trading Service IOR string.
  ACE_CString ts_ior;

  // Trading Service port #.
  u_short ts_port = 0;

  // Buffer sizes for kernel socket buffers
  size_t rcv_sock_size = 0;
  size_t snd_sock_size = 0;

  char *preconnections = 0;

  // Use dotted decimal addresses
#if defined (TAO_USE_DOTTED_DECIMAL_ADDRESSES)
  int dotted_decimal_addresses = 1;
#else
  int dotted_decimal_addresses = 0;
#endif /* TAO_USE_DOTTED_DECIMAL_ADDRESSES */

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
      else if (ACE_OS::strcmp (current_arg, "-ORBdotteddecimaladdresses") == 0)
        {
          // Use dotted decimal addresses
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              dotted_decimal_addresses = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBdebug") == 0)
        {
          // Turn on debugging
          ACE::debug (1);
          TAO_orbdebug = 1;
          arg_shifter.consume_arg ();
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBdebuglevel") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next())
            {
              TAO_debug_level =
                ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBhost") == 0)
        {
          // Specify the name of the host (i.e., interface) on which
          // the server should listen.
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next())
            {
              host = arg_shifter.get_current ();
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBnameserviceior") == 0)
        {
          // Specify the IOR of the NameService.

          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              ns_ior = arg_shifter.get_current ();
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
              ts_ior = arg_shifter.get_current ();
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
                use_ior = 0;

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
                this->opt_for_collocation_ = 1;
              else if (ACE_OS::strcasecmp (opt, "NO") == 0)
                this->opt_for_collocation_ = 0;

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
      else if (ACE_OS::strcmp (current_arg, "-ORBcdrtradeoff") == 0)
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
#if defined (TAO_ARL_USES_SAME_CONNECTOR_PORT)
      else if (ACE_OS::strcmp (current_arg, "-ORBarlsameportconnect") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              if (ACE_OS::strcasecmp (arg_shifter.get_current (), "yes") == 0)
                this->arl_same_port_connect_ = 1;
              arg_shifter.consume_arg ();
            }
        }
#endif /* TAO_ARL_USES_SAME_CONNECTOR_PORT */
      else if (ACE_OS::strcmp (current_arg, "-ORBgioplite") == 0)
        {
          arg_shifter.consume_arg ();
          giop_lite = 1;
        }

      // A new <ObjectID>:<IOR> mapping has been specified. This will be
      // used by the resolve_initial_references ().

      else if (ACE_OS::strcmp (current_arg, "-ORBInitRef") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              init_ref = arg_shifter.get_current ();
              if (this->add_to_ior_table (init_ref,*ior_lookup_table) != 0)
		ACE_ERROR_RETURN ((LM_ERROR,
				   "Unable to add IOR to the Table\n"),
				  -1);
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-ORBDefaultInitRef") == 0)
	{
	  arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              default_init_ref = arg_shifter.get_current ();
              arg_shifter.consume_arg ();
            }
	}
      else
        arg_shifter.ignore_arg ();
    }

#if defined (DEBUG)
  // Make it a little easier to debug programs using this code.
  {
    TAO_debug_level = ACE_Env_Value<u_int> ("TAO_ORB_DEBUG", 0);

    char *value = ACE_OS::getenv ("TAO_ORB_DEBUG");

    if (value != 0)
      {
        TAO_debug_level = ACE_OS::atoi (value);
        if (TAO_debug_level <= 0)
          TAO_debug_level = 1;
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_debug_level == %d", TAO_debug_level));
      }
  }
#endif  /* DEBUG */

  // Set the endpoint
  ACE_INET_Addr rendezvous;
  if (this->set_endpoint (dotted_decimal_addresses,
                          port,
                          host,
                          rendezvous) == -1)
    return -1;

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

  // Init the connector registry ... this initializes the registry
  // pointer in the ORB core.  The actual registry is either in TSS or global
  // memory.
  this->connector_registry (trf->get_connector_registry ());
  // @@ Make sure the IIOP_Connector is registered with the connector registry.
  this->connector_registry ()->add_connector (trf->get_connector ());

  // @@ Init acceptor ... This needs altering for Pluggable Protocols! fredk
  this->acceptor (trf->get_acceptor ());

  TAO_Server_Strategy_Factory *ssf = this->server_factory ();

  if (ssf == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "ORB Core unable to find a Server Strategy Factory instance"),
                      -1);

  ssf->open (this);

  // Inititalize the "ORB" pseudo-object now.
  ACE_NEW_RETURN (this->orb_, CORBA_ORB (this), 0);

  // This should probably move into the ORB Core someday rather then
  // being done at this level.
  this->orb_->_use_omg_ior_format (use_ior);
  this->orb_->_optimize_collocation_objects (this->opt_for_collocation_);

  // Set the <shutdown_lock_> for the ORB.
  this->orb_->shutdown_lock_ = ssf->create_event_loop_lock ();

  // @@ Michael: I don't know if this is the best spot,
  // we might have to discuss that.
  //this->leader_follower_lock_ptr_ =  this->client_factory ()
  //                                       ->create_leader_follower_lock ();

  // Initialize the container for the ORB parameters.
  if (this->orb_params_ == 0)
    ACE_NEW_RETURN (this->orb_params_, TAO_ORB_Parameters, 0);

  // Set all kinds of orb parameters whose setting needed to be
  // deferred until after the service config entries had been
  // determined.

  this->orb_params ()->addr (rendezvous);
  this->orb_params ()->host (host);

  // Set the init_ref.
  this->orb_params ()->init_ref (init_ref);

  // Set the IOR Table.
  this->orb_params ()->ior_lookup_table (ior_lookup_table);

  // Set the list of prefixes from -ORBDefaultInitRef.
  this->orb_params ()->default_init_ref (default_init_ref);

  this->orb_params ()->name_service_ior (ns_ior);
  this->orb_params ()->name_service_port (ns_port);
  this->orb_params ()->trading_service_ior (ts_ior);
  this->orb_params ()->trading_service_port (ts_port);
  this->orb_params ()->use_dotted_decimal_addresses (dotted_decimal_addresses);
  if (rcv_sock_size != 0)
    this->orb_params ()->sock_rcvbuf_size (rcv_sock_size);
  if (snd_sock_size != 0)
    this->orb_params ()->sock_sndbuf_size (snd_sock_size);
  if (cdr_tradeoff >= 0)
    this->orb_params ()->cdr_memcpy_tradeoff (cdr_tradeoff);

  this->orb_params ()->use_lite_protocol (giop_lite);

  this->orb_params ()->use_dotted_decimal_addresses (dotted_decimal_addresses);

  // tell the registry to open all registered interfaces! fredk
  if (this->connector_registry ()->open (trf, this->reactor ()) != 0)
    return -1;

  // Have registry parse the preconnects
  if (preconnections)
    this->connector_registry ()->preconnect (preconnections);

  return 0;
}


int
TAO_ORB_Core::set_endpoint (int dotted_decimal_addresses,
                            CORBA::UShort port,
                            ACE_CString &host,
                            ACE_INET_Addr &rendezvous)
{
  // No host specified; find it
  if (host.length () == 0)
    {
      char buffer[MAXHOSTNAMELEN + 1];
      if (rendezvous.get_host_name (buffer, sizeof (buffer)) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) failed to resolve local host %p.\n"),
                          -1);
      else
        host = buffer;
    }

  // Set the host and port parameters in the address
  if (rendezvous.set (port, host.c_str ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) failed to resolve host %s, %p.\n",
                       host.c_str (),
                       "reason"),
                      -1);

  // Set up the hostname so that we can put it into object later on.
  // This extra step is necessary since the user specified hostname
  // usually gets expanded to a complete hostname by the conversion.
  // Example: tango -> tango.cs.wustl.edu
  if (dotted_decimal_addresses)
    {
      const char *temphost = rendezvous.get_host_addr ();
      if (temphost == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t) failed in get_host_addr () %p.\n"),
                            -1);
        }
      else
        {
          host = temphost;
        }
    }
  else
    {
      char buffer[MAXHOSTNAMELEN + 1];
      if (rendezvous.get_host_name (buffer, sizeof (buffer)) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t) failed in get_host_name () %p.\n"),
                            -1);
        }
      else
        {
          host = buffer;
        }
    }

  return 0;
}

int
TAO_ORB_Core::fini (void)
{
  // Ask the registry to close all registered connectors!
  // this->connector_registry ()->close_all ();

  TAO_Internal::close_services ();

  if (!this->resource_factory_from_service_config_)
    delete resource_factory_;

  if (!this->client_factory_from_service_config_)
    delete client_factory_;

  if (!this->server_factory_from_service_config_)
    delete server_factory_;

  delete this;

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
      this->resource_factory_from_service_config_ = 1;
    }

  if (this->resource_factory_ == 0)
    {
      // Still don't have one, so let's allocate the default.  This
      // will throw an exception if it fails on exception-throwing
      // platforms.
      if (TAO_orbdebug)
        ACE_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No Resource Factory found in Service Repository."
                    "  Using default instance with GLOBAL resource source specifier.\n"));

      TAO_Default_Resource_Factory* default_factory;
      ACE_NEW_RETURN (default_factory,
                      TAO_Default_Resource_Factory,
                      0);

      this->resource_factory_from_service_config_ = 0;
      default_factory->resource_source (TAO_Default_Resource_Factory::TAO_GLOBAL);
      this->resource_factory_ = default_factory;

      // @@ At this point we need to register this with the
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
        1;
    }

  if (this->client_factory_ == 0)
    {
      // Still don't have one, so let's allocate the default.  This
      // will throw an exception if it fails on exception-throwing
      // platforms.
      if (TAO_orbdebug)
        ACE_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No Client Strategy Factory found in Service Repository."
                    "  Using default instance.\n"));

      ACE_NEW_RETURN (this->client_factory_,
                      TAO_Default_Client_Strategy_Factory,
                      0);

      this->client_factory_from_service_config_ = 0;
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
      this->server_factory_from_service_config_ = 1;
    }

  // If the <server_factory_> isn't found it's usually because the ORB
  // hasn't been intialized correctly...
  if (this->server_factory_ == 0)
    {
      // Still don't have one, so let's allocate the default.
      if (TAO_orbdebug)
        ACE_ERROR ((LM_WARNING,
                    "(%P|%t) WARNING - No %s found in Service Repository."
                    "  Using default instance.\n",
                    "Server Strategy Factory"));

      ACE_NEW_RETURN (this->server_factory_,
                      TAO_Default_Server_Strategy_Factory,
                      0);

      this->server_factory_from_service_config_ = 0;
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
  CORBA::ORB_ptr old_orb = this->orb_;
  this->orb_ = op;
  return old_orb;
}

TAO_POA *
TAO_ORB_Core::root_poa (TAO_POA *np)
{
  // Shouldn't need to check for ptr validity at this point b/c
  // we already did in ::init()
  TAO_POA *old_poa = this->root_poa_;
  this->root_poa_ = np;
  this->root_poa_reference_ = PortableServer::POA::_nil ();
  return old_poa;
}

PortableServer::POA_ptr
TAO_ORB_Core::root_poa_reference (CORBA::Environment &TAO_IN_ENV,
                                  const char *adapter_name,
                                  TAO_POA_Manager *poa_manager,
                                  const TAO_POA_Policies *policies)
{
  if (CORBA::is_nil (this->root_poa_reference_.in ()))
    {
      TAO_POA *poa = this->root_poa (adapter_name,
                                     poa_manager,
                                     policies);

      this->root_poa_reference_ = poa->_this (TAO_IN_ENV);
      TAO_CHECK_RETURN (PortableServer::POA::_nil ());
    }

  return PortableServer::POA::_duplicate (this->root_poa_reference_.in ());
}

#if defined (TAO_ARL_USES_SAME_CONNECTOR_PORT)
CORBA::Boolean
TAO_ORB_Core::arl_same_port_connect (void)
{
  return this->arl_same_port_connect_;
}
#endif /* TAO_ARL_USES_SAME_CONNECTOR_PORT */

int
TAO_ORB_Core::inherit_from_parent_thread (TAO_ORB_Core *p)
{
  // Inherit properties/objects used in ORB_Core from the
  // parent thread.  Stuff inherited here must already exist
  // in the "parent" orbcore.

  this->reactor (p->reactor ());
  // We'll use the spawning thread's reactor.

  this->thr_mgr (p->thr_mgr ());
  // We should use the same thread_manager.

  this->connector_registry (p->connector_registry ());
  // We'll use the spawning thread's connector.

  this->orb (p->orb ());
  // We'll use the spawning thread's ORB.

  this->root_poa (p->root_poa ());
  // And its root_poa.

  this->orb_params_ = p->orb_params ();
  // We also need its ORB_Params.

  this->acceptor (p->acceptor ());
  // Also grab the acceptor passively listening for connection
  // requests.

  this->using_collocation (p->using_collocation ());
  // Use the same collocation settings

  this->resource_factory_ = p->resource_factory ();
  this->client_factory_ = p->client_factory ();
  this->server_factory_ = p->server_factory ();
  // Inherit the factories.  Notice that they will not be destroyed by
  // this orb_core because *_factory_from_service_config_'s all
  // default to FALSE.

  return 0;
}

void
TAO_ORB_Core::create_and_set_root_poa (const char *adapter_name,
                                       TAO_POA_Manager *poa_manager,
                                       const TAO_POA_Policies *policies)
{
  CORBA::Environment env;
  TAO_POA *poa = 0;
  int delete_policies = 0;

  // Need to do double-checked locking here to cover the case of
  // multiple threads using a global resource policy.
  if (poa_manager == 0)
    poa_manager = new TAO_POA_Manager (this->object_adapter ()->lock ());

  TAO_POA_Policies *root_poa_policies = 0;
  if (policies == 0)
    {
      root_poa_policies = new TAO_POA_Policies;
      // RootPOA policies defined in spec
      root_poa_policies->implicit_activation (PortableServer::IMPLICIT_ACTIVATION);

      delete_policies = 1;
      policies = root_poa_policies;
    }

  // Construct a new POA
  poa = new TAO_POA (adapter_name,
                     *poa_manager,
                     *policies,
                     0,
                     this->object_adapter ()->lock (),
                     this->object_adapter ()->thread_lock (),
                     *this,
                     env);

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
      TAO_GLOBAL_Collocation_Table *collocation_table = this->resource_factory ()->get_global_collocation_table ();
      if (collocation_table != 0)
        return collocation_table->bind (this->orb_params ()->addr (),
                                        this->object_adapter ());
    }
  return 0;
}

TAO_Object_Adapter *
TAO_ORB_Core::get_collocated_object_adapter (const ACE_INET_Addr &addr)
{
  if (this->using_collocation ())
    {
      TAO_GLOBAL_Collocation_Table *collocation_table = this->resource_factory ()->get_global_collocation_table ();
      if (collocation_table != 0)
        {
          TAO_Object_Adapter *object_adapter;
          if (collocation_table->find (addr,
                                       object_adapter) == 0)
            return object_adapter;
        }
      else
        {
          if (addr == this->orb_params ()->addr ())
            return this->object_adapter ();
        }
    }
  return 0;
}


int
TAO_ORB_Core::leader_available (void)
  // returns the value of the flag indicating if a leader
  // is available in the leader-follower model
{
  return this->orb ()->leader_follower_info ().leaders_;
}

int
TAO_ORB_Core::I_am_the_leader_thread (void)
  // returns 1 if we are the leader thread,
  // else 0
{
  TAO_Leader_Follower_Info &lf_info = this->orb ()->leader_follower_info ();
  if (lf_info.leaders_)
    return ACE_OS::thr_equal (lf_info.leader_thread_ID_,
                              ACE_Thread::self ());
  return 0;
}

void
TAO_ORB_Core::set_leader_thread (void)
  // sets the thread ID of the leader thread in the leader-follower
  // model
{
  TAO_Leader_Follower_Info &lf_info = this->orb ()->leader_follower_info ();
  ACE_ASSERT ((lf_info.leaders_ >= 1
               && ACE_OS::thr_equal (lf_info.leader_thread_ID_,
                                     ACE_Thread::self ()))
              || lf_info.leaders_ == 0);
  lf_info.leaders_++;
  lf_info.leader_thread_ID_ = ACE_Thread::self ();
}

int
TAO_ORB_Core::unset_leader_wake_up_follower (void)
  // sets the leader_available flag to false and tries to wake up a follower
{
  ACE_Guard <ACE_SYNCH_MUTEX> g (this->leader_follower_lock ());

  this->unset_leader_thread ();

  if (this->follower_available () && !this->leader_available ())
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
  TAO_Leader_Follower_Info &lf_info = this->orb ()->leader_follower_info ();
  ACE_ASSERT ((lf_info.leaders_ > 1
               && ACE_OS::thr_equal (lf_info.leader_thread_ID_,
                                     ACE_Thread::self ()))
              || lf_info.leaders_ == 1);
  lf_info.leaders_--;
}


ACE_SYNCH_MUTEX &
TAO_ORB_Core::leader_follower_lock (void)
  // returns the leader-follower lock
{
  return this->orb ()->leader_follower_info ().leader_follower_lock_;
}

int
TAO_ORB_Core::add_follower (ACE_SYNCH_CONDITION *follower_ptr)
  // adds the a follower to the set of followers in the leader-
  // follower model
  // returns 0 on success, -1 on failure
{
  if (this->orb ()->leader_follower_info ().follower_set_.insert (follower_ptr) != 0)
    return -1;
  return 0;
}

int
TAO_ORB_Core::follower_available (void)
// checks for the availablity of a follower
  // returns 1 on available, 0 else
{
  return !this->orb ()->leader_follower_info ().follower_set_.is_empty ();
}

int
TAO_ORB_Core::remove_follower (ACE_SYNCH_CONDITION *follower_ptr)
  // removes a follower from the leader-follower set
  // returns 0 on success, -1 on failure
{
  return this->orb ()->leader_follower_info ().follower_set_.remove (follower_ptr);
}

ACE_SYNCH_CONDITION*
TAO_ORB_Core::get_next_follower (void)
  // returns randomly a follower from the leader-follower set
  // returns follower on success, else 0
{
  ACE_Unbounded_Set_Iterator<ACE_SYNCH_CONDITION *> iterator (
    this->orb ()->leader_follower_info ().follower_set_);
  if (iterator.first () == 0)
    // means set is empty
    return 0;
  return *iterator;
}

ACE_Allocator*
TAO_ORB_Core::input_cdr_dblock_allocator (void)
{
#if 0
  if (this->input_cdr_dblock_allocator_ == 0)
    {
      this->input_cdr_dblock_allocator_ =
        this->resource_factory ()->input_cdr_dblock_allocator ();
    }
  return this->input_cdr_dblock_allocator_;
#else
  return this->resource_factory ()->input_cdr_dblock_allocator ();
#endif
}

ACE_Allocator*
TAO_ORB_Core::input_cdr_buffer_allocator (void)
{
#if 0
  if (this->input_cdr_buffer_allocator_ == 0)
    {
      this->input_cdr_buffer_allocator_ =
        this->resource_factory ()->input_cdr_buffer_allocator ();
    }
  return this->input_cdr_buffer_allocator_;
#else
  return this->resource_factory ()->input_cdr_buffer_allocator ();
#endif
}

ACE_Allocator*
TAO_ORB_Core::output_cdr_dblock_allocator (void)
{
  return this->resource_factory ()->output_cdr_dblock_allocator ();
}

ACE_Allocator*
TAO_ORB_Core::output_cdr_buffer_allocator (void)
{
  return this->resource_factory ()->output_cdr_buffer_allocator ();
}

ACE_Reactor *
TAO_ORB_Core::reactor (ACE_Reactor *r)
{
  TAO_ORB_Core_TSS_Resources *tss =
    TAO_ORB_CORE_TSS_RESOURCES::instance ();

  ACE_Reactor *old_reactor = tss->reactor_;
  tss->reactor_ = r;
  return old_reactor;
}

ACE_Reactor *
TAO_ORB_Core::reactor (void)
{
  TAO_ORB_Core_TSS_Resources *tss =
    TAO_ORB_CORE_TSS_RESOURCES::instance ();

  if (tss->reactor_ == 0)
    tss->reactor_ = this->resource_factory ()->get_reactor ();

  return tss->reactor_;
}

TAO_POA_Current *
TAO_ORB_Core::poa_current (void)
{
  return TAO_ORB_CORE_TSS_RESOURCES::instance ()->poa_current_;
}

TAO_POA_Current *
TAO_ORB_Core::poa_current (TAO_POA_Current *new_current)
{
  TAO_ORB_Core_TSS_Resources *tss =
    TAO_ORB_CORE_TSS_RESOURCES::instance ();

  TAO_POA_Current *old = tss->poa_current_;
  tss->poa_current_ = new_current;
  return old;
}

CORBA_Environment*
TAO_ORB_Core::default_environment (void) const
{
  return TAO_ORB_CORE_TSS_RESOURCES::instance ()->default_environment_;
}

void
TAO_ORB_Core::default_environment (CORBA_Environment* env)
{
  TAO_ORB_CORE_TSS_RESOURCES::instance ()->default_environment_ = env;
}

#if defined (TAO_HAS_CORBA_MESSAGING)

TAO_Policy_Current*
TAO_ORB_Core::policy_current (void) const
{
  return TAO_ORB_CORE_TSS_RESOURCES::instance ()->policy_current_;
}

TAO_Policy_Current*
TAO_ORB_Core::policy_current (TAO_Policy_Current* current)
{
  TAO_ORB_Core_TSS_Resources *tss =
    TAO_ORB_CORE_TSS_RESOURCES::instance ();

  TAO_Policy_Current *old = tss->policy_current_;
  tss->policy_current_ = current;
  return old;
}

#endif /* TAO_HAS_CORBA_MESSAGING */

// ****************************************************************

TAO_ORB_Core_TSS_Resources::TAO_ORB_Core_TSS_Resources (void)
  :  reactor_ (0),
     poa_current_ (0),
     default_environment_ (&this->tss_environment_),
#if defined (TAO_HAS_CORBA_MESSAGING)
     policy_current_ (&this->initial_policy_current_),
#endif /* TAO_HAS_CORBA_MESSAGING */
     output_cdr_dblock_allocator_ (0),
     output_cdr_buffer_allocator_ (0),
     connection_cache_ (0)
{
}

TAO_ORB_Core_TSS_Resources::~TAO_ORB_Core_TSS_Resources (void)
{
  if (this->output_cdr_dblock_allocator_ != 0)
    {
      delete this->output_cdr_dblock_allocator_;
      this->output_cdr_dblock_allocator_ = 0;
    }
  if (this->output_cdr_buffer_allocator_ != 0)
    {
      delete this->output_cdr_buffer_allocator_;
      this->output_cdr_buffer_allocator_ = 0;
    }
}

// ****************************************************************

// ****************************************************************

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
  return CORBA::instance ()->orb_core_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Env_Value<int>;
template class ACE_Env_Value<u_int>;

template class ACE_TSS_Singleton<TAO_ORB_Core_TSS_Resources, ACE_SYNCH_MUTEX>;
template class ACE_TSS<TAO_ORB_Core_TSS_Resources>;

template class ACE_Hash_Map_Manager<ACE_INET_Addr, TAO_Object_Adapter *, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Entry<ACE_INET_Addr, TAO_Object_Adapter *>;
template class ACE_Hash<ACE_INET_Addr>;
template class ACE_Equal_To<ACE_INET_Addr>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_INET_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Iterator<ACE_INET_Addr, TAO_Object_Adapter *, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Iterator_Ex<ACE_INET_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_INET_Addr, TAO_Object_Adapter *, TAO_Collocation_Table_Lock>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_INET_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, TAO_Collocation_Table_Lock>;

template class ACE_Guard<TAO_Collocation_Table_Lock>;
template class ACE_Read_Guard<TAO_Collocation_Table_Lock>;
template class ACE_Write_Guard<TAO_Collocation_Table_Lock>;
template class ACE_Read_Guard<ACE_SYNCH_MUTEX>;
template class ACE_Write_Guard<ACE_SYNCH_MUTEX>;

template class ACE_Singleton<TAO_GLOBAL_Collocation_Table, ACE_SYNCH_MUTEX>;

template class ACE_Node<ACE_SYNCH_CONDITION*>;
template class ACE_Unbounded_Set<ACE_SYNCH_CONDITION*>;
template class ACE_Unbounded_Set_Iterator<ACE_SYNCH_CONDITION*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Env_Value<int>
#pragma instantiate ACE_Env_Value<u_int>

#pragma instantiate ACE_TSS_Singleton<TAO_ORB_Core_TSS_Resources, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_TSS<TAO_ORB_Core_TSS_Resources>

#pragma instantiate ACE_Hash_Map_Manager<ACE_INET_Addr, TAO_Object_Adapter *, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Entry<ACE_INET_Addr, TAO_Object_Adapter *>
#pragma instantiate ACE_Hash<ACE_INET_Addr>
#pragma instantiate ACE_Equal_To<ACE_INET_Addr>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_INET_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_INET_Addr, TAO_Object_Adapter *, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_INET_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_INET_Addr, TAO_Object_Adapter *, TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_INET_Addr, TAO_Object_Adapter *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, TAO_Collocation_Table_Lock>

#pragma instantiate ACE_Guard<TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Read_Guard<TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Write_Guard<TAO_Collocation_Table_Lock>
#pragma instantiate ACE_Read_Guard<ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Write_Guard<ACE_SYNCH_MUTEX>

#pragma instantiate ACE_Singleton<TAO_GLOBAL_Collocation_Table, ACE_SYNCH_MUTEX>

#pragma instantiate ACE_Node<ACE_SYNCH_CONDITION*>
#pragma instantiate ACE_Unbounded_Set<ACE_SYNCH_CONDITION*>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_SYNCH_CONDITION*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
