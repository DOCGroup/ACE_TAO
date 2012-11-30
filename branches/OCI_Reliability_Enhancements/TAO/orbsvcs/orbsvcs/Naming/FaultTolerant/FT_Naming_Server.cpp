// -*- C++ -*-

//=============================================================================
/**
 * @file   FT_Naming_Server.cpp
 *
 * $Id$
 *
 * @author Kevin Stanley <stanleyk@ociweb.com>
 */
//=============================================================================


#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Server.h"
#include "orbsvcs/Naming/Naming_Server.h"

#include "orbsvcs/Naming/Flat_File_Persistence.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Manager.h"
#include "orbsvcs/Naming/Storable.h"
#include "orbsvcs/Naming/Storable_Naming_Context.h"
#include "orbsvcs/Naming/Storable_Naming_Context_Activator.h"

#include "orbsvcs/Naming/FaultTolerant/FT_Storable_Naming_Context.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Storable_Naming_Context_Factory.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Persistent_Naming_Context_Factory.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Persistent_Naming_Context.h"
#include "orbsvcs/Naming/Persistent_Context_Index.h"
#include "orbsvcs/Naming/Naming_Context_Interface.h"


#include "ace/Arg_Shifter.h"
#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

#include "tao/IORTable/IORTable.h"
#include "tao/ORB_Core.h"

#include "tao/debug.h"
#include "tao/default_ports.h"
#include "tao/Storable_FlatFileStream.h"

#include "orbsvcs/CosNamingC.h"

#include "tao/debug.h"
#include "tao/default_ports.h"

#include "tao/IORTable/IORTable.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
#include "tao/Messaging/Messaging.h"
#endif

#include "tao/AnyTypeCode/Any.h"

#include "ace/Arg_Shifter.h"
#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"


/// Default Constructor.
TAO_FT_Naming_Server::TAO_FT_Naming_Server (void)
  : replica_id_(0),
    naming_manager_ (),
    replication_manager_ (0),
    naming_manager_ior_file_name_(0),
    replication_manager_ior_file_name_(0),
    naming_manager_persistence_file_name_(0),
    use_object_group_persistence_(0)
{
}

int
TAO_FT_Naming_Server::init_with_orb (int argc,
                                     ACE_TCHAR *argv [],
                                     CORBA::ORB_ptr orb)
{
  // Invoke the base class initialization to setup the naming service
  // What follows after that are the initialization steps to support
  // fault tolerance and load balancing with the FT_Naming_Manager
  int result = TAO_Naming_Server::init_with_orb (argc, argv, orb);

  // Check the result to make sure it executed Ok.
  if (result != 0)
    return result;

  result = this->init_naming_manager_with_orb (argc, argv, orb);
  if (result != 0)
    return result;

  try {

    // Initialize the replication manager
    result = init_replication_manager_with_orb(argc, argv, orb);
    if (result != 0)
      return result;
  }
  catch (CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      "TAO_FT_Naming_Server::init_with_orb");
    return -1;
  }
  return 0;
}


int
TAO_FT_Naming_Server::init_naming_manager_with_orb (int argc, ACE_TCHAR *argv [], CORBA::ORB_ptr orb)
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  int result = 0;

  try {

    // Get the POA from the ORB.
    CORBA::Object_var poa_object =
      orb->resolve_initial_references ("RootPOA");

    if (CORBA::is_nil (poa_object.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT(" (%P|%t) Unable to initialize the POA.\n")),
        -1);
    }

    if (result != 0)
      return result;

    // Get the POA object.
    this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ());

    // Get the POA_Manager.
    PortableServer::POAManager_var poa_manager =
      this->root_poa_->the_POAManager ();

    int numPolicies = 2;

    CORBA::PolicyList policies (numPolicies);
    policies.length (numPolicies);

    // Id Assignment policy
    policies[0] =
      this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);

    // Lifespan policy
    policies[1] =
      this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);

    /* Register the naming manager with a POA
    * TODO: 1) Error checking
    *       2) Write IOR to file
    *       3) Persistence for Object Group Manager
    */

    // We use a different POA, otherwise the user would have to change
    // the object key each time it invokes the server.
    this->naming_manager_poa_ = this->root_poa_->create_POA ("NamingManager",
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

    poa_manager->activate ();

    // Register with the POA.
    PortableServer::ObjectId_var id =
      PortableServer::string_to_ObjectId ("NamingManager");

    this->naming_manager_poa_->activate_object_with_id (id.in (),
      &this->naming_manager_);

    this->naming_manager_ior_ =
      orb->object_to_string (this->naming_manager_poa_->id_to_reference (id.in ()));

    this->naming_manager_.initialize (this->orb_,
      this->naming_manager_poa_);

  }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_FT_Naming_Server::init_naming_manager_with_orb");
      return -1;
    }

  if (this->naming_manager_ior_file_name_ != 0)
    {
      FILE *iorf = ACE_OS::fopen (this->naming_manager_ior_file_name_, ACE_TEXT("w"));
      if (iorf == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("Unable to open %s for writing:(%u) %p\n"),
                             this->naming_manager_ior_file_name_,
                             ACE_ERRNO_GET,
                             ACE_TEXT("TAO_FT_Naming_Server::init_naming_manager_with_orb")),
                            -1);
        }
      CORBA::String_var str = this->naming_manager_ior ();
      ACE_OS::fprintf (iorf, "%s\n", str.in ());
      ACE_OS::fclose (iorf);
    }

  // Make the Object Group Manager easily accessible using Interoperable Naming Service IORs
  CORBA::Object_var table_object =
    orb->resolve_initial_references ("IORTable");

  IORTable::Table_var adapter =
    IORTable::Table::_narrow (table_object.in ());
  if (CORBA::is_nil (adapter.in ()))
  {
    ACE_ERROR ((LM_ERROR, "TAO_FT_Naming_Server::init_naming_manager_with_orb - Nil IORTable\n"));
  }
  else
  {
    CORBA::String_var ior = this->naming_manager_ior ();
    adapter->bind ("ObjectGroupManager", ior.in ());
  }
  return 0;
}

int
TAO_FT_Naming_Server::init_replication_manager_with_orb (int argc, ACE_TCHAR *argv [], CORBA::ORB_ptr orb)
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  int result = 0;

  try {

    // Get the POA from the ORB.
    CORBA::Object_var poa_object =
      orb->resolve_initial_references ("RootPOA");

    if (CORBA::is_nil (poa_object.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT(" (%P|%t) Unable to initialize the POA.\n")),
        -1);
    }

    if (result != 0)
      return result;

    // Get the POA object.
    this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ());

    // Get the POA_Manager.
    PortableServer::POAManager_var poa_manager =
      this->root_poa_->the_POAManager ();

    int numPolicies = 2;

    CORBA::PolicyList policies (numPolicies);
    policies.length (numPolicies);

    // Id Assignment policy
    policies[0] =
      this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);

    // Lifespan policy
    policies[1] =
      this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);

    /* Register the naming manager with a POA
    * TODO: 1) Error checking
    *       2) Write IOR to file
    *       3) Persistence for Object Group Manager
    */

    if (this->replica_id_ == 0)
    {
      // Create the default replica name
      this->replica_id_ = ACE_OS::strdup ("Replica_P");
#if 0
    ACE_ERROR_RETURN ((LM_ERROR,
                      "No replica id provided.\n"),
                      -1);
#endif
    }

    // We use a different POA, otherwise the user would have to change
    // the object key each time it invokes the server.
    this->replication_manager_poa_ = this->root_poa_->create_POA (this->replica_id_,
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

    poa_manager->activate ();

    // Construct the replication manager providing it with its ID
    ACE_NEW_RETURN (this->replication_manager_,
                    TAO_FT_Naming_Replication_Manager (this,
                                                       this->replica_id_),
                    -1);

    // Register with the POA.
    PortableServer::ObjectId_var id =
      PortableServer::string_to_ObjectId (this->replica_id_);

    this->replication_manager_poa_->activate_object_with_id (id.in (),
      this->replication_manager_);

    this->naming_manager_ior_ =
      orb->object_to_string (
        this->replication_manager_poa_->id_to_reference (id.in ()));

    // Provide the replication manager its ORB and POA
    this->replication_manager_->initialize (this->orb_,
                                            this->replication_manager_poa_);

    CORBA::Object_var table_object =
      orb->resolve_initial_references ("IORTable");

    IORTable::Table_var adapter =
      IORTable::Table::_narrow (table_object.in ());
    if (CORBA::is_nil (adapter.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Replication Manager init - Nil IORTable\n"));
    }
    else
    {
      adapter->bind (this->replica_id_, this->naming_manager_ior_.in ());
    }

    if (this->replica_peer_ior_ != 0)
    { // A peer replica was provided. Provide the reference to the
      // ReplicationManager
      CORBA::Object_var peer_obj =
        orb->string_to_object (this->replica_peer_ior_.in ());
      if (CORBA::is_nil (peer_obj.in ()))
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("invalid ior <%s>\n"),
                                 this->replica_peer_ior_.in ()),
                                -1);
      FT_Naming::ReplicationManager_var peer_ref =
        FT_Naming::ReplicationManager::_narrow (peer_obj.in ());

      if (CORBA::is_nil (peer_ref.in ()))
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("IOR is not a FT_Naming::ReplicationManager <%s>\n"),
                                 this->replica_peer_ior_.in ()),
                                -1);

      int registration_result = this->replication_manager_->register_with_peer_replica (peer_ref.in ());
      if (registration_result != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                          ACE_TEXT ("Unable to register with peer <%s>\n"),
                          this->replica_peer_ior_.in ()),
                          -1);
    }

  }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_FT_Naming_Server::init_replication_manager_with_orb.\n");
      return -1;
    }

  if (this->replication_manager_ior_ != 0)
    {
      FILE *iorf = ACE_OS::fopen (this->replication_manager_ior_file_name_, ACE_TEXT("w"));
      if (iorf == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("Unable to open %s for writing:(%u) %p\n"),
                             this->replication_manager_ior_file_name_,
                             ACE_ERRNO_GET,
                             ACE_TEXT("TAO_FT_Naming_Server::init_replication_manager_with_orb")),
                            -1);
        }

      CORBA::String_var str = this->naming_manager_ior ();

      ACE_OS::fprintf (iorf, "%s\n", str.in ());
      ACE_OS::fclose (iorf);
    }

  return 0;
}


int
TAO_FT_Naming_Server::parse_args (int argc,
                                  ACE_TCHAR *argv[])
{
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:do:p:s:f:m:z:r:u:v:g:i:j:"));
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

  // Make sure only one naming context persistence option is specified
  int f_opt_used = 0;
  int u_opt_used = 0;
  int r_opt_used = 0;

  // TODO: remove unsupported options with FT Naming Server
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        ++TAO_debug_level;
        break;
      case 'o': // outputs the naming service ior to a file.
        this->ior_file_name_ = get_opts.opt_arg ();
        break;
      case 'g': // outputs the object group manager ior to a file
        this->naming_manager_ior_file_name_ = get_opts.opt_arg ();
        break;
      case 'p':
        this->pid_file_name_ = get_opts.opt_arg ();
        break;
      case 'i':
        this->replica_id_ = get_opts.opt_arg ();
        break;
      case 'j':
        this->replica_peer_ior_ = get_opts.opt_arg ();
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
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-b> option"),
                            -1);
        this->base_address_ = (void *) address;
        break;
      case 'f':
        this->persistence_file_name_ = get_opts.opt_arg ();
        f_opt_used = 1;
        break;
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)
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
      case 'v':
        this->use_object_group_persistence_ = 1;
        this->object_group_dir_ = get_opts.opt_arg ();
        break;

#endif /* TAO_HAS_MINIMUM_POA == 0 */
#endif /* !CORBA_E_MICRO */
      case 'z':
        this->use_round_trip_timeout_ = 1;
        this->round_trip_timeout_ = (int)1.0e7 * ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'n':  // the file name to same the object group persistence info
        this->pid_file_name_ = get_opts.opt_arg ();
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
          ACE_TEXT ("-v <storable_object_group_persistence_directory> ")
          ACE_TEXT ("-r <redundant_persistence_directory> ");
#else
          ACE_TEXT ("");
#endif /* TAO_HAS_MINIMUM_POA && !CORBA_E_MICRO */
#endif /* !ACE_NLOGGING */
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-d ")
                           ACE_TEXT ("-o <name_svc_ior_output_file> ")
                           ACE_TEXT ("-g <naming_mgr_ior_output_file> ")
                           ACE_TEXT ("-i <replica_id> ")
                           ACE_TEXT ("-j <peer_replica_ior> ")
                           ACE_TEXT ("-p <pid_file_name> ")
                           ACE_TEXT ("-s <context_size> ")
                           ACE_TEXT ("-b <base_address> ")
                           ACE_TEXT ("-m <1=enable multicast, 0=disable multicast(default) ")
                           ACE_TEXT ("%s")
                           ACE_TEXT ("-z <relative round trip timeout> ")
                           ACE_TEXT ("\n"),
                           argv [0], reqNonMinCorba),
                          -1);
      }


  if (f_opt_used + u_opt_used + r_opt_used > 1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Only one persistence option can be passed")
                       ACE_TEXT ("\n")),
                      -1);

  return 0;
}


// TODO: Refactor to allow the implementation from TAO_Naming_Server to be used.
// Need to split into separate inits for different naming service persistence modes
int
TAO_FT_Naming_Server::init_new_naming (CORBA::ORB_ptr orb,
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
#if defined (CORBA_E_MICRO)
      ACE_UNUSED_ARG (persistence_location);
      ACE_UNUSED_ARG (base_addr);
      ACE_UNUSED_ARG (use_storable_context);
#else
      if (this->use_object_group_persistence_)
        {
          // Make sure the object group directory is accessible
          if (ACE_OS::access (this->object_group_dir_.c_str (), W_OK|X_OK))
            {
              ACE_ERROR_RETURN ((LM_ERROR, "Invalid object group persistence directory\n"), -1);
            }

          TAO::Storable_Factory * object_group_storable_factory;
          ACE_NEW_RETURN (object_group_storable_factory,
                          TAO::Storable_FlatFileFactory (this->object_group_dir_), -1);

          naming_manager_.set_object_group_storable_factory(object_group_storable_factory);
        }

      if (use_storable_context)
        {
          // In lieu of a fully implemented service configurator version
          // of this Reader and Writer, let's just take something off the
          // command line for now.
          TAO_Naming_Service_Persistence_Factory* pf = 0;
          ACE_NEW_RETURN(pf, TAO_NS_FlatFileFactory, -1);
          auto_ptr<TAO_Naming_Service_Persistence_Factory> persFactory(pf);

          // Use an auto_ptr to ensure that we clean up the factory in the case
          // of a failure in creating and registering the Activator
          TAO_FT_Storable_Naming_Context_Factory* cf = 0;
          ACE_NEW_RETURN (cf, TAO_FT_Storable_Naming_Context_Factory (context_size), -1);
          auto_ptr<TAO_FT_Storable_Naming_Context_Factory> contextFactory (cf);

          // Provide the naming manager reference for use in
          // TAO_FT_Persistent_Naming_Contexts for load balancing functionality
          TAO_FT_Storable_Naming_Context::set_naming_manager (&naming_manager_);

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
              ACE_ERROR_RETURN ((LM_ERROR, "Invalid persistence directory\n"), -1);
            }

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)
          if (this->use_servant_activator_)
            {
              ACE_NEW_THROW_EX (this->servant_activator_,
                                TAO_Storable_Naming_Context_Activator (orb,
                                                                       persFactory.get (),
                                                                       contextFactory.get (),
                                                                       persistence_location),
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
                                                       contextFactory.get (),
                                                       persFactory.get (),
                                                       persistence_location,
                                                       use_redundancy_);

          // We have successfull turned over the context factory to the activator so
          // we can now release it.
          contextFactory.release ();

          if (this->use_servant_activator_)
            persFactory.release();

        }
      else if (persistence_location != 0)
        //
        // Initialize Persistent Naming Service.
        //
        {
          // Create a factory for Fault Tolerant / Persistent Naming Contexts and use it
          TAO_Naming_Context_Factory *naming_context_factory = 0;
          ACE_NEW_RETURN (naming_context_factory, TAO_FT_Persistent_Naming_Context_Factory, -1);

          // Provide the naming manager reference for use in
          // TAO_FT_Persistent_Naming_Contexts for load balancing functionality
          TAO_FT_Persistent_Naming_Context::set_naming_manager_impl (&naming_manager_);

          // Allocate and initialize Persistent Context Index.
          ACE_NEW_RETURN (this->context_index_,
            TAO_Persistent_Context_Index (orb, poa, naming_context_factory),
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
#endif /* CORBA_E_MICRO */
        {
          // This option is not supported by the FT_Naming_Server at this time.
          // Should return an error.
          ACE_ERROR ((LM_ERROR, "Options not supported with FT_Naming Service.\n"));
          return -1;

        }

#if !defined (CORBA_E_MICRO)
      // Register with the ORB's resolve_initial_references()
      // mechanism.  Primarily useful for dynamically loaded Name
      // Services.
      orb->register_initial_reference ("NameService",
                                       this->naming_context_.in ());
#endif /* CORBA_E_MICRO */

      // Set the ior of the root Naming Context.
      this->naming_service_ior_=
        orb->object_to_string (this->naming_context_.in ());

      CORBA::Object_var table_object =
        orb->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in ());
      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
        }
      else
        {
          CORBA::String_var ior =
            orb->object_to_string (this->naming_context_.in ());
          adapter->bind ("NameService", ior.in ());
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

/// Shut down the TAO_FT_Naming_Service; you must still call fini().
void
TAO_FT_Naming_Server::shutdown (void)
{

}

/// Return the IOR for the registered object group manager
char*
TAO_FT_Naming_Server::naming_manager_ior (void)
{
  return CORBA::string_dup (this->naming_manager_ior_.in ());
}

int
TAO_FT_Naming_Server::update_object_group (
    const FT_Naming::ObjectGroupUpdate & group_info)
{
  ACE_UNUSED_ARG (group_info);

  // TODO: Update or mark the affected object group that is specified
  return 0;
}

int
TAO_FT_Naming_Server::update_naming_context (
    const FT_Naming::NamingContextUpdate & context_info)
{
  PortableServer::ServantBase_var servant;

  // Lookup the servant for the identified context and see if it is
  // active here locally.
  try {
    // Get the servant if it exists in this process
    PortableServer::ObjectId_var context_id =
      PortableServer::string_to_ObjectId (context_info.context_name);
    servant = this->ns_poa_->id_to_servant (context_id);
  }
  catch (PortableServer::POA::ObjectNotActive&)
  { // No servant registered for this object reference so no need to create it.
    // It will be created on first access in incarnate function
    ACE_DEBUG ((LM_DEBUG,
               "Context with unknown servant. name = %s, Change type = %i\n",
               context_info.context_name.in (), context_info.change_type));
    // Nothing to be done, so return success
    return 0;
  }

  TAO_Naming_Context* changed_context_servant = dynamic_cast<TAO_Naming_Context*> (servant.in ());

  if (changed_context_servant == 0)
  { // Another type of class was used as the servant. Should not happen.
    ACE_ERROR ((LM_ERROR,
               "Invalid servant type registered with oid: %s", context_info.context_name.in ()));
    return -1;
  }

  // Print out a helpful message
  CORBA::String_var changed_context = changed_context_servant->to_string (context_info.changed_context);
  ACE_DEBUG ((LM_DEBUG,
              "Updated Context: name = %s, path = %s, type = %i\n",
               context_info.context_name.in (), changed_context.in (), context_info.change_type));

  changed_context_servant->mark_dirty ();

  // Must remove the reference to this servant.
  //servant->_remove_ref ();
  return 0;
}


/// Destructor.
TAO_FT_Naming_Server::~TAO_FT_Naming_Server (void)
{
  // Clear out the static naming manager from the persistent naming context
  TAO_FT_Persistent_Naming_Context::set_naming_manager_impl (0);
}
