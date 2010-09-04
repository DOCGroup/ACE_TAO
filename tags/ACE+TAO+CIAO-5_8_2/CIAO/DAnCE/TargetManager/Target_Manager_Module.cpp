// -*- C++ -*-
// $Id$

#include "Target_Manager_Module.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Task.h"
#include "tao/IORTable/IORTable.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "orbsvcs/CosNamingC.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/DAnCE/DAnCE_PropertiesC.h"

#include "TargetManager_Impl.h"

namespace DAnCE
{
  namespace Target_Manager
  {
    bool
    write_IOR (const ACE_TCHAR * ior_file_name, const char* ior)
    {
      FILE* ior_output_file_ =
        ACE_OS::fopen (ior_file_name, ACE_TEXT("w"));

      if (ior_output_file_)
        {
          ACE_OS::fprintf (ior_output_file_,
                           "%s",
                           ior);
          ACE_OS::fclose (ior_output_file_);
          return true;
        }
      return false;
    }
  }
}

DAnCE_TargetManager_Module::DAnCE_TargetManager_Module (void)
{
  //DANCE_TRACE("DAnCE_TargetManager_Module::DAnCE_TargetManager_Module");
}

DAnCE_TargetManager_Module::~DAnCE_TargetManager_Module (void)
{
  DANCE_TRACE ("DAnCE_TargetManager_Module::~DAnCE_TargetManager_Module");

  for (Servant_Map::iterator it = this->rm_map_.begin ();
       it != this->rm_map_.end ();
       ++it)
    {
      delete (*it).int_id_;
    }
}

const char *
DAnCE_TargetManager_Module::usage (void)
{
  DANCE_TRACE ("DAnCE_TargetManager_Module::usage");
  return "Repository Manager Options:\n"
    //"\t-n|--name [name]\t Name to register in the naming service\n"
    "\t-f|--file [name]\t Filename to output IOR.\n"
    "\t-i|--init [name]\t Filename read initial domain descriptor.\n"
    "\t-d|--domain-nc [NC]\t Default naming context for domain objects.\n"
    "\t-h|help\t\t\t print this help message\n"
    ;

}

bool
DAnCE_TargetManager_Module::parse_args (int argc, ACE_TCHAR * argv[])
{
  DANCE_TRACE ("DAnCE_TargetManager_Module::parse_args");

  ACE_Get_Opt get_opts (argc - 1,
                        argv + 1,
                        ACE_TEXT(":hd:f:i:"),
                        0,
                        0,
                        ACE_Get_Opt::RETURN_IN_ORDER,
                        1);

  get_opts.long_option (ACE_TEXT("help"), 'h', ACE_Get_Opt::NO_ARG);
  get_opts.long_option (ACE_TEXT("domain-nc"), 'd', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("file"), 'f', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("init"), 'i', ACE_Get_Opt::ARG_REQUIRED);

  int c;
  while ( (c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'd':
          DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT ("Target_Manager_Module::parse_args - ")
                        ACE_TEXT ("Binding to provided Domain Naming Context: '%s'\n"),
                        get_opts.opt_arg ()));
          this->options_.domain_nc_ = get_opts.opt_arg ();
          break;

        case 'f':
          DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT ("Target_Manager_Module::parse_args - ")
                        ACE_TEXT ("Output filename for IOR is %s\n"),
                        get_opts.opt_arg ()));
          this->options_.ior_file_ = get_opts.opt_arg ();
          break;

        case 'i':
          DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT ("Target_Manager_Module::parse_args - ")
                        ACE_TEXT ("Initial domain file %s\n"),
                        get_opts.opt_arg ()));
          this->options_.domain_descr_ = get_opts.opt_arg ();
          break;

        case 'h':
        case '?': // Display help for use of the server.
          ACE_ERROR_RETURN ((LM_ERROR,
                             this->usage (),
                             argv [0], c),
                             false);
          break;

        case 0:
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("Target_Manager_Module::parse_args - ")
                            ACE_TEXT ("ERROR: unknown long option %s\n"),
                            get_opts.long_option ()));
            }

          break;

        default:
          DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT ("Target_Manager_Module::parse_args - ignoring unknown option %c:%C\n"),
                        c, get_opts.opt_arg ()));
        }

    }

   return true;
}

CORBA::Object_ptr
DAnCE_TargetManager_Module::init (CORBA::ORB_ptr orb,
                                  int argc,
                                  ACE_TCHAR *argv[])
{
  DANCE_TRACE ("DAnCE_TargetManager_Module::init");

  try
    {
      if (CORBA::is_nil(orb))
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::init - ")
                       ACE_TEXT ("Attempted to create Target Manager with a nil orb.\n")));
          return CORBA::Object::_nil();
        }
      else
        {
          this->orb_ = CORBA::ORB::_duplicate (orb);
        }

      if (ACE_OS::strcmp(orb->id(), this->orb_->id()) != 0)
        {
          DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::init - ")
                       ACE_TEXT ("Resetting TM's orb.\n")));
          this->orb_ = CORBA::ORB::_duplicate (orb);
          this->domain_nc_ = CosNaming::NamingContext::_nil();
        }

      if (!this->parse_args (argc, argv))
        {
          return CORBA::Object::_nil ();
        }

      this->create_poas ();

      if (this->options_.domain_nc_)
        {
          try
            {
              DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::init - ")
                           ACE_TEXT ("Resolving DomainNC.\n")));
              CORBA::Object_var domain_obj = this->orb_->string_to_object (this->options_.domain_nc_);
              if (!CORBA::is_nil (domain_obj.in ()))
                {
                  this->domain_nc_ = CosNaming::NamingContext::_narrow (domain_obj.in());
                  if (CORBA::is_nil (this->domain_nc_.in ()))
                    {
                      DANCE_ERROR (1, (LM_ERROR,DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::init - ")
                                    ACE_TEXT ("Narrow to NamingContext return nil for DomainNC.\n")));
                      return CORBA::Object::_nil ();
                    }
                }
            }
          catch (CORBA::Exception&)
            {
              DANCE_DEBUG (6, (LM_WARNING, DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::init - ")
                             ACE_TEXT ("DomainNC context not found!\n")));
            }
        }


      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::init - ")
                    ACE_TEXT ("Initializing the IOR Table\n")));
      // Initialize IOR table
      CORBA::Object_var table_object = orb->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter = IORTable::Table::_narrow (table_object.in ());

      if (CORBA::is_nil (adapter.in ()))
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::init - ")
                         ACE_TEXT ("Unable to RIR the IORTable.\n")));
          return CORBA::Object::_nil ();
        }


      //Creating repository manager servant
      DAnCE::TargetManagerDaemon_i * rm = new DAnCE::TargetManagerDaemon_i (orb);

      PortableServer::ServantBase_var safe_svt (rm);

      ACE_CString repository_manager_oid;

      if (this->options_.name_ == 0)
        {
          repository_manager_oid = "TargetManager";
        }
      else
        {
          repository_manager_oid = ACE_TEXT_ALWAYS_CHAR (this->options_.name_);
          repository_manager_oid += ".TargetManager";
        }

      // Registering servant in poa
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId (repository_manager_oid.c_str());
      this->rm_poa_->activate_object_with_id (oid, rm);

      // Getting repository manager ior
      CORBA::Object_var nm_obj = this->rm_poa_->id_to_reference (oid.in ());
      CORBA::String_var ior = orb->object_to_string (nm_obj.in ());

      // Binding ior to IOR Table
      adapter->bind (repository_manager_oid.c_str (), ior.in ());

      // Binding repository manager to DomainNC
      if (!CORBA::is_nil (this->domain_nc_.in ()))
        {
          ACE_CString ns_name;
          if (this->options_.name_ == 0)
            {
              ns_name = "TargetManager";
            }
          else
            {
              ns_name = ACE_TEXT_ALWAYS_CHAR (this->options_.name_);
            }

          DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::init - ")
                       ACE_TEXT ("Registering NM in NC as \"%C\".\n"), ns_name.c_str ()));
          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup (ns_name.c_str ());
          name[0].kind = CORBA::string_dup ("TargetManager");
          this->domain_nc_->rebind (name, nm_obj.in ());
        }

      // Writing ior to file
      if (0 != this->options_.ior_file_)
        {
          DANCE_DEBUG (9, (LM_TRACE,  DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::init - ")
                        ACE_TEXT ("Writing RM IOR %C to file %C.\n"), this->options_.ior_file_, ior.in ()));
          if (!DAnCE::Target_Manager::write_IOR (this->options_.ior_file_, ior.in ()))
            DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::init - ")
                          ACE_TEXT ("Error: Unable to write IOR to file %C\n"),
                          this->options_.ior_file_));
        }

      // Activate POA manager
      PortableServer::POAManager_var mgr = this->root_poa_->the_POAManager ();
      mgr->activate ();

      // Finishing Deployment part
      DANCE_DEBUG (6, (LM_NOTICE, DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::init - ")
                    ACE_TEXT ("DAnCE_TargetManager is running...\n")));

      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::init - ")
                    ACE_TEXT ("TargetManager IOR: %s\n"), ior.in ()));

      return nm_obj._retn ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("DAnCE_TargetManager::main\t\n");
      return CORBA::Object::_nil ();
    }
}

void
DAnCE_TargetManager_Module::create_poas (void)
{
  DANCE_TRACE("DAnCE_Target_Manager_Module::create_poas");
  // Get reference to Root POA.
  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::create_poas - ")
                ACE_TEXT ("Resolving root POA\n")));
  CORBA::Object_var obj = this->orb_->resolve_initial_references ("RootPOA");

  this->root_poa_ = PortableServer::POA::_narrow (obj.in ());

  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::create_poas - ")
                ACE_TEXT ("Obtaining the POAManager\n")));
  PortableServer::POAManager_var mgr = this->root_poa_->the_POAManager ();

  TAO::Utils::PolicyList_Destroyer policies (2);
  policies.length (2);

  try
    {
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::create_poas - ")
                    ACE_TEXT ("DAnCE_TargetManager_Module::create_poas - ")
                    ACE_TEXT ("Creating the \"Repository\" POA.\n")));

      policies[0] = this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);
      policies[1] = this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);
      this->rm_poa_ = this->root_poa_->create_POA ("Repository",
                                       mgr.in(),
                                       policies);
    }
  catch (const PortableServer::POA::AdapterAlreadyExists &)
    {
      DANCE_DEBUG (8, (LM_INFO, DLINFO ACE_TEXT ("DAnCE_TargetManager_Module::create_poas - ")
                    ACE_TEXT ("Using existing \"Repository\" POA\n")));
      this->rm_poa_ = this->root_poa_->find_POA ("Repository", 0);
    }
}

