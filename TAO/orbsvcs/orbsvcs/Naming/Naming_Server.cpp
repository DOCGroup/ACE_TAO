
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Naming/Naming_Server.h"

// Placing the include of Messanging.h at this point
// allows for successful compilation if versioned namespaces
// are enabled.
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
#include "tao/Messaging/Messaging.h"
#endif

#include "orbsvcs/Naming/Transient_Naming_Context.h"
#include "orbsvcs/Naming/Persistent_Naming_Context_Factory.h"
#include "orbsvcs/Naming/Storable_Naming_Context_Factory.h"

#if !defined (CORBA_E_MICRO)
#include "orbsvcs/Naming/Persistent_Context_Index.h"
#include "orbsvcs/Naming/Storable_Naming_Context.h"
#include "orbsvcs/Naming/Storable_Naming_Context_Activator.h"

#include "tao/Storable_FlatFileStream.h"

#endif /* CORBA_E_MICRO */

#include "orbsvcs/CosNamingC.h"

#include "tao/debug.h"
#include "tao/default_ports.h"
#include "tao/ORB_Core.h"

#include "tao/IORTable/IORTable.h"

#include "tao/AnyTypeCode/Any.h"

#include "ace/Arg_Shifter.h"
#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Naming_Server::TAO_Naming_Server (size_t bsize)
  : ior_multicast_ (0),
    pid_file_name_ (0),
    iors_ (0),
    bundle_size_ (bsize),
    context_size_ (ACE_DEFAULT_MAP_SIZE),
    multicast_ (0),
#if !defined (CORBA_E_MICRO)
    context_index_ (0),
    persistence_dir_ (0),
    base_address_ (TAO_NAMING_BASE_ADDR),
    use_storable_context_ (0),
    use_servant_activator_ (false),
    servant_activator_ (0),
#endif /* CORBA_E_MICRO */
    use_redundancy_(0),
    round_trip_timeout_ (0),
    use_round_trip_timeout_ (0)
{
  ACE_NEW (iors_, IOR_Bundle[bsize]);
}

TAO_Naming_Server::TAO_Naming_Server (CORBA::ORB_ptr orb,
                                      PortableServer::POA_ptr poa,
                                      size_t context_size,
                                      ACE_Time_Value *timeout,
                                      bool resolve_for_existing_naming_service,
                                      const ACE_TCHAR *persistence_location,
                                      void *base_addr,
                                      int enable_multicast,
                                      int use_storable_context,
                                      int round_trip_timeout,
                                      int use_round_trip_timeout,
                                      size_t bsize)
  : ior_multicast_ (0),
    pid_file_name_ (0),
    iors_ (0),
    bundle_size_ (bsize),
    context_size_ (ACE_DEFAULT_MAP_SIZE),
    multicast_ (0),
#if !defined (CORBA_E_MICRO)
    context_index_ (0),
    persistence_dir_ (0),
    base_address_ (TAO_NAMING_BASE_ADDR),
    use_storable_context_ (use_storable_context),
    use_servant_activator_ (false),
    servant_activator_ (0),
#endif /* CORBA_E_MICRO */
    use_redundancy_(0),
    round_trip_timeout_ (0),
    use_round_trip_timeout_ (0)
{
  ACE_NEW (iors_, IOR_Bundle[bsize]);
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
    ORBSVCS_ERROR ((LM_ERROR,
                "(%P|%t) %p\n",
                "TAO_Naming_Server::init"));
}

int
TAO_Naming_Server::init (CORBA::ORB_ptr orb,
                         PortableServer::POA_ptr poa,
                         size_t context_size,
                         ACE_Time_Value *timeout,
                         bool resolve_for_existing_naming_service,
                         const ACE_TCHAR *persistence_location,
                         void *base_addr,
                         int enable_multicast,
                         int use_storable_context,
                         int round_trip_timeout,
                         int use_round_trip_timeout)
{
  if (CORBA::is_nil (this->orb_.in ()))
    {
      this->orb_ = CORBA::ORB::_duplicate (orb);
    }
  for (size_t i = 0; i < bundle_size_; i++ )
    {
      this->iors_[i].ref_ = CORBA::Object::_nil();
    }

  if (resolve_for_existing_naming_service)
    {
      try
        {
          // Try to find an existing Naming Service.
          CORBA::Object_var naming_obj =
            orb->resolve_initial_references ("NameService", timeout);

          if (!CORBA::is_nil (naming_obj.in ()))
            {
              //
              // Success in finding a Naming Service.
              //
              if (TAO_debug_level > 0)
                ORBSVCS_DEBUG ((LM_DEBUG,
                                "\nNameService found!\n"));

              this->assign (size_t(ROOT), false, naming_obj);
              return 0;
            }
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("TAO_Naming_Server::init");
        }
    }

  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG,
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
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:do:p:s:f:m:u:r:z:"));
#else
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:do:p:s:f:m:z:"));
#endif /* TAO_HAS_MINIMUM_POA */

  int c;
  int size;
#if !defined (CORBA_E_MICRO)
  int result;

  // This is declared this way to avoid warnings from
  // some compilers that complain about mismatching types
  // in the sscanf.
#if ACE_SIZEOF_VOID_P == ACE_SIZEOF_LONG_LONG
  ptrdiff_t address;
#else
  long int address;
#endif /* ACE_SIZEOF_VOID_P */
#endif /* CORBA_E_MICRO */

  // Make sure only one persistence option is specified
  int f_opt_used = 0;
  int u_opt_used = 0;
  int r_opt_used = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        ++TAO_debug_level;
        break;
      case 'o': // outputs the naming service ior to a file.
        this->iors_[ROOT].filename_ = get_opts.opt_arg ();
        break;
      case 'p':
        this->pid_file_name_ = get_opts.opt_arg ();
        break;
      case 's':
        size = ACE_OS::atoi (get_opts.opt_arg ());
        if (size >= 0)
          this->context_size_ = size;
        break;
      case 'm':
        this->multicast_ = ACE_OS::atoi(get_opts.opt_arg ());
        break;
#if !defined (CORBA_E_MICRO)
      case 'b':
        result = ::sscanf (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()),
#if ACE_SIZEOF_VOID_P == ACE_SIZEOF_LONG_LONG
                           ACE_INT64_FORMAT_SPECIFIER_ASCII,
#else
                           "%ld",
#endif /* ACE_SIZEOF_VOID_P */
                           &address);
        if (result == 0 || result == EOF)
          ORBSVCS_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-b> option"),
                            -1);
        this->base_address_ = (void *) address;
        break;
      case 'f':
        this->persistence_dir_ = get_opts.opt_arg ();
        f_opt_used = 1;
        break;
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)
      case 'r':
        this->use_redundancy_ = 1;
        this->use_storable_context_ = 1;
        this->persistence_dir_ = get_opts.opt_arg ();
        r_opt_used = 1;
        break;
      case 'u':
        this->use_storable_context_ = 1;
        this->persistence_dir_ = get_opts.opt_arg ();
        u_opt_used = 1;
        break;
#endif /* TAO_HAS_MINIMUM_POA == 0 */
#endif /* !CORBA_E_MICRO */
      case 'z':
        this->use_round_trip_timeout_ = 1;
        this->round_trip_timeout_ = (int)1.0e7 * ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
#if !defined (ACE_NLOGGING)
        const ACE_TCHAR *reqNonMinCorba=
#if !defined (CORBA_E_MICRO)
          ACE_TEXT ("-f <persistence_file_name> ")
#endif /* CORBA_E_MICRO */
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_MICRO)
          ACE_TEXT ("-u <storable_persistence_directory (not used with -f)> ")
          ACE_TEXT ("-r <redundant_persistence_directory> ");
#else
          ACE_TEXT ("");
#endif /* TAO_HAS_MINIMUM_POA && !CORBA_E_MICRO */
#endif /* !ACE_NLOGGING */
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-d ")
                           ACE_TEXT ("-o <ior_output_file> ")
                           ACE_TEXT ("-p <pid_file_name> ")
                           ACE_TEXT ("-s <context_size> ")
                           ACE_TEXT ("-b <base_address> ")
                           ACE_TEXT ("-u <persistence dir name> ")
                           ACE_TEXT ("-m <1=enable multicast, 0=disable multicast(default) ")
                           ACE_TEXT ("%s")
                           ACE_TEXT ("-z <relative round trip timeout> ")
                           ACE_TEXT ("\n"),
                           argv [0], reqNonMinCorba),
                          -1);
      }

  if (f_opt_used + u_opt_used + r_opt_used > 1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
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

  try
    {
      // Duplicate the ORB
      this->orb_ = CORBA::ORB::_duplicate (orb);

      // Get the POA from the ORB.
      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ORBSVCS_ERROR_RETURN ((LM_ERROR,
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
      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ());

      // Get the POA_Manager.
      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager ();

#if defined (CORBA_E_MICRO)
      this->ns_poa_ = PortableServer::POA::_duplicate (this->root_poa_);
#else
      int numPolicies = 2;
# if (TAO_HAS_MINIMUM_POA == 0)
      if (this->use_storable_context_)
        {
          this->use_servant_activator_ = true;
        }

      if (this->use_servant_activator_) {
        numPolicies += 2;
      }
# endif /* TAO_HAS_MINIMUM_POA */

      CORBA::PolicyList policies (numPolicies);
      policies.length (numPolicies);

      // Id Assignment policy
      policies[0] =
        this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);

      // Lifespan policy
      policies[1] =
        this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);

# if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)
      if (this->use_servant_activator_)
        {
          // Request Processing Policy
          policies[2] =
            this->root_poa_->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

          // Servant Retention Policy
          policies[3] =
            this->root_poa_->create_servant_retention_policy (PortableServer::RETAIN);
        }
# endif /* TAO_HAS_MINIMUM_POA */

      // We use a different POA, otherwise the user would have to change
      // the object key each time it invokes the server.
      this->ns_poa_ = this->root_poa_->create_POA ("NameService",
                                                   poa_manager.in (),
                                                   policies);
      // Warning!  If create_POA fails, then the policies won't be
      // destroyed and there will be hell to pay in memory leaks!

      // Creation of the new POAs over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy ();
        }
#endif /* CORBA_E_MICRO */

      poa_manager->activate ();

#if defined (CORBA_E_MICRO)
      result = this->init (orb,
                           this->ns_poa_.in (),
                           this->context_size_,
                           0,
                           0,
                           0,
                           0,
                           this->multicast_,
                           false,
                           this->round_trip_timeout_,
                           this->use_round_trip_timeout_);
#else
      result = this->init (orb,
                           this->ns_poa_.in (),
                           this->context_size_,
                           0,
                           0,
                           this->persistence_dir_,
                           this->base_address_,
                           this->multicast_,
                           this->use_storable_context_,
                           this->round_trip_timeout_,
                           this->use_round_trip_timeout_);
#endif /* CORBA_E_MICRO */

      if (result == -1)
        return result;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_Naming_Server::init_with_orb");
      return -1;
    }

  // If an ior file name was provided on command line
  if (this->write (size_t(ROOT)) != 0)
    {
      return -1;
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
  try
    {
      if (CORBA::is_nil (this->orb_.in ()))
        {
          this->orb_ = CORBA::ORB::_duplicate (orb);
        }

#if defined (CORBA_E_MICRO)
      ACE_UNUSED_ARG (persistence_location);
      ACE_UNUSED_ARG (base_addr);
      ACE_UNUSED_ARG (use_storable_context);
#else
      if (use_storable_context)
        {
          // In lieu of a fully implemented service configurator version
          // of this Reader and Writer, let's just take something off the
          // command line for now.
          TAO::Storable_Factory* pf = 0;
          ACE_CString directory (ACE_TEXT_ALWAYS_CHAR (persistence_location));
          ACE_NEW_RETURN (pf, TAO::Storable_FlatFileFactory (directory), -1);
          auto_ptr<TAO::Storable_Factory> persFactory(pf);

          // Use an auto_ptr to ensure that we clean up the factory in the case
          // of a failure in creating and registering the Activator.
          TAO_Storable_Naming_Context_Factory* cf =
            this->storable_naming_context_factory (context_size);
          // Make sure we got a factory
          if (cf == 0) return -1;
          auto_ptr<TAO_Storable_Naming_Context_Factory> contextFactory (cf);

          // This instance will either get deleted after recreate all or,
          // in the case of a servant activator's use, on destruction of the
          // activator.

          // Was a location specified?
          if (persistence_location == 0)
            {
              // No, assign the default location "NameService"
              persistence_location = ACE_TEXT ("NameService");
            }

          // Now make sure this directory exists
          if (ACE_OS::access (persistence_location, W_OK|X_OK))
            {
              ORBSVCS_ERROR_RETURN ((LM_ERROR,
                "Invalid persistence directory %C %p\n",
                persistence_location, "access"), -1);
            }

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)
          if (this->use_servant_activator_)
            {
              ACE_NEW_THROW_EX (this->servant_activator_,
                                TAO_Storable_Naming_Context_Activator (orb,
                                                                       persFactory.get(),
                                                                       contextFactory.get (),
                                                                       persistence_location),
                                CORBA::NO_MEMORY ());
              this->ns_poa_->set_servant_manager(this->servant_activator_);
            }
#endif /* TAO_HAS_MINIMUM_POA */
          try {  // The following might throw an exception.
            this->assign (size_t(ROOT), false, TAO_Storable_Naming_Context::
                                    recreate_all (orb,
                                                  poa,
                                                  TAO_ROOT_NAMING_CONTEXT,
                                                  context_size,
                                                  0,
                                                  contextFactory.get (),
                                                  persFactory.get (),
                                                  use_redundancy_));
          }
          catch (const CORBA::Exception& ex)
          {
            // The activator already took over the factories so we need to release the auto_ptr
            if (this->use_servant_activator_)
            {
              // The context factory is now owned by the activator
              // so we should release it
              contextFactory.release ();
              // If using a servant activator, the activator now owns the
              // factory, so we should release it
              persFactory.release ();
            }
            // Print out the exception and return failure
            ex._tao_print_exception (
              "TAO_Naming_Server::init_new_naming");
            return -1;
          }

        // Kind of a duplicate of the above here, but we must also release the
        // factory autoptrs in the good case as well.
        if (this->use_servant_activator_)
            {
              // The context factory is now owned by the activator
              // so we should release it
              contextFactory.release ();
              // If using a servant activator, the activator now owns the
              // factory, so we should release it
              persFactory.release ();
            }

      }
      else if (persistence_location != 0)
        //
        // Initialize Persistent Naming Service.
        //
        {

          // Create Naming Context Implementation Factory to be used for the creation of
          // naming contexts by the TAO_Persistent_Context_Index
          TAO_Persistent_Naming_Context_Factory *naming_context_factory =
            this->persistent_naming_context_factory ();
          // Make sure we got a factory.
          if (naming_context_factory == 0) return -1;

          // Allocate and initialize Persistent Context Index.
          ACE_NEW_RETURN (this->context_index_,
                          TAO_Persistent_Context_Index (orb, poa, naming_context_factory),
                          -1);

          if (this->context_index_->open (persistence_location,
                                          base_addr) == -1
              || this->context_index_->init (context_size) == -1)
            {
              if (TAO_debug_level >0)
                ORBSVCS_DEBUG ((LM_DEBUG,
                            "TAO_Naming_Server: context_index initialization failed\n"));
              return -1;
            }

          // Set the root Naming Context reference.
          this->assign (size_t(ROOT), false, this->context_index_->root_context ());
        }
      else
#endif /* CORBA_E_MICRO */
        {
          //
          // Initialize Transient Naming Service.
          //
          CosNaming::NamingContext_var new_context = TAO_Transient_Naming_Context::make_new_context (poa,
                                                            TAO_ROOT_NAMING_CONTEXT,
                                                            context_size);
          this->assign (size_t(ROOT), false, new_context.in());

        }

#if !defined (CORBA_E_MICRO)
      // Register with the ORB's resolve_initial_references()
      // mechanism.  Primarily useful for dynamically loaded Name
      // Services.
      orb->register_initial_reference ("NameService",
                                       this->iors_[ROOT].ref_.in ());
#endif /* CORBA_E_MICRO */

      CORBA::Object_var table_object =
        orb->resolve_initial_references ("IORTable");

      IORTable::Table_var ior_table =
        IORTable::Table::_narrow (table_object.in ());
      if (CORBA::is_nil (ior_table.in ()))
        {
          ORBSVCS_ERROR ((LM_ERROR, "Nil IORTable\n"));
        }
      else
        {
          ior_table->bind ("NameService",
                         this->iors_[ROOT].ior_.c_str());
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
              if (this->ior_multicast_->init (this->iors_[ROOT].ior_.c_str (),
                                              mde.c_str (),
                                              TAO_SERVICEID_NAMESERVICE) == -1)
                return -1;
            }
          else
            {
              if (this->ior_multicast_->init (this->iors_[ROOT].ior_.c_str (),
                                              port,
#if defined (ACE_HAS_IPV6)
                                              ACE_DEFAULT_MULTICASTV6_ADDR,
#else
                                              ACE_DEFAULT_MULTICAST_ADDR,
#endif /* ACE_HAS_IPV6 */
                                              TAO_SERVICEID_NAMESERVICE) == -1)
                return -1;
            }

          // Register event handler for the ior multicast.
          if (reactor->register_handler (this->ior_multicast_,
                                         ACE_Event_Handler::READ_MASK) == -1)
            {
              if (TAO_debug_level > 0)
                ORBSVCS_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("TAO_Naming_Server: cannot register Event handler\n")));
              return -1;
            }

          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO_Naming_Server: The multicast server setup is done.\n")));
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

        // set a timeout on the orb
        //
        CORBA::Object_var orbPolicyManagerObj =
          orb->resolve_initial_references ("ORBPolicyManager");

        CORBA::PolicyManager_var orbPolicyManager =
          CORBA::PolicyManager::_narrow (orbPolicyManagerObj.in ());
        orbPolicyManager->set_policy_overrides (polList, CORBA::SET_OVERRIDE);

        polList[0]->destroy ();
        polList[0] = CORBA::Policy::_nil ();
      }
#else
  ACE_UNUSED_ARG (use_round_trip_timeout);
  ACE_UNUSED_ARG (round_trip_timeout);
#endif /* TAO_HAS_CORBA_MESSAGING */
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_Naming_Server::init_new_naming");
      return -1;
    }

  return 0;
}

TAO_Storable_Naming_Context_Factory *
TAO_Naming_Server::storable_naming_context_factory (size_t context_size)
{
#if defined (ACE_HAS_NEW_NOTHROW)
  return new (ACE_nothrow) TAO_Storable_Naming_Context_Factory (context_size);
#else
  return new TAO_Storable_Naming_Context_Factory (context_size);
#endif /* ACE_HAS_NEW_NOTHROW */
}

TAO_Persistent_Naming_Context_Factory *
TAO_Naming_Server::persistent_naming_context_factory (void)
{
#if defined (ACE_HAS_NEW_NOTHROW)
  return new (ACE_nothrow) TAO_Persistent_Naming_Context_Factory;
#else
  return new TAO_Persistent_Naming_Context_Factory;
#endif /* ACE_HAS_NEW_NOTHROW */
}

int
TAO_Naming_Server::fini (void)
{
  // First get rid of the multi cast handler
  if (this->ior_multicast_)
    {
      orb_->orb_core()->reactor ()->remove_handler (this->ior_multicast_,
         ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);
      delete this->ior_multicast_;
      this->ior_multicast_ = 0;
    }

  for (size_t i = 0; i < bundle_size_; i++ )
    {
      this->iors_[i].ref_ = CORBA::Object::_nil();
    }

  // Destroy the child POA ns_poa that is created when initializing
  // the Naming Service
  try
    {
      if (!CORBA::is_nil (this->ns_poa_.in ()))
        this->ns_poa_->destroy (1, 1);

    }
  catch (const CORBA::Exception&)
    {
      // Ignore
    }
  try
    {
      CORBA::Object_var table_object =
        this->orb_->resolve_initial_references ("IORTable");

      IORTable::Table_var ior_table =
        IORTable::Table::_narrow (table_object.in ());

      if (!CORBA::is_nil (ior_table.in ()))
        {
          ior_table->unbind ("NameService");
        }

    }
  catch (const CORBA::Exception&)
    {
      // Ignore
    }

  try
    {
#if !defined (CORBA_E_MICRO)
      CORBA::Object_var svc =
        this->orb_->unregister_initial_reference ("NameService");
#endif /* CORBA_E_MICRO */

    }
  catch (const CORBA::Exception&)
    {
      // Ignore
    }

  ns_poa_ = PortableServer::POA::_nil ();
  root_poa_ = PortableServer::POA::_nil ();
  orb_ = CORBA::ORB::_nil ();

#if !defined (CORBA_E_MICRO)
  delete this->context_index_;
#endif /* CORBA_E_MICRO */

  return 0;
}

char*
TAO_Naming_Server::naming_service_ior (void)
{
  return CORBA::string_dup (this->iors_[ROOT].ior_.c_str());
}

CosNaming::NamingContext_ptr
TAO_Naming_Server::operator-> (void) const
{
  if (CORBA::is_nil (this->iors_[ROOT].ref_))
    {
      return CosNaming::NamingContext::_nil ();
    }

  CosNaming::NamingContext_ptr ret =
    CosNaming::NamingContext::_narrow (this->iors_[ROOT].ref_);

  return ret;
}


TAO_Naming_Server::~TAO_Naming_Server (void)
{
  delete [] this->iors_;

#if (TAO_HAS_MINIMUM_POA == 0) && \
    !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  if (this->use_servant_activator_ &&
      this->servant_activator_)
    {
      // Activator is reference counted. Don't delete it directly.
      this->servant_activator_->_remove_ref ();
    }
#endif /* TAO_HAS_MINIMUM_POA */
}


TAO_Naming_Server::IOR_Bundle *
TAO_Naming_Server::bundle_at(size_t ndx)
  {
  if (ndx >= this->bundle_size_)
    {
    if (TAO_debug_level)
      {
      ORBSVCS_ERROR((LM_ERROR,
        ACE_TEXT("(%P|%t) TAO_naming_Server::bundle_at, ")
        ACE_TEXT("index %d out of range\n"),
        ndx));
      }
    return 0;
    }
  return &iors_[ndx];
  }

const TAO_Naming_Server::IOR_Bundle *
TAO_Naming_Server::bundle_at(size_t ndx) const
  {
  if (ndx >= this->bundle_size_)
    {
    if (TAO_debug_level)
      {
      ORBSVCS_ERROR((LM_ERROR,
        ACE_TEXT("(%P|%t) const TAO_naming_Server::bundle_at, ")
        ACE_TEXT("index %d out of range\n"),
        ndx));
      }
    return 0;
    }
  return &iors_[ndx];
  }



int
TAO_Naming_Server::write(size_t ndx) const
{
  const IOR_Bundle *b = bundle_at(ndx);
  if (b == 0)
    return -1;

  if (b->filename_.length() == 0)
    return 0;

  if (b->ior_.length() == 0)
    {
      if (TAO_debug_level > 0)
        {
          ORBSVCS_ERROR ((LM_ERROR,
                         ACE_TEXT ("IOR_Bundle::write called with ")
                          ACE_TEXT ("empty IOR \n")));
        }
      return -1;
    }

  FILE *iorf = ACE_OS::fopen (b->filename_.c_str(), ACE_TEXT("w"));
  if (iorf == 0)
    {
      if (TAO_debug_level > 0)
        {
          ORBSVCS_ERROR ((LM_ERROR,
                          ACE_TEXT("Unable to open %C for writing:(%u) %p\n"),
                          b->filename_.c_str(),
                          ACE_ERRNO_GET,
                          ACE_TEXT("IOR_Bundle::write")));
        }
      return -1;
    }
  ACE_OS::fprintf (iorf, "%s\n", b->ior_.c_str());
  ACE_OS::fclose (iorf);

  return 0;
}

void
TAO_Naming_Server::assign (size_t ndx, bool take, CORBA::Object_ptr obj)
{
  IOR_Bundle *b = this->bundle_at(ndx);
  if (b == 0)
    return;

  b->ref_ = take ? obj : CORBA::Object::_duplicate (obj);
  CORBA::String_var ior = this->orb_->object_to_string (b->ref_.in());
  b->ior_ = ior.in();
}


TAO_END_VERSIONED_NAMESPACE_DECL
