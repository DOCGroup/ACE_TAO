// $Id$

#include "Naming_Utils.h"
#include "Transient_Naming_Context.h"
#include "Persistent_Context_Index.h"
#include "Storable_Naming_Context.h"
#include "Flat_File_Persistence.h"
#include "orbsvcs/CosNamingC.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"

#include "tao/IORTable/IORTable.h"

#include "ace/Arg_Shifter.h"
#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"

ACE_RCSID (Naming,
           Naming_Utils,
           "$Id$")

TAO_Naming_Server::TAO_Naming_Server (void)
  : naming_context_ (),
    ior_multicast_ (0),
    naming_service_ior_ (),
    context_index_ (0),
    ior_output_file_ (0),
    pid_file_name_ (0),
    context_size_ (ACE_DEFAULT_MAP_SIZE),
    persistence_file_name_ (0),
    base_address_ (TAO_NAMING_BASE_ADDR),
    multicast_ (0),
    use_storable_context_ (0)
{
}

TAO_Naming_Server::TAO_Naming_Server (CORBA::ORB_ptr orb,
                                      PortableServer::POA_ptr poa,
                                      size_t context_size,
                                      ACE_Time_Value *timeout,
                                      int resolve_for_existing_naming_service,
                                      const ACE_TCHAR *persistence_location,
                                      void *base_addr,
                                      int enable_multicast,
                                      int use_storable_context)
  : naming_context_ (),
    ior_multicast_ (0),
    naming_service_ior_ (),
    context_index_ (0),
    ior_output_file_ (0),
    pid_file_name_ (0),
    context_size_ (ACE_DEFAULT_MAP_SIZE),
    persistence_file_name_ (0),
    base_address_ (TAO_NAMING_BASE_ADDR),
    multicast_ (0),
    use_storable_context_ (0)
{
  if (this->init (orb,
                  poa,
                  context_size,
                  timeout,
                  resolve_for_existing_naming_service,
                  persistence_location,
                  base_addr,
                  enable_multicast,
                  use_storable_context) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) %p\n",
                "TAO_Naming_Server::init"));
}


int
TAO_Naming_Server::init (CORBA::ORB_ptr orb,
                         PortableServer::POA_ptr poa,
                         size_t context_size,
                         ACE_Time_Value *timeout,
                         int resolve_for_existing_naming_service,
                         const ACE_TCHAR *persistence_location,
                         void *base_addr,
                         int enable_multicast,
                         int use_storable_context)
{
  if (resolve_for_existing_naming_service)
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          // Try to find an existing Naming Service.
          CORBA::Object_var naming_obj =
            orb->resolve_initial_references ("NameService",
                                             timeout
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (!CORBA::is_nil (naming_obj.in ()))
            {
              //
              // Success in finding a Naming Service.
              //
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            "\nNameService found!\n"));

              this->naming_context_ =
                CosNaming::NamingContext::_narrow (naming_obj.in ()
                                                   ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              this->naming_service_ior_ =
                orb->object_to_string (naming_obj.in ()
                                       ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              return 0;
            }
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Naming_Server::init");
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (-1);
    }

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "\nWe'll become a NameService\n"));

  // Become a Naming Service.
  return this->init_new_naming (orb,
                                poa,
                                persistence_location,
                                base_addr,
                                context_size,
                                enable_multicast,
                                use_storable_context);
}

int
TAO_Naming_Server::parse_args (int argc,
                               char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "b:do:p:s:f:m:u:");
  int c;
  int size, result;
  long address;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o': // outputs the naming service ior to a file.
        this->ior_output_file_ =
          ACE_OS::fopen (get_opts.opt_arg (), "w");

        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.opt_arg ()), -1);
        break;
      case 'p':
        this->pid_file_name_ = get_opts.opt_arg ();
        break;
      case 'f':
        this->persistence_file_name_ = get_opts.opt_arg ();
        break;
      case 's':
        size = ACE_OS::atoi (get_opts.opt_arg ());
        if (size >= 0)
          this->context_size_ = size;
        break;
      case 'b':
        result = ::sscanf (get_opts.opt_arg (),
                           "%ld",
                           &address);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-b> option"),
                            -1);
        this->base_address_ = (void *) address;
        break;
      case 'm':
        this->multicast_ = ACE_OS::atoi(get_opts.opt_arg ());
        break;
      case 'u':
        this->use_storable_context_ = 1;
        this->persistence_file_name_ = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-NScontextname <contextname> "
                           "-o <ior_output_file> "
                           "-p <pid_file_name> "
                           "-f <persistence_file_name> "
                           "-b <base_address> "
                           "-m <1=enable multicast, 0=disable multicast(default) "
                           "-u <storable_persistence_directory (not used with -f)> "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
TAO_Naming_Server::init_with_orb (int argc,
                                  char *argv [],
                                  CORBA::ORB_ptr orb)
{
  int result;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Duplicate the ORB
      this->orb_ = CORBA::ORB::_duplicate (orb);

      // Get the POA from the ORB.
      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                            -1);
        }

      // Get the POA object.
      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ()
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POA_Manager.
      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList policies (2);
      policies.length (2);

      // Id Assignment policy
      policies[0] =
        this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Lifespan policy
      policies[1] =
        this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // We use a different POA, otherwise the user would have to change
      // the object key each time it invokes the server.
      this->ns_poa_ = this->root_poa_->create_POA ("NameService",
                                                   poa_manager.in (),
                                                   policies
                                                   ACE_ENV_ARG_PARAMETER);
      // Warning!  If create_POA fails, then the policies won't be
      // destroyed and there will be hell to pay in memory leaks!
      ACE_TRY_CHECK;

      // Creation of the new POAs over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Check the non-ORB arguments.  this needs to come before we
      // initialize my_naming_server so that we can pass on some of
      // the command-line arguments.
      result = this->parse_args (argc, argv);

      if (result < 0)
        return result;

      result = this->init (orb,
                           this->ns_poa_.in (),
                           this->context_size_,
                           0,
                           0,
                           this->persistence_file_name_,
                           this->base_address_,
                           this->multicast_,
                           this->use_storable_context_);
      if (result == -1)
        return result;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Naming_Service::init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  if (this->ior_output_file_ != 0)
    {
      CORBA::String_var str =
        this->naming_service_ior ();
      ACE_OS::fprintf (this->ior_output_file_,
                       "%s",
                       str.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }

  if (this->pid_file_name_ != 0)
    {
      FILE *pidf = fopen (this->pid_file_name_, "w");
      if (pidf != 0)
        {
          ACE_OS::fprintf (pidf,
                           "%ld\n",
                           ACE_static_cast (long, ACE_OS::getpid ()));
          ACE_OS::fclose (pidf);
        }
    }
  return 0;
}

int
TAO_Naming_Server::init_new_naming (CORBA::ORB_ptr orb,
                                    PortableServer::POA_ptr poa,
                                    const ACE_TCHAR *persistence_location,
                                    void *base_addr,
                                    size_t context_size,
                                    int enable_multicast,
                                    int use_storable_context)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (use_storable_context)
        {
          // In lieu of a fully implemented service configurator version
          // of this Reader and Writer, let's just take something off the
          // command line for now.
          TAO_Naming_Service_Persistence_Factory *persFactory = 0;
          ACE_NEW_RETURN(persFactory, TAO_NS_FlatFileFactory, -1);

          // Was a location specified?
          if (persistence_location == 0)
            {
              // No, assign the default location "."
              persistence_location = ".";
            }

          this->naming_context_ =
            TAO_Storable_Naming_Context::recreate_all(orb,
                                                      poa,
                                                      TAO_ROOT_NAMING_CONTEXT,
                                                      context_size,
                                                      0,
                                                      persFactory,
                                                      persistence_location
                                                      ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else if (persistence_location != 0)
        //
        // Initialize Persistent Naming Service.
        //
        {
          // Allocate and initialize Persistent Context Index.
          ACE_NEW_RETURN (this->context_index_,
                          TAO_Persistent_Context_Index (orb, poa),
                          -1);

          if (this->context_index_->open (persistence_location,
                                          base_addr) == -1
              || this->context_index_->init (context_size) == -1)
            {
              if (TAO_debug_level >0)
                ACE_DEBUG ((LM_DEBUG,
                            "TAO_Naming_Server: context_index initialization failed\n"));
              return -1;
            }

          // Set the root Naming Context reference.
          this->naming_context_ =
            this->context_index_->root_context ();
        }
      else
        {
          //
          // Initialize Transient Naming Service.
          //
          this->naming_context_ =
            TAO_Transient_Naming_Context::make_new_context (poa,
                                                            TAO_ROOT_NAMING_CONTEXT,
                                                            context_size
                                                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

        }

      // Set the ior of the root Naming Context.
      this->naming_service_ior_=
        orb->object_to_string (this->naming_context_.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var table_object =
        orb->resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
        }
      else
        {
          CORBA::String_var ior =
            orb->object_to_string (this->naming_context_.in ()
                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          adapter->bind ("NameService", ior.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

#if defined (ACE_HAS_IP_MULTICAST)
      if (enable_multicast)
        {
          // @@ Marina: is there anyway to implement this stuff
          // without using ORB_Core_instance()? For example can you
          // pass the ORB as an argument?

          //
          // Install ior multicast handler.
          //
          // Get reactor instance from TAO.
          ACE_Reactor *reactor =
            TAO_ORB_Core_instance ()->reactor ();

          // See if the -ORBMulticastDiscoveryEndpoint option was specified.
          ACE_CString mde (TAO_ORB_Core_instance ()->orb_params ()
                           ->mcast_discovery_endpoint ());

          // First, see if the user has given us a multicast port number
          // on the command-line;
          u_short port =
            TAO_ORB_Core_instance ()->orb_params ()->service_port (NAMESERVICE);

          if (port == 0)
            {
              // Check environment var. for multicast port.
              const char *port_number =
                ACE_OS::getenv ("NameServicePort");

              if (port_number != 0)
                port = ACE_OS::atoi (port_number);
            }

          // Port wasn't specified on the command-line or in environment -
          // use the default.
          if (port == 0)
            port = TAO_DEFAULT_NAME_SERVER_REQUEST_PORT;

          // Instantiate a handler which will handle client requests for
          // the root Naming Context ior, received on the multicast port.
          ACE_NEW_RETURN (this->ior_multicast_,
                          TAO_IOR_Multicast (),
                          -1);

          if (mde.length () != 0)
            {
              if (this->ior_multicast_->init (this->naming_service_ior_.in (),
                                              mde.c_str (),
                                              TAO_SERVICEID_NAMESERVICE) == -1)
                return -1;
            }
          else
            {
              if (this->ior_multicast_->init (this->naming_service_ior_.in (),
                                              port,
                                              ACE_DEFAULT_MULTICAST_ADDR,
                                              TAO_SERVICEID_NAMESERVICE) == -1)
                return -1;
            }

          // Register event handler for the ior multicast.
          if (reactor->register_handler (this->ior_multicast_,
                                         ACE_Event_Handler::READ_MASK) == -1)
            {
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            "TAO_Naming_Server: cannot register Event handler\n"));
              return -1;
            }

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO_Naming_Server: The multicast server setup is done.\n"));
        }
#else
  ACE_UNUSED_ARG (enable_multicast);
#endif /* ACE_HAS_IP_MULTICAST */
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Naming_Server");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
TAO_Naming_Server::fini (void)
{
  // Destroy the child POA ns_poa that is created when initializing
  // the Naming Service
  ACE_TRY_NEW_ENV
    {
      this->ns_poa_->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      CORBA::Object_var table_object =
        this->orb_->resolve_initial_references ("IORTable"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
        }
      else
        {
          adapter->unbind ("NameService" ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      // Ignore
    }
  ACE_ENDTRY;
  return 0;
}

char*
TAO_Naming_Server::naming_service_ior (void)
{
  return CORBA::string_dup (this->naming_service_ior_.in ());
}

CosNaming::NamingContext_ptr
TAO_Naming_Server::operator-> (void) const
{
  return this->naming_context_.ptr ();
}

TAO_Naming_Server::~TAO_Naming_Server (void)
{
  if (this->ior_multicast_ != 0)
    {
      TAO_ORB_Core_instance ()->reactor ()->remove_handler
        (this->ior_multicast_,
         ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);
      delete this->ior_multicast_;
    }
  delete context_index_;
}


CosNaming::NamingContext_ptr
TAO_Naming_Client::operator -> (void) const
{
  return this->naming_context_.ptr ();
}

CosNaming::NamingContext_ptr
TAO_Naming_Client::get_context (void) const
{
  // Must duplicate this pointer so it can be stored in a _var.
  return CosNaming::NamingContext::_duplicate (this->naming_context_.ptr ());
}

int
TAO_Naming_Client::init (CORBA::ORB_ptr orb,
                         ACE_Time_Value *timeout)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService", timeout ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the NameService.\n"),
                          -1);
      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

TAO_Naming_Client::TAO_Naming_Client (void)
{
  // Do nothing
}

TAO_Naming_Client::~TAO_Naming_Client (void)
{
  // Do nothing
}
