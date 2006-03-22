// $Id$

#include "Naming_Server.h"
#include "Transient_Naming_Context.h"
#include "Persistent_Context_Index.h"
#include "Storable_Naming_Context.h"
#include "Storable_Naming_Context_Activator.h"
#include "Flat_File_Persistence.h"
#include "orbsvcs/CosNamingC.h"

#include "tao/debug.h"
#include "tao/default_ports.h"
#include "tao/ORB_Core.h"

#include "tao/IORTable/IORTable.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
#include "tao/Messaging/Messaging.h"
#endif

#include "tao/AnyTypeCode/Any.h"

#include "ace/Arg_Shifter.h"
#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

ACE_RCSID (Naming,
           Naming_Server,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Naming_Server::TAO_Naming_Server (void)
  : naming_context_ (),
    ior_multicast_ (0),
    naming_service_ior_ (),
    context_index_ (0),
    ior_file_name_ (0),
    pid_file_name_ (0),
    context_size_ (ACE_DEFAULT_MAP_SIZE),
    persistence_file_name_ (0),
    base_address_ (TAO_NAMING_BASE_ADDR),
    multicast_ (0),
    use_storable_context_ (0),
    use_servant_activator_ (0),
    servant_activator_ (0),
    use_redundancy_(0),
    round_trip_timeout_ (0),
    use_round_trip_timeout_ (0)
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
                                      int use_storable_context,
                                      int round_trip_timeout,
                                      int use_round_trip_timeout)
  : naming_context_ (),
    ior_multicast_ (0),
    naming_service_ior_ (),
    context_index_ (0),
    ior_file_name_ (0),
    pid_file_name_ (0),
    context_size_ (ACE_DEFAULT_MAP_SIZE),
    persistence_file_name_ (0),
    base_address_ (TAO_NAMING_BASE_ADDR),
    multicast_ (0),
    use_storable_context_ (use_storable_context),
    use_servant_activator_ (0),
    servant_activator_ (0),
    use_redundancy_(0),
    round_trip_timeout_ (0),
    use_round_trip_timeout_ (0)
{
  if (this->init (orb,
                  poa,
                  context_size,
                  timeout,
                  resolve_for_existing_naming_service,
                  persistence_location,
                  base_addr,
                  enable_multicast,
                  use_storable_context,
                  round_trip_timeout,
                  use_round_trip_timeout) == -1)
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
                         int use_storable_context,
                         int round_trip_timeout,
                         int use_round_trip_timeout)
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
                                use_storable_context,
                                round_trip_timeout,
                                use_round_trip_timeout);
}

int
TAO_Naming_Server::parse_args (int argc,
                               ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:do:p:s:f:m:u:r:z:"));

  int c;
  int size, result;

  // This is declared this way to avoid warnings from
  // some compilers that complain about mismatching types
  // in the sscanf.
#if ACE_SIZEOF_VOID_P == ACE_SIZEOF_LONG_LONG
  ptrdiff_t address;
#else
  long int address;
#endif /* ACE_SIZEOF_VOID_P */

  // Make sure only one persistence option is specified
  int f_opt_used = 0;
  int u_opt_used = 0;
  int r_opt_used = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o': // outputs the naming service ior to a file.
        this->ior_file_name_ = get_opts.opt_arg ();
        break;
      case 'p':
        this->pid_file_name_ = get_opts.opt_arg ();
        break;
      case 's':
        size = ACE_OS::atoi (get_opts.opt_arg ());
        if (size >= 0)
          this->context_size_ = size;
        break;
      case 'b':
        result = ::sscanf (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()),
#if ACE_SIZEOF_VOID_P == ACE_SIZEOF_LONG_LONG
                           ACE_INT64_FORMAT_SPECIFIER,
#else
                           "%ld",
#endif /* ACE_SIZEOF_VOID_P */
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
      case 'f':
        this->persistence_file_name_ = get_opts.opt_arg ();
        f_opt_used = 1;
        break;
      case 'r':
        this->use_redundancy_ = 1;
        this->use_storable_context_ = 1;
        this->persistence_file_name_ = get_opts.opt_arg ();
        r_opt_used = 1;
        break;
      case 'u':
        this->use_storable_context_ = 1;
        this->persistence_file_name_ = get_opts.opt_arg ();
        u_opt_used = 1;
        break;
      case 'z':
        this->use_round_trip_timeout_ = 1;
        this->round_trip_timeout_ = (int)1.0e7 * ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-d ")
                           ACE_TEXT ("-o <ior_output_file> ")
                           ACE_TEXT ("-p <pid_file_name> ")
                           ACE_TEXT ("-s <context_size> ")
                           ACE_TEXT ("-b <base_address> ")
                           ACE_TEXT ("-m <1=enable multicast, 0=disable multicast(default) ")
                           ACE_TEXT ("-f <persistence_file_name> ")
                           ACE_TEXT ("-u <storable_persistence_directory (not used with -f)> ")
                           ACE_TEXT ("-r <redundant_persistence_directory> ")
                           ACE_TEXT ("-z <relative round trip timeout> ")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }

  if (f_opt_used + u_opt_used + r_opt_used > 1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Only one persistence option can be passed")
                       ACE_TEXT ("\n")),
                      -1);

  return 0;
}

int
TAO_Naming_Server::init_with_orb (int argc,
                                  ACE_TCHAR *argv [],
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
                             ACE_TEXT(" (%P|%t) Unable to initialize the POA.\n")),
                            -1);
        }

      // Check the non-ORB arguments.  this needs to come before we
      // initialize my_naming_server so that we can pass on some of
      // the command-line arguments.
      result = this->parse_args (argc, argv);

      if (result < 0)
        return result;

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

      int numPolicies = 2;
#if (TAO_HAS_MINIMUM_POA == 0)
      if (this->use_storable_context_)
        {
          this->use_servant_activator_ = 1;
        }

      if (this->use_servant_activator_) {
        numPolicies += 2;
      }
#endif /* TAO_HAS_MINIMUM_POA */

      CORBA::PolicyList policies (numPolicies);
      policies.length (numPolicies);

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

#if (TAO_HAS_MINIMUM_POA == 0)
      if (this->use_servant_activator_)
        {
          // Request Processing Policy
          policies[2] =
            this->root_poa_->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER
                                                               ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Servant Retention Policy
          policies[3] =
            this->root_poa_->create_servant_retention_policy (PortableServer::RETAIN
                                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
#endif /* TAO_HAS_MINIMUM_POA */

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

      result = this->init (orb,
                           this->ns_poa_.in (),
                           this->context_size_,
                           0,
                           0,
                           this->persistence_file_name_,
                           this->base_address_,
                           this->multicast_,
                           this->use_storable_context_,
                           this->round_trip_timeout_,
                           this->use_round_trip_timeout_);
      if (result == -1)
        return result;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Naming_Server::init_with_orb");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  if (this->ior_file_name_ != 0)
    {
      FILE *iorf = ACE_OS::fopen (this->ior_file_name_, ACE_TEXT("w"));
      if (iorf == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("Unable to open %s for writing:(%u) %p\n"),
                             this->ior_file_name_,
                             errno,
                             ACE_TEXT("TAO_Naming_Server::init_with_orb")),
                            -1);
        }

      CORBA::String_var str = this->naming_service_ior ();

      ACE_OS::fprintf (iorf, "%s\n", str.in ());
      ACE_OS::fclose (iorf);
    }

  if (this->pid_file_name_ != 0)
    {
      FILE *pidf = ACE_OS::fopen (this->pid_file_name_, ACE_TEXT("w"));
      if (pidf != 0)
        {
          ACE_OS::fprintf (pidf,
                           "%ld\n",
                           static_cast<long> (ACE_OS::getpid ()));
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
                                    int use_storable_context,
                                    int round_trip_timeout,
                                    int use_round_trip_timeout)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (use_storable_context)
        {
          // In lieu of a fully implemented service configurator version
          // of this Reader and Writer, let's just take something off the
          // command line for now.
          TAO_Naming_Service_Persistence_Factory* pf = 0;
          ACE_NEW_RETURN(pf, TAO_NS_FlatFileFactory, -1);
          auto_ptr<TAO_Naming_Service_Persistence_Factory> persFactory(pf);
          // This instance will either get deleted after recreate all or,
          // in the case of a servant activator's use, on destruction of the
          // activator.

          // Was a location specified?
          if (persistence_location == 0)
            {
              // No, assign the default location "NameService"
              persistence_location = ACE_TEXT("NameService");
            }

          // Now make sure this directory exists
          if (ACE_OS::access (persistence_location, W_OK|X_OK))
            {
              ACE_ERROR ((LM_ERROR, "Invalid persistence directory\n"));
              return -1;
            }

#if (TAO_HAS_MINIMUM_POA == 0)
          if (this->use_servant_activator_)
            {
              ACE_NEW_THROW_EX (this->servant_activator_,
                                TAO_Storable_Naming_Context_Activator (orb,
                                                                       persFactory.get(),
                                                                       persistence_location,
                                                                       context_size),
                                CORBA::NO_MEMORY ());
              this->ns_poa_->set_servant_manager(this->servant_activator_);
            }
#endif /* TAO_HAS_MINIMUM_POA */

          this->naming_context_ =
            TAO_Storable_Naming_Context::recreate_all (orb,
                                                       poa,
                                                       TAO_ROOT_NAMING_CONTEXT,
                                                       context_size,
                                                       0,
                                                       persFactory.get(),
                                                       persistence_location,
                                                       use_redundancy_
                                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (this->use_servant_activator_)
            persFactory.release();
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

      // Register with the ORB's resolve_initial_references()
      // mechanism.  Primarily useful for dynamically loaded Name
      // Services.
      orb->register_initial_reference ("NameService",
                                       this->naming_context_.in ()
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
          ACE_Reactor *reactor = orb->orb_core()->reactor ();

          // See if the -ORBMulticastDiscoveryEndpoint option was specified.
          ACE_CString mde (orb->orb_core ()->orb_params ()->mcast_discovery_endpoint ());

          // First, see if the user has given us a multicast port number
          // on the command-line;
          u_short port =
            orb->orb_core ()->orb_params ()->service_port (TAO::MCAST_NAMESERVICE);

          if (port == 0)
            {
              // Check environment var. for multicast port.
              const char *port_number =
                ACE_OS::getenv ("NameServicePort");

              if (port_number != 0)
                port = static_cast<u_short> (ACE_OS::atoi (port_number));
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

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
      if (use_round_trip_timeout == 1)
      {
        TimeBase::TimeT roundTripTimeoutVal = round_trip_timeout;
        CORBA::Any anyObjectVal;
        anyObjectVal <<= roundTripTimeoutVal;
        CORBA::PolicyList polList (1);
        polList.length (1);
        polList[0] = orb->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                                         anyObjectVal);
        ACE_TRY_CHECK;

        // set a timeout on the orb
        //
        CORBA::Object_var orbPolicyManagerObj =
          orb->resolve_initial_references ("ORBPolicyManager"
                                           ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        CORBA::PolicyManager_var orbPolicyManager =
          CORBA::PolicyManager::_narrow (orbPolicyManagerObj.in ());
        ACE_TRY_CHECK;
        orbPolicyManager->set_policy_overrides (polList, CORBA::SET_OVERRIDE);

        polList[0]->destroy ();
        ACE_TRY_CHECK;
        polList[0] = CORBA::Policy::_nil ();
      }
#else
  ACE_UNUSED_ARG (use_round_trip_timeout);
#endif /* TAO_HAS_CORBA_MESSAGING */
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Naming_Server::init_new_naming");
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


  if (this->ior_multicast_ != 0)
    {
      orb_->orb_core()->reactor ()->remove_handler (this->ior_multicast_,
         ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);
      delete this->ior_multicast_;
    }

  delete this->context_index_;

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
  return this->naming_context_.in ();
}

TAO_Naming_Server::~TAO_Naming_Server (void)
{
#if (TAO_HAS_MINIMUM_POA == 0)
  if (this->use_servant_activator_)
    delete this->servant_activator_;
#endif /* TAO_HAS_MINIMUM_POA */
}

TAO_END_VERSIONED_NAMESPACE_DECL
