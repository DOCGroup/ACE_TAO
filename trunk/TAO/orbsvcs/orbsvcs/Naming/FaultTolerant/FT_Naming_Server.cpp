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
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

#include "tao/IORTable/IORTable.h"
#include "tao/ORB_Core.h"

#include "tao/debug.h"
#include "tao/default_ports.h"
#include "tao/Storable_FlatFileStream.h"

#include "tao/debug.h"
#include "tao/default_ports.h"

#include "tao/IORManipulation/IORManip_Loader.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
#include "tao/Messaging/Messaging.h"
#endif

#include "tao/AnyTypeCode/Any.h"

const ACE_TCHAR*
TAO_FT_Naming_Server::primary_replica_ior_filename =
  ACE_TEXT ("ns_replica_primary.ior");

const ACE_TCHAR*
TAO_FT_Naming_Server::backup_replica_ior_filename =
  ACE_TEXT ("ns_replica_backup.ior");

/// Default Constructor.
TAO_FT_Naming_Server::TAO_FT_Naming_Server (void)
  : replica_id_ (0),
    naming_manager_ (),
    replication_manager_ (0),
    combined_naming_service_ior_file_name_ (0),
    combined_naming_manager_ior_file_name_ (0),
    naming_manager_ior_file_name_ (0),
    naming_manager_persistence_file_name_ (0),
    use_object_group_persistence_ (0),
    server_role_ (STANDALONE)
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

  if (this->use_object_group_persistence_)
    {
      // Make sure the object group directory is accessible
      if (ACE_OS::access (this->object_group_dir_.c_str (), W_OK|X_OK))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) ERROR: Invalid object ")
                             ACE_TEXT ("group persistence directory\n")),
                            -1);
        }

      TAO::Storable_Factory * object_group_storable_factory;
      ACE_NEW_RETURN (object_group_storable_factory,
                      TAO::Storable_FlatFileFactory (this->object_group_dir_),
                      -1);

      naming_manager_.set_object_group_storable_factory (
        object_group_storable_factory);
    }

  // Provide the naming manager reference for use in
  // TAO_FT_Persistent_Naming_Contexts for load balancing functionality
  TAO_FT_Storable_Naming_Context::set_naming_manager (&naming_manager_);

  // Initialize the naming manager which supports the Object Group Manager
  // interface
  result = this->init_naming_manager_with_orb (argc, argv, orb);
  if (result != 0)
    return result;

  try {

    // Initialize the replication manager
    result = init_replication_manager_with_orb (argc, argv, orb);
    if (result != 0)
      return result;
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("TAO_FT_Naming_Server::init_with_orb"));
    return -1;
  }

  // If we successfully initialized the replication manager and we are
  // a backup server, then we should export the multi-profile
  // references to files.
  if (this->server_role_ == TAO_FT_Naming_Server::BACKUP)
    {
      // The backup should write out the combined IOR for the primary
      // and backup naming service and naming manager.
      result = export_ft_naming_references ();
    }

  return result;
}

int
TAO_FT_Naming_Server::init_naming_manager_with_orb (int argc,
                                                    ACE_TCHAR *argv [],
                                                    CORBA::ORB_ptr orb)
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  int result = 0;

  // Need to lock during startup to prevent access of partially
  // initialized variables
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  try {

    // Get the POA from the ORB.
    CORBA::Object_var poa_object =
      orb->resolve_initial_references ("RootPOA");

    if (CORBA::is_nil (poa_object.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT(" (%P|%t) ERROR: Unable to initialize the POA.\n")),
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
    this->naming_manager_poa_ = this->root_poa_->create_POA (
      "NamingManager",
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
      PortableServer::string_to_ObjectId (
         "NamingManager");

    this->naming_manager_poa_->activate_object_with_id (
       id.in (),
       &this->naming_manager_);

    CORBA::Object_var nm_obj =
      this->naming_manager_poa_->id_to_reference (id.in ());

    this->my_naming_manager_ = FT_Naming::NamingManager::_narrow (nm_obj.in ());

    this->naming_manager_ior_ =
      orb->object_to_string (this->my_naming_manager_.in ());

  // write out our object reference to the file defined in the -h option
  if (this->naming_manager_ior_file_name_ != 0)
    {
      if (this->write_ior_to_file (
            this->naming_manager_ior_.in (),
            ACE_TEXT_ALWAYS_CHAR (this->naming_manager_ior_file_name_)) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) ERROR: Unable to open %s ")
                             ACE_TEXT ("for writing:(%u) %p\n"),
                             this->naming_manager_ior_file_name_,
                             ACE_ERRNO_GET,
                             ACE_TEXT ("TAO_Naming_Server::")
                             ACE_TEXT ("init_naming_manager_with_orb")),
                            -1);
        }
    }

    this->naming_manager_.initialize (this->orb_,
      this->naming_manager_poa_);

  }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("TAO_FT_Naming_Server::init_naming_manager_with_orb"));
      return -1;
    }

  // Make the Object Group Manager easily accessible using Interoperable
  // Naming Service IORs
  CORBA::Object_var table_object =
    orb->resolve_initial_references ("IORTable");

  IORTable::Table_var adapter =
    IORTable::Table::_narrow (table_object.in ());
  if (CORBA::is_nil (adapter.in ()))
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) ERROR: TAO_FT_Naming_Server::")
                ACE_TEXT ("init_naming_manager_with_orb - Nil IORTable\n")));
  }
  else
  {
    CORBA::String_var ior = this->naming_manager_ior ();
    adapter->bind ("NamingManager", ior.in ());
  }

  return 0;
}

int
TAO_FT_Naming_Server::init_replication_manager_with_orb (int argc,
                                                         ACE_TCHAR *argv [],
                                                         CORBA::ORB_ptr orb)
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  // Need to lock during startup to prevent access of partially initialized
  // variables
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // If redundancy is not requested, then do not initialize the
  // replication manager
  if (!this->use_redundancy_)
    return 0;

  int result = 0;

  try {

    // Get the POA from the ORB.
    CORBA::Object_var poa_object =
      orb->resolve_initial_references ("RootPOA");

    if (CORBA::is_nil (poa_object.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT(" (%P|%t) ERROR: Unable to initialize the POA.\n")),
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

    // We use a different POA, otherwise the user would have to change
    // the object key each time it invokes the server.
    this->replication_manager_poa_ = this->root_poa_->create_POA (
      ACE_TEXT_ALWAYS_CHAR (this->replica_id_ ),
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
                    TAO_FT_Naming_Replication_Manager (
                      this,
                      ACE_TEXT_ALWAYS_CHAR (this->replica_id_)),
                    -1);

    // Register with the POA.
    PortableServer::ObjectId_var id =
      PortableServer::string_to_ObjectId (
        ACE_TEXT_ALWAYS_CHAR (this->replica_id_));

    this->replication_manager_poa_->activate_object_with_id (
      id.in (),
      this->replication_manager_);

    CORBA::Object_var repl_mgr_ref =
      this->replication_manager_poa_->id_to_reference (id.in ());

    this->replication_manager_ior_ =
      orb->object_to_string (repl_mgr_ref.in ());

    // Provide the replication manager its ORB and POA
    this->replication_manager_->initialize (
      this->orb_.in (),
      this->replication_manager_poa_.in ());

    ACE_CString primary_file_name (
      ACE_TEXT_ALWAYS_CHAR (this->persistence_file_name_));
    primary_file_name += "/";
    primary_file_name +=
      ACE_TEXT_ALWAYS_CHAR (TAO_FT_Naming_Server::primary_replica_ior_filename);

    ACE_CString backup_file_name (
      ACE_TEXT_ALWAYS_CHAR (this->persistence_file_name_));
    backup_file_name += "/";
    backup_file_name +=
      ACE_TEXT_ALWAYS_CHAR (TAO_FT_Naming_Server::backup_replica_ior_filename);

    if (this->server_role_ == PRIMARY)
      { // We are the primary
        if (TAO_debug_level > 3)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) - FT_Naming_Server ")
                      ACE_TEXT ("is a primary\n")));

        // Write out this replicas IOR for the backup to use to bootstrap
        CORBA::String_var replication_ior = naming_service_ior ();
        this->write_ior_to_file (
          this->replication_manager_ior_.in (),
          primary_file_name.c_str ());

        // Check if there is already a backup IOR file. If so, then the backup
        // may be up and running so we should register with it.
        CORBA::Object_var backup_ior;
        if (TAO_debug_level > 3)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) - FT_Naming_Server reading ")
                      ACE_TEXT ("backup ior file\n")));

        if ((ACE_OS::access (primary_file_name.c_str (),
                             R_OK) == 0) &&
            this->read_reference_from_file (backup_file_name.c_str (),
                                            backup_ior.out ()) == 0)
          {// Success in reading backup IOR file
            // Store the backup reference as our peer
            FT_Naming::ReplicationManager_var peer_ref =
              FT_Naming::ReplicationManager::_narrow (backup_ior.in ());

            if (TAO_debug_level > 3)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) - FT_Naming_Server ")
                          ACE_TEXT ("narrowing IOR\n")));
            if (CORBA::is_nil (peer_ref.in ()))
              ACE_ERROR_RETURN (
                (LM_ERROR,
                 ACE_TEXT ("(%P|%t) ERROR: IOR in file %s is not ")
                 ACE_TEXT ("a FT_Naming::ReplicationManager\n"),
                 primary_file_name.c_str ()),
                -1);

            try {
              if (TAO_debug_level > 3)
                ACE_DEBUG (
                  (LM_DEBUG,
                   ACE_TEXT ("(%P|%t) - FT_Naming_Server registering ")
                   ACE_TEXT ("with backup.\n")));

              // Register with the backup
              CosNaming::NamingContext_var root = this->my_root_context ();
              FT_Naming::NamingManager_var nm = this->my_naming_manager ();

              int registration_result =
                this->replication_manager_->register_with_peer_replica (
                   peer_ref.in (),
                   root.in (),
                   nm.in ());

              if (registration_result == 0)
                {
                  if (TAO_debug_level > 3)
                    ACE_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("(%P|%t) - FT_Naming_Server ")
                                ACE_TEXT ("registered with backup.\n")));
                }
              else
                {
                  if (TAO_debug_level > 3)
                    ACE_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("(%P|%t) - FT_Naming_Server:Backup peer ")
                                ACE_TEXT ("replica not started yet.\n")));
                }
            }
            catch (const CORBA::Exception& ex)
              {
                // Its Ok that we were unable to contact the backup peer.
                // It has apparently not started yet.
                // It will register with the primary when it starts up.
                ex._tao_print_exception (
                   ACE_TEXT ("Backup peer replica not started yet.\n"));
              }
          }
        else
          {
            // Could not get the backup replica from the IOR file, which is OK.
            // The backup will register with us in the future.
            if (TAO_debug_level > 3)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) - FT_Naming_Server no Replica ")
                          ACE_TEXT ("IOR file. Waiting for registration.\n")));
          }
      }
    else if (this->server_role_ == TAO_FT_Naming_Server::BACKUP)
      { // We are the backup
        if (TAO_debug_level > 3)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) - FT_Naming_Server:Is a Backup\n")));

        if (TAO_debug_level > 3)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) - FT_Naming_Server writing ")
                      ACE_TEXT ("replica ior\n")));
        // Write out the backup ior for use by the primary if it must be restarted.
        this->write_ior_to_file (
          replication_manager_ior_.in (),
          backup_file_name.c_str ());

        // Get the ior file for the primary from the
        // persistence directory. If not there, fail.
        CORBA::Object_var primary_ref = CORBA::Object::_nil ();

        if (TAO_debug_level > 3)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) - FT_Naming_Server ")
                      ACE_TEXT ("reading primary ior file\n")));
        // Check for the primary IOR.  We must have it to bootstrap the redundant
        // naming pair.
        if ((ACE_OS::access (primary_file_name.c_str (), R_OK) == 0) &&
            (this->read_reference_from_file (primary_file_name.c_str (),
                                             primary_ref.out ()) == 0))
          {
            if (TAO_debug_level > 3)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) - FT_Naming_Server ")
                          ACE_TEXT ("toring the primary reference ior\n")));
            // Store the primary reference as our peer
            FT_Naming::ReplicationManager_var peer_ref =
              FT_Naming::ReplicationManager::_narrow (primary_ref.in ());

            if (CORBA::is_nil (peer_ref.in ()))
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("(%P|%t) ERROR: IOR in file %s ")
                                 ACE_TEXT ("is not a FT_Naming::ReplicationManager\n"),
                                 primary_file_name.c_str ()),
                                -1);

            if (TAO_debug_level > 3)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) - FT_Naming_Server ")
                          ACE_TEXT ("backup registering with primary.\n")));
            // Register with the primary
            CosNaming::NamingContext_var root = this->my_root_context ();
            FT_Naming::NamingManager_var nm = this->my_naming_manager ();
            int registration_result =
              this->replication_manager_->register_with_peer_replica (peer_ref.in (),
                                                                      root.in (),
                                                                      nm.in ());
            if (registration_result == -1)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("(%P|%t) ERROR: Backup unable to ")
                                 ACE_TEXT ("register with the primary\n")),
                                -1);
          }
        else
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%P|%t) ERROR: No primary IOR ")
                               ACE_TEXT ("available. Have you started the ")
                               ACE_TEXT ("primary? Exiting.\n")),
                              -1);
          }
      }
    else
      {// We are neither a primary or replica, but running in standalone mode
        if (TAO_debug_level > 3)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) - FT_Naming_Server:Is Standalone\n")));

      }
  }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("TAO_FT_Naming_Server::init_replication_manager_with_orb.\n"));
      return -1;
    }

  // Success
  return 0;
}


int
TAO_FT_Naming_Server::parse_args (int argc,
                                  ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:c:do:p:s:f:m:z:r:u:v:g:h:"));

  // Define the arguments for primary and backup
  get_opts.long_option (ACE_TEXT ("primary"), ACE_Get_Opt::NO_ARG);
  get_opts.long_option (ACE_TEXT ("backup"), ACE_Get_Opt::NO_ARG);
  bool role_defined = false;

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

  int v_opt_used = 0;

  // TODO: remove unsupported options with FT Naming Server
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        ++TAO_debug_level;
        break;
      case 'o': // outputs this servers naming service ior to a file.
        this->ior_file_name_ = get_opts.opt_arg ();
        break;
      case 'c': // outputs the multi-profile naming service ior file
        this->combined_naming_service_ior_file_name_ = get_opts.opt_arg ();
        break;
      case 'g': // outputs the mutli-profile object group manager ior file
        this->combined_naming_manager_ior_file_name_ = get_opts.opt_arg ();
        break;
      case 'h': // outputs the object group manager ior to a file
        this->naming_manager_ior_file_name_ = get_opts.opt_arg ();
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
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) ERROR: Unable to ")
                             ACE_TEXT ("process <-b> option")),
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
        this->object_group_dir_ = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ());
        v_opt_used = 1;
        break;

#endif /* TAO_HAS_MINIMUM_POA == 0 */
#endif /* !CORBA_E_MICRO */
      case 'z':
        this->use_round_trip_timeout_ = 1;
        this->round_trip_timeout_ =
          (int)1.0e7 * ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 0:   // A long option was found
        {
          const ACE_TCHAR *long_option = get_opts.long_option ();
          if (ACE_OS::strcmp (long_option, ACE_TEXT ("backup")) == 0)
            {
              this->replica_id_ = ACE_TEXT ("Backup");
              this->server_role_ = TAO_FT_Naming_Server::BACKUP;
              role_defined = true;
            }
          else if (ACE_OS::strcmp (long_option,
                                   ACE_TEXT ("primary")) == 0)
            {
              this->replica_id_ = ACE_TEXT ("Primary");
              this->server_role_ = TAO_FT_Naming_Server::PRIMARY;
              role_defined = true;
            }
        }
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s\n")
                           ACE_TEXT ("--primary  (not used with --backup)\n")
                           ACE_TEXT ("--backup  (not used with --primary)\n")
                           ACE_TEXT ("-d\n")
                           ACE_TEXT ("-c <multi-profile_name_service_ior_file>\n")
                           ACE_TEXT ("-o <name_svc_ior_output_file>\n")
                           ACE_TEXT ("-g <multi-profile_naming_mgr_ior_file>\n")
                           ACE_TEXT ("-h <naming_mgr_ior_output_file>\n")
                           ACE_TEXT ("-p <pid_file_name>\n")
                           ACE_TEXT ("-s <context_size>\n")
                           ACE_TEXT ("-b <base_address>\n")
                           ACE_TEXT ("-m <1=enable multicast,")
                           ACE_TEXT (" 0=disable multicast(default)>\n")
                           ACE_TEXT ("-n <num_threads>\n")
                           ACE_TEXT ("-f <persistence_file_name>\n")
                           ACE_TEXT ("-u <storable_persistence_directory")
                           ACE_TEXT (" (not used with -f)>\n")
                           ACE_TEXT ("-v <storable_object_group_persistence")
                           ACE_TEXT ("_directory>\n")
                           ACE_TEXT ("-r <redundant_persistence_directory>\n")
                           ACE_TEXT ("-z <relative round trip timeout>\n")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }


  if (f_opt_used + u_opt_used + r_opt_used > 1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("ERROR: Only one persistence option ")
                       ACE_TEXT ("can be provided.\n\n")),
                      -1);

  // If naming context or object group persistence is being used then
  // enable backup/restore compability of persitent files.
  if (u_opt_used || r_opt_used || v_opt_used)
    {
      TAO::Storable_Base::use_backup_default = true;
    }

  if (!role_defined)
    { // No role specified, so we will become a STANDALONE server
      this->replica_id_ = ACE_TEXT ("Standalone");
      this->server_role_ = TAO_FT_Naming_Server::STANDALONE;
      // User has not provided a role, so we will not use redundancy option
      if (this->use_redundancy_ == 1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("INFO: Cannot run standalone with ")
                      ACE_TEXT ("-r option. Using -u instead.\n")
                      ACE_TEXT ("Must start a '--primary' and a '--backup' ")
                      ACE_TEXT ("server to run as a Fault \n")
                      ACE_TEXT ("Tolerant Naming Service. \n")));
          this->use_redundancy_ = 0;
        }

    }
  else
    {
      // Only the backup should be requested to write the multi-profile IOR
      if ((this->server_role_ != TAO_FT_Naming_Server::BACKUP) &&
          (this->combined_naming_service_ior_file_name_ != 0))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: Must export the multi-profile ")
                           ACE_TEXT ("IOR (using '-c' option) from the backup")
                           ACE_TEXT (" server.\n\n")),
                          -1);

      // Only the backup should be requested to write the multi-profile IOR
      if ((this->server_role_ == TAO_FT_Naming_Server::BACKUP) &&
          (this->combined_naming_service_ior_file_name_ == 0))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: Must export the multi-profile ")
                           ACE_TEXT ("IOR (using '-c' option) from the backup")
                           ACE_TEXT (" server.\n\n")),
                          -1);

    }
  return 0;
}

int
TAO_FT_Naming_Server::fini (void)
{
  // Destroy the child POAs created when initializing
  // the FT Naming Service
  try
    {
      if (!CORBA::is_nil (this->naming_manager_poa_.in ()))
        this->naming_manager_poa_->destroy (1, 1);
      this->naming_manager_poa_ = PortableServer::POA::_nil ();

      if (!CORBA::is_nil (this->replication_manager_poa_.in ()))
        this->replication_manager_poa_->destroy (1, 1);

      this->replication_manager_poa_ = PortableServer::POA::_nil ();
      CORBA::Object_var table_object =
        this->orb_->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in ());
      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) ERROR: Nil IORTable\n")));
        }
      else
        {
          adapter->unbind ("NameService");
          adapter->unbind ("NamingManager");
        }

#if !defined (CORBA_E_MICRO)
      CORBA::Object_var svc =
        this->orb_->unregister_initial_reference ("NameService");
        this->orb_->unregister_initial_reference ("NamingManager");
#endif /* CORBA_E_MICRO */

    }
  catch (const CORBA::Exception&)
    {
      // Ignore
    }

  // Specific FT_Naming cleanup
  naming_manager_poa_ = PortableServer::POA::_nil ();
  replication_manager_poa_ = PortableServer::POA::_nil ();
  my_naming_manager_ = FT_Naming::NamingManager::_nil ();
  peer_naming_manager_ = FT_Naming::NamingManager::_nil ();
  peer_root_context_ = CosNaming::NamingContext::_nil ();

#if !defined (CORBA_E_MICRO)
  delete replication_manager_;
#endif /* CORBA_E_MICRO */

  // Invoke the base class fini
  return TAO_Naming_Server::fini ();
}

TAO_Storable_Naming_Context_Factory *
TAO_FT_Naming_Server::storable_naming_context_factory (size_t context_size)
{
  return new (ACE_nothrow) TAO_FT_Storable_Naming_Context_Factory (context_size);
}

TAO_Persistent_Naming_Context_Factory *
TAO_FT_Naming_Server::persistent_naming_context_factory (void)
{
  return new (ACE_nothrow) TAO_FT_Persistent_Naming_Context_Factory;
}


int
TAO_FT_Naming_Server::read_reference_from_file (const char* replica_file_name,
                                                CORBA::Object_out obj_ref)
{

  ACE_CString replica_ior_string ("file://");
  replica_ior_string += replica_file_name;

  try {
    CORBA::Object_var object =
      this->orb_->string_to_object (replica_ior_string.c_str ());
    if (CORBA::is_nil (object.in ()))
      {
        if (TAO_debug_level > 3)
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) - invalid ior in file <%s>\n"),
                      replica_file_name));

        return -1;
      }

    obj_ref = object._retn ();

  }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("Invalid object reference in file: %s\n"));
      return -1;
    }

  return 0;
}

int
TAO_FT_Naming_Server::export_ft_naming_references (void)
{
  int result = 0;

  switch (this->server_role_) {
    // Neither the PRIMARY or STANDALONE server roles are able to write
    // a multi-profile IOR for the redundant server pair.
  case TAO_FT_Naming_Server::STANDALONE:
  case TAO_FT_Naming_Server::PRIMARY:

    if (this->naming_manager_ior_file_name_ != 0)
      {
          FT_Naming::NamingManager_var my_nm =
            this->my_naming_manager ();
          CORBA::String_var naming_manager_ior_string =
            this->orb_->object_to_string (my_nm.in ());
          this->write_ior_to_file (
            naming_manager_ior_string.in (),
            ACE_TEXT_ALWAYS_CHAR (this->naming_manager_ior_file_name_));
      }

    // Make sure the user provided an ior_file_name for the comb
    if (this->combined_naming_service_ior_file_name_ != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) ERROR: Unable to write combined")
                           ACE_TEXT (" NameService IOR file. ")
                           ACE_TEXT ("Only supported by the backup naming service.\n")
                           ACE_TEXT ("Provide the -c option to the --backup role.\n")),
                          -1);
      }
    return 0;
    break;

  case TAO_FT_Naming_Server::BACKUP:
    {
      // Make sure the user provided an ior_file_name for the multi-profile ior file
      if (this->combined_naming_service_ior_file_name_ == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) ERROR: Unable to write combined")
                             ACE_TEXT (" NameService IOR file. ")
                             ACE_TEXT ("No file name provided.\n")),
                            -1);
          return 0;
        }

      CORBA::Object_var peer_root_cxt = this->peer_root_context ();
      if (CORBA::is_nil (peer_root_cxt.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) ERROR: Unable to get the primary")
                             ACE_TEXT (" NameService object ref")),
                            -1);
        }

      CORBA::Object_var my_root_cxt = this->my_root_context ();
      if (CORBA::is_nil (my_root_cxt.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) ERROR: Unable to get this")
                             ACE_TEXT (" services NameService object ref")),
                            -1);
        }

      CORBA::Object_var IORM =
        this->orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION, 0);

      TAO_IOP::TAO_IOR_Manipulation_var iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in ());

      // Combine the primary and backup (my) object references for the naming service
      CORBA::Object_var combined_obj_ref =
        iorm->add_profiles (peer_root_cxt.in (),
                            my_root_cxt.in ());

      if (CORBA::is_nil (combined_obj_ref.in ()))
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: could not combine")
                     ACE_TEXT(" primary and backup IORs for")
                     ACE_TEXT(" fault tolerant Naming Service.\n")));
          return -1;
        }

      CORBA::String_var combined_nameservice_ior_string =
        this->orb_->object_to_string (combined_obj_ref.in ());

      // Write out the combined IOR for the NameService
      this->write_ior_to_file (
        combined_nameservice_ior_string.in (),
        ACE_TEXT_ALWAYS_CHAR (this->combined_naming_service_ior_file_name_));

      // Verify that a naming manager ior file name was provided by user
      if (this->combined_naming_manager_ior_file_name_ == 0)
        {
          if (TAO_debug_level > 3)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t) - FT_Naming_Server:No NamingManager")
                        ACE_TEXT (" IOR file name provided")
                        ACE_TEXT (" with -g option. Not writing IOR.\n")));
        }
      else
        {// A file name was provided to store the naming manager IOR

          FT_Naming::NamingManager_var peer_nm =
            this->peer_naming_manager ();
          FT_Naming::NamingManager_var my_nm =
            this->my_naming_manager ();

          // This is the object reference for the fault tolerant
          // naming manager.  The primary should be first.
          combined_obj_ref =
            iorm->add_profiles (peer_nm.in (),
                                my_nm.in ());

          if (CORBA::is_nil (combined_obj_ref.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT("(%P|%t) ERROR: could not combine")
                                 ACE_TEXT(" primary and backup IORs for")
                                 ACE_TEXT(" fault tolerant Naming Manager.\n")),
                                -1);
            }

          CORBA::String_var combined_naming_manager_ior_string =
            this->orb_->object_to_string (combined_obj_ref.in ());

          // Write out the combined IOR for the NameService
          this->write_ior_to_file (
            combined_naming_manager_ior_string.in (),
            ACE_TEXT_ALWAYS_CHAR (this->combined_naming_manager_ior_file_name_));
        }

      return 0;
    }
    break;
  };
  return result;
}



/// Return the IOR for the registered replication manager
char*
TAO_FT_Naming_Server::replication_manager_ior (void)
{
  return CORBA::string_dup (this->replication_manager_ior_.in ());
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
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  if (this->use_object_group_persistence_)
    {
      if (TAO_debug_level > 3)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("%T %n (%P|%t) - ")
                      ACE_TEXT ("An update of object group with ID %lld ")
                      ACE_TEXT ("has been made by the peer"),
                      group_info.id
                      ));
        }
      this->naming_manager_.set_object_group_stale (group_info);
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) ERROR: Attempting to update object group ")
                  ACE_TEXT ("as stale with obect group persistence not ")
                  ACE_TEXT ("enabled.")));
      return -1;
    }

  return 0;
}

int
TAO_FT_Naming_Server::update_naming_context (
    const FT_Naming::NamingContextUpdate & context_info)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

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

    // This context is not currently active in this server so
    // there is nothing to be done, so return success.
    return 0;
  }

  TAO_Naming_Context* changed_context_servant =
    dynamic_cast<TAO_Naming_Context*> (servant.in ());

  if (changed_context_servant == 0)
  { // Another type of class was used as the servant. Should not happen.
    ACE_ERROR ((LM_ERROR,
               ACE_TEXT ("(%P|%t) ERROR: Invalid servant type registered")
               ACE_TEXT (" with oid: %s"),
               context_info.context_name.in ()));
    return -1;
  }

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%T %n (%P|%t) - ")
                  ACE_TEXT ("An update of naming context with name %s ")
                  ACE_TEXT ("has been made by the peer"),
                  context_info.context_name.in ()
                  ));
    }

  // Mark the local context stale, so we will reload it next
  // time it is modified or accessed.
  changed_context_servant->stale (true);

  return 0;
}

/// Destructor.
TAO_FT_Naming_Server::~TAO_FT_Naming_Server (void)
{
  // Clear out the static naming manager from the persistent naming context
  TAO_FT_Persistent_Naming_Context::set_naming_manager_impl (0);
}


void
TAO_FT_Naming_Server::peer_root_context (CosNaming::NamingContext_ptr peer_cxt)
{
  peer_root_context_ = CosNaming::NamingContext::_duplicate (peer_cxt);
}

CosNaming::NamingContext_ptr
TAO_FT_Naming_Server::peer_root_context (void)
{
  return CosNaming::NamingContext::_duplicate (peer_root_context_.in ());
}

CosNaming::NamingContext_ptr
TAO_FT_Naming_Server::my_root_context (void) const
{
  return CosNaming::NamingContext::_duplicate (this->naming_context_.in ());
}

void
TAO_FT_Naming_Server::peer_naming_manager (FT_Naming::NamingManager_ptr peer_cxt)
{
  peer_naming_manager_ = FT_Naming::NamingManager::_duplicate (peer_cxt);
}

FT_Naming::NamingManager_ptr
TAO_FT_Naming_Server::peer_naming_manager (void)
{
  return  FT_Naming::NamingManager::_duplicate (peer_naming_manager_.in ());
}

FT_Naming::NamingManager_ptr
TAO_FT_Naming_Server::my_naming_manager (void) const
{
  return  FT_Naming::NamingManager::_duplicate (this->my_naming_manager_.in ());
}
