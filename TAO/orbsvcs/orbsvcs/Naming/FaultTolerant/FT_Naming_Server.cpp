// -*- C++ -*-

//=============================================================================
/**
 * @file   FT_Naming_Server.cpp
 *
 *
 * @author Kevin Stanley <stanleyk@ociweb.com>
 */
//=============================================================================


#include "orbsvcs/Log_Macros.h"
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

#include "tao/ORB_Core.h"

#include "tao/debug.h"
#include "tao/default_ports.h"
#include "tao/Storable_FlatFileStream.h"

#include "tao/debug.h"
#include "tao/default_ports.h"
#include "tao/IORTable/IORTable.h"
#include "tao/IORManipulation/IORManip_Loader.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
#include "tao/Messaging/Messaging.h"
#endif

#include "tao/AnyTypeCode/Any.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const ACE_TCHAR* primary_replica_ior_filename =
  ACE_TEXT ("ns_replica_primary.ior");

const ACE_TCHAR* backup_replica_ior_filename =
  ACE_TEXT ("ns_replica_backup.ior");


TAO_FTNS_Notifier::TAO_FTNS_Notifier (TAO_FT_Naming_Server &owner, bool iors)
  : owner_ (owner),
    iors_ (iors)
{
}

int
TAO_FTNS_Notifier::handle_exception (ACE_HANDLE )
{
  if (this->iors_)
    owner_.update_iors_i();
  else
    owner_.update_info_i();
  return 0;
}

/// Default Constructor.
TAO_FT_Naming_Server::TAO_FT_Naming_Server (void)
  : TAO_Naming_Server (IOR_ARRAY_SIZE),
    naming_manager_ (),
    replicator_ (0),
    use_object_group_persistence_ (0),
    server_role_ (STANDALONE),
    ft_endpoint_ (""),
    ft_update_delay_ (ACE_Time_Value::zero),
    registered_ (false),
    ior_notifier_ (*this, true),
    info_notifier_ (*this, false),
    u_iors_(),
    u_infos_(),
    ior_lock_(),
    info_lock_()

{
}

void
TAO_FT_Naming_Server::update_info_i (void)
{
  while (true)
    {
      FT_Naming::UpdateInfoSeq_var block;
      {
        ACE_GUARD(TAO_SYNCH_MUTEX, guard, this->info_lock_);
        if (this->u_infos_.dequeue_head(block.out()) == -1)
          {
            break;
          }
      }

      for (CORBA::ULong i = 0; i < block->length(); i++)
        {
          FT_Naming::UpdateInfo &infoRef = (*block)[i];

          if (infoRef._d() == FT_Naming::ctx_name)
            {
              this->update_naming_context (infoRef.ctx());
            }
          else
            {
              this->update_object_group (infoRef.grp());
            }
        }
    }
}

void
TAO_FT_Naming_Server::update_info (FT_Naming::UpdateInfoSeq &infos)
{

  CORBA::ULong count = infos.length();
  FT_Naming::UpdateInfo* guts = infos.get_buffer(true);
  FT_Naming::UpdateInfoSeq_var block = 0;
  ACE_NEW(block, FT_Naming::UpdateInfoSeq (count,count,guts,true));
  {
    ACE_GUARD(TAO_SYNCH_MUTEX, guard, this->info_lock_);
    this->u_infos_.enqueue_tail (block._retn());
  }

  this->orb_->orb_core ()->reactor ()->notify (&this->info_notifier_);
}

void
TAO_FT_Naming_Server::update_iors_i (void)
{
  while (true)
    {
      FT_Naming::ReplicaInfo rep;
      {
        ACE_GUARD(TAO_SYNCH_MUTEX, guard, this->ior_lock_);
        if (this->u_iors_.dequeue_head(rep) == -1)
          {
            break;
          }
      }

      if (rep.combined)
        {
          this->update_ior (FT_Naming::ctx_name, rep.root_context.in());
          this->update_ior (FT_Naming::obj_grp, rep.naming_manager.in());
        }
      else
        {
          this->combine_iors (FT_Naming::ctx_name, rep.root_context.in());
          this->combine_iors (FT_Naming::obj_grp, rep.naming_manager.in());
          this->replicator_->send_registration (true);
        }
    }
}

void
TAO_FT_Naming_Server::update_iors (const FT_Naming::ReplicaInfo & iors )
{
  {
    ACE_GUARD(TAO_SYNCH_MUTEX, guard, this->ior_lock_);
    this->u_iors_.enqueue_tail(iors);
  }
  this->orb_->orb_core ()->reactor ()->notify (&this->ior_notifier_);
}


int
TAO_FT_Naming_Server::init_with_orb (int argc,
                                     ACE_TCHAR *argv [],
                                     CORBA::ORB_ptr orb)
{
  ACE_NEW_RETURN (this->replicator_, TAO_FT_Naming_Replication_Manager (*this), -1);

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
          ORBSVCS_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) ERROR: Invalid object ")
                             ACE_TEXT ("group persistence directory\n")),
                            -1);
        }

      TAO::Storable_Factory * object_group_storable_factory;
#if defined (ACE_USES_WCHAR)
      ACE_CString alt_group_dir =
        ACE_TEXT_ALWAYS_CHAR (this->object_group_dir_.c_str());
      ACE_NEW_RETURN (object_group_storable_factory,
                      TAO::Storable_FlatFileFactory (alt_group_dir),
                      -1);
#else
      ACE_NEW_RETURN (object_group_storable_factory,
                      TAO::Storable_FlatFileFactory (this->object_group_dir_),
                      -1);
#endif /* ACE_USES_WCHAR */

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

  try
    {
      // Initialize the replicator if necessary
      result = init_replicator ();
      if (result != 0)
        return result;

      // Setup the pairing with peer
      init_replication_pairing ();
      this->naming_manager_.set_replicator (this->replicator_);
    }
  catch (const CORBA::Exception& ex)
    { // No exceptions are expected.
      ex._tao_print_exception
        (ACE_TEXT ("TAO_FT_Naming_Server::init_with_orb"));
      return -1;
    }

  return result;
}

int
TAO_FT_Naming_Server::init_naming_manager_with_orb (int, ACE_TCHAR *[], CORBA::ORB_ptr orb)
{
  int result = 0;

  // Need to lock during startup to prevent access of partially
  // initialized variables
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  try
    {
      if (CORBA::is_nil (this->orb_.in ()))
        {
          this->orb_ = CORBA::ORB::_duplicate (orb);
        }

      // Get the POA from the ORB.
      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ORBSVCS_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT(" (%P|%t) ERROR: Unable to initialize the POA.\n")),
                                -1);
        }

      if (result != 0)
        return result;

      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager ();

      int numPolicies = 2;
      CORBA::PolicyList policies (numPolicies);
      policies.length (numPolicies);
      policies[0] =
        this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);
      policies[1] =
        this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);

      this->naming_manager_poa_ =
        this->root_poa_->create_POA ("NamingManager",
                                     poa_manager.in (),
                                     policies);

      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy ();
        }
      poa_manager->activate ();

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("NamingManager");

      this->naming_manager_poa_->activate_object_with_id (id.in (), &this->naming_manager_);
      this->assign (size_t(GROUP), true, this->naming_manager_poa_->id_to_reference (id.in ()));

      if (write (size_t(GROUP)) != 0)
        return -1;


      this->naming_manager_.initialize (this->orb_.in (),
                                        this->naming_manager_poa_.in ());

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
                               ACE_TEXT ("TAO_FT_Naming_Server::init_naming_manager_with_orb"));
      return -1;
    }
  CORBA::Object_var table_object =
    orb->resolve_initial_references ("IORTable");

  IORTable::Table_var ior_table =
    IORTable::Table::_narrow (table_object.in ());
  if (!CORBA::is_nil (ior_table))
    {
      ior_table->bind ("NamingManager",
                       this->iors_[GROUP].ior_.c_str ());
    }
  return 0;
}

int
TAO_FT_Naming_Server::init_replicator (void)
{
  if (this->server_role_ == STANDALONE )
    return 0;

  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  try
    {
      this->replicator_->init_orb();
      this->replicator_->activate();
      this->iors_[REPLICATOR].ior_ = this->replicator_->ior ();
      this->iors_[REPLICATOR].filename_ = this->replica_ior_filename (false);
      this->write(REPLICATOR);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("TAO_FT_Naming_Server::init_replicator\n"));
      return -1;
    }

  return 0;
}

ACE_TString
TAO_FT_Naming_Server::replica_ior_filename (bool peer_ior_file) const
{
  ServerRole desired = this->server_role_;
  if (peer_ior_file)
    {
      desired = (desired  == PRIMARY) ? BACKUP : PRIMARY;
    }
  ACE_TString path = this->persistence_dir_;
  path += ACE_DIRECTORY_SEPARATOR_STR;
  if (desired == PRIMARY)
    {
      path += primary_replica_ior_filename;
    }
  else
    {
      path += backup_replica_ior_filename;
    }
  return path;
}

void
TAO_FT_Naming_Server::init_replication_pairing (void)
{
  if (this->server_role_ == STANDALONE)
    {
      if (TAO_debug_level > 3)
        ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t) - FT_Naming_Server:Is ")
                        ACE_TEXT ("Standalone\n")));
    }

  if (this->replicator_->init_peer ())
    {
      this->replicator_->send_registration (false);
    }
}

void
TAO_FT_Naming_Server::no_replica ()
{

  if (this->server_role_ == BACKUP)
    {
      this->recover_iors ();
    }
}


int
TAO_FT_Naming_Server::parse_args (int argc,
                                  ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:c:do:p:s:f:m:z:r:u:v:g:h:l:"));

  // Define the arguments for primary and backup
  get_opts.long_option (ACE_TEXT ("primary"), ACE_Get_Opt::NO_ARG);
  get_opts.long_option (ACE_TEXT ("backup"), ACE_Get_Opt::NO_ARG);
  get_opts.long_option (ACE_TEXT ("ftendpoint"), ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT ("ftupdatedelay"), ACE_Get_Opt::ARG_REQUIRED);

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
        this->iors_[ROOT].filename_ = get_opts.opt_arg ();
        break;
      case 'c': // outputs the multi-profile naming service ior file
        this->iors_[FT_ROOT].filename_ = get_opts.opt_arg ();
        break;
      case 'g': // outputs the mutli-profile object group manager ior file
        this->iors_[FT_GROUP].filename_ = get_opts.opt_arg ();
        break;
      case 'h': // outputs the object group manager ior to a file
        this->iors_[GROUP].filename_ = get_opts.opt_arg ();
        break;
      case 'l':
        this->naming_manager_.set_global_strategy (get_opts.opt_arg ());
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
                             ACE_TEXT ("(%P|%t) ERROR: Unable to ")
                             ACE_TEXT ("process <-b> option")),
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
      case 'v':
        this->use_object_group_persistence_ = 1;
        this->object_group_dir_ = get_opts.opt_arg ();
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
              this->server_role_ = TAO_FT_Naming_Server::BACKUP;
              break;
            }
          else if (ACE_OS::strcmp (long_option,
                                   ACE_TEXT ("primary")) == 0)
            {
              this->server_role_ = TAO_FT_Naming_Server::PRIMARY;
              break;
            }
          else if (ACE_OS::strcasecmp (long_option,
                                       ACE_TEXT ("ftendpoint")) == 0)
            {
              this->ft_endpoint_ = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg());
              break;
            }
          else if (ACE_OS::strcasecmp (long_option,
                                       ACE_TEXT ("ftupdatedelay")) == 0)
            {
              this->ft_update_delay_ =
                ACE_Time_Value (0, 1000 * ACE_OS::atoi (get_opts.opt_arg()));
              break;
            }
        }
      case '?':
      default:
        ORBSVCS_ERROR ((LM_ERROR,ACE_TEXT ("Unknown arg %c\n"), c ));
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s\n")
                           ACE_TEXT ("--primary  (not used with --backup)\n")
                           ACE_TEXT ("--backup  (not used with --primary)\n")
                           ACE_TEXT ("--FtEndpoint <endpoint for FT ORB>\n")
                           ACE_TEXT ("--FtUpdateDelay <msec>\n")
                           ACE_TEXT ("-d\n")
                           ACE_TEXT ("-c <multi-profile_name_service_ior_file>\n")
                           ACE_TEXT ("-o <name_svc_ior_output_file>\n")
                           ACE_TEXT ("-g <multi-profile_naming_mgr_ior_file>\n")
                           ACE_TEXT ("-h <naming_mgr_ior_output_file>\n")
                           ACE_TEXT ("-l <global_lb_strategy_name>\n")
                           ACE_TEXT ("-p <pid_file_name>\n")
                           ACE_TEXT ("-s <context_size>\n")
                           ACE_TEXT ("-b <base_address>\n")
                           ACE_TEXT ("-m <1=enable multicast,")
                           ACE_TEXT (" 0=disable multicast(default)>\n")
                           ACE_TEXT ("-n <num_threads>\n")
                           ACE_TEXT ("-f <persistence_directory>\n")
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
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("ERROR: Only one persistence option ")
                       ACE_TEXT ("can be provided.\n\n")),
                      -1);

  // If naming context or object group persistence is being used then
  // enable backup/restore compatibility of persistent files.
  if (u_opt_used || v_opt_used || r_opt_used)
    {
      TAO::Storable_Base::use_backup_default = true;
    }

  if (server_role_ != STANDALONE)
    {
      if ((this->server_role_ == BACKUP) ==
          (this->iors_[FT_ROOT].filename_.length() == 0))
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("ERROR: Must export the multi-profile ")
                               ACE_TEXT ("IOR (using '-c' option) from the backup ")
                               ACE_TEXT ("server.\n\n")),
                              -1);
    }
  return 0;
}

int
TAO_FT_Naming_Server::fini (void)
{
  if (this->replicator_ != 0)
    {
      try
        {
          this->replicator_->stop ();
        }
      catch (const CORBA::Exception&)
        {
          // Ignore
        }
      delete replicator_;
      replicator_ = 0;
    }
  try
    {
      if (!CORBA::is_nil (this->naming_manager_poa_.in ()))
        this->naming_manager_poa_->destroy (1, 1);
      this->naming_manager_poa_ = PortableServer::POA::_nil ();
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
      if (!CORBA::is_nil (ior_table.in()))
        {
          ior_table->unbind ("NameService");
          ior_table->unbind ("NamingManager");
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
        this->orb_->unregister_initial_reference ("NamingManager");
#endif /* CORBA_E_MICRO */

    }
  catch (const CORBA::Exception&)
    {
      // Ignore
    }

  // Specific FT_Naming cleanup
  naming_manager_poa_ = PortableServer::POA::_nil ();
  iors_[GROUP].ref_ =  FT_Naming::NamingManager::_nil ();


  // Invoke the base class fini
  return TAO_Naming_Server::fini ();
}

TAO_Storable_Naming_Context_Factory *
TAO_FT_Naming_Server::storable_naming_context_factory (size_t context_size)
{
#if defined (ACE_HAS_NEW_NOTHROW)
  return new (ACE_nothrow) TAO_FT_Storable_Naming_Context_Factory (context_size, this->replicator_);
#else
  return new TAO_FT_Storable_Naming_Context_Factory (context_size, this->replicator_);
#endif /* ACE_HAS_NEW_NOTHROW */
}

TAO_Persistent_Naming_Context_Factory *
TAO_FT_Naming_Server::persistent_naming_context_factory (void)
{
#if defined (ACE_HAS_NEW_NOTHROW)
  return new (ACE_nothrow) TAO_FT_Persistent_Naming_Context_Factory;
#else
  return new TAO_FT_Persistent_Naming_Context_Factory;
#endif /* ACE_HAS_NEW_NOTHROW */
}

/// Return the IOR for the registered replication manager
char*
TAO_FT_Naming_Server::replicator_ior (void)
{
  return CORBA::string_dup (this->iors_[REPLICATOR].ior_.c_str());
}

/// Return the IOR for the registered object group manager
char*
TAO_FT_Naming_Server::naming_manager_ior (void)
{
  return CORBA::string_dup (this->iors_[GROUP].ior_.c_str());
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
          ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) - ")
                      ACE_TEXT ("An update of object group with ID %lld ")
                      ACE_TEXT ("has been made by the peer\n"),
                      group_info.id
                      ));
        }
      this->naming_manager_.set_object_group_stale (group_info);
    }
  else
    {
      ORBSVCS_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) ERROR: Attempting to update object group ")
                  ACE_TEXT ("as stale with obect group persistence not ")
                  ACE_TEXT ("enabled.\n")));
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
    ORBSVCS_ERROR ((LM_ERROR,
               ACE_TEXT ("(%P|%t) ERROR: Invalid servant type registered")
               ACE_TEXT (" with oid: %s"),
               context_info.context_name.in ()));
    return -1;
  }

  if (TAO_debug_level > 3)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
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

CosNaming::NamingContext_ptr
TAO_FT_Naming_Server::my_root_context (void) const
{
  if (CORBA::is_nil (this->iors_[ROOT].ref_.in()))
    {
      return CosNaming::NamingContext::_nil ();
    }

  CosNaming::NamingContext_ptr ret =
    CosNaming::NamingContext::_narrow (this->iors_[ROOT].ref_);

  return ret;
  //  return CosNaming::NamingContext::_narrow (this->iors_[ROOT].ref_.in());
}

FT_Naming::NamingManager_ptr
TAO_FT_Naming_Server::my_naming_manager (void) const
{
  if (CORBA::is_nil (this->iors_[GROUP].ref_))
    {
      return FT_Naming::NamingManager::_nil ();
    }

  FT_Naming::NamingManager_ptr ret =
    FT_Naming::NamingManager::_narrow (this->iors_[GROUP].ref_);

  return ret;

  //  return FT_Naming::NamingManager::_narrow (this->iors_[GROUP].ref_.in ());
}

CosNaming::NamingContext_ptr
TAO_FT_Naming_Server::ft_root_context (void) const
{
  if (CORBA::is_nil (this->iors_[FT_ROOT].ref_))
    {
      return CosNaming::NamingContext::_nil ();
    }

  CosNaming::NamingContext_ptr ret =
    CosNaming::NamingContext::_narrow (this->iors_[FT_ROOT].ref_);

  return ret;
  //  return CosNaming::NamingContext::_narrow (this->iors_[FT_ROOT].ref_.in());
}

FT_Naming::NamingManager_ptr
TAO_FT_Naming_Server::ft_naming_manager (void) const
{
  if (CORBA::is_nil (this->iors_[FT_GROUP].ref_))
    {
      return FT_Naming::NamingManager::_nil ();
    }

  FT_Naming::NamingManager_ptr ret =
    FT_Naming::NamingManager::_narrow (this->iors_[FT_GROUP].ref_);

  return ret;

  //  return FT_Naming::NamingManager::_narrow (this->iors_[FT_GROUP].ref_.in ());
}

const ACE_CString &
TAO_FT_Naming_Server::ft_endpoint (void)
{
  return this->ft_endpoint_;
}

const ACE_Time_Value &
TAO_FT_Naming_Server::ft_update_delay (void)
{
  return this->ft_update_delay_;
}

void
TAO_FT_Naming_Server::update_ior (FT_Naming::EntityKind kind, CORBA::Object_ptr ft_obj)
{
  size_t combo = kind == FT_Naming::obj_grp ? FT_GROUP : FT_ROOT;

  this->assign(combo, false, ft_obj);
  this->write (combo);
}

void
TAO_FT_Naming_Server::combine_iors (FT_Naming::EntityKind kind, CORBA::Object_ptr peer)
{
  size_t local = ROOT;
  size_t combo = FT_ROOT;
  if (kind == FT_Naming::obj_grp)
    {
      local = GROUP;
      combo = FT_GROUP;
    }

  CORBA::Object_var IORM =
    this->orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION, 0);

  TAO_IOP::TAO_IOR_Manipulation_var iorm =
    TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in ());

  bool peer_nil = CORBA::is_nil (peer);
  bool local_nil =  CORBA::is_nil (this->iors_[local].ref_.in ());

  if (peer_nil || local_nil)
    {
      if (peer_nil)
        {
          this->assign (combo, false, this->iors_[local].ref_.in());
        }
    }
  else
    {
      if (this->server_role_ == PRIMARY)
        {
          this->assign (combo, false, iorm->add_profiles (this->iors_[local].ref_.in (),
                                                         peer));
        }
      else
        {

          this->assign (combo, false, iorm->add_profiles (peer,
                                                         this->iors_[local].ref_.in ()));
        }
    }

  if (CORBA::is_nil (this->iors_[combo].ref_.in ()) && TAO_debug_level > 0)
    {
      ORBSVCS_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: could not combine")
                     ACE_TEXT(" primary and backup IORs for")
                     ACE_TEXT(" fault tolerant Naming Service.\n")));
    }

  this->write (size_t(combo));
}

int
TAO_FT_Naming_Server::recover_iors (void)
{
  if (this->registered_)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
         ACE_TEXT ("ERROR: already registered IOR. ")
         ACE_TEXT ("recover_ior should not be called.\n")), -1);
    }

  size_t index[2] = {FT_ROOT, FT_GROUP};
  for (int i = 0; i < 2; i++)
    {
      ACE_TString fname = this->iors_[index[i]].filename_;
      if (TAO_debug_level > 0)
        {
          ORBSVCS_DEBUG ((LM_INFO, ACE_TEXT ("recover_ior <%C>\n"),
                          fname.c_str()));
        }
      if (ACE_OS::access (fname.c_str (), F_OK) != 0)
        return -1;

      CORBA::Object_ptr obj = CORBA::Object::_nil();
      try
        {
          ACE_TString combined_ior = ACE_TEXT ("file://") + fname;
          obj = this->orb_->string_to_object (combined_ior.c_str());
          if (!CORBA::is_nil (obj))
            {
              this->assign(index[i], true, obj);
            }
        }
      catch (const CORBA::Exception& ex)
        {
          if (TAO_debug_level > 0)
            {
              ORBSVCS_DEBUG ((LM_INFO,
                              ACE_TEXT ("Caught %C loading from %C\n"),
                              ex._name(),
                              fname.c_str()));
            }
          return -1;
        }
    }
  this->registered_ = true;

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
