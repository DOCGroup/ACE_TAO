// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS.h"

#include "tao/TAO_Singleton_Manager.h"
#include "tao/StringSeqC.h"
#include "tao/IORTable/IORTable.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "orbsvcs/CosNamingC.h"
#include "ciao/CIAO_common.h"
#include "ciao/Valuetype_Factories/Cookies.h"
#include "ExecutionManager_Module.h"
#include "ExecutionManager_Impl.h"
#include "RedirectionService/RedirectionService.h"
#include "DAnCE/Logger/Log_Macros.h"

ACE_RCSID (DAnCE,
           DAnCE_ExecutionManager_Module,
           "$Id$")

namespace DAnCE
{
  namespace ExecutionManager
  {
    bool
    write_IOR (const char * ior_file_name, const char* ior)
    {
      FILE* ior_output_file_ =
        ACE_OS::fopen (ior_file_name, "w");

      if (ior_output_file_)
        {
          ACE_OS::fprintf (ior_output_file_,
                           "%s",
                           ior);
          ACE_OS::fclose (ior_output_file_);
          DANCE_DEBUG ( (LM_TRACE, DLINFO "ExectionManager::write_IOR - EM's ior was written into file \"%C\"\n", ior_file_name));
          return true;
        }
      else
        {
          DANCE_ERROR ( (LM_ERROR,
                         "[%M] DAnCE_ExecutionManager::write_ior-file - "
                         "Unable to open ExecutionManager IOR output file %C : %m\n",
                         ior_file_name));
          return false;
        }
      return true;
    }
  }
}

DAnCE_ExecutionManager_Module::DAnCE_ExecutionManager_Module (void)
  : em_impl_ (0)
{
}

DAnCE_ExecutionManager_Module::~DAnCE_ExecutionManager_Module (void)
{
  delete this->em_impl_;
}

bool
DAnCE_ExecutionManager_Module::parse_args (int argc, char *argv[])
{
  DANCE_TRACE ("DAnCE_ExecutionManager_Module::parse_args");

  ACE_Get_Opt get_opts (argc,
                        argv,
                        "n:e::p::c::r::ifh",
                        0,
                        0,
                        ACE_Get_Opt::RETURN_IN_ORDER);

  get_opts.long_option (ACE_TEXT("exec-mgr"), 'e', ACE_Get_Opt::ARG_OPTIONAL);
  get_opts.long_option (ACE_TEXT("node-mgr"), 'n', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("process-ns"), 'p', ACE_Get_Opt::ARG_OPTIONAL);
  get_opts.long_option (ACE_TEXT("create-plan-ns"), 'c', ACE_Get_Opt::ARG_OPTIONAL);
  get_opts.long_option (ACE_TEXT("rebind-plan-ns"), 'r', ACE_Get_Opt::ARG_OPTIONAL);
  get_opts.long_option (ACE_TEXT("port-indirection"), 'i', ACE_Get_Opt::NO_ARG);
  get_opts.long_option (ACE_TEXT("ignore-failure"), 'f', ACE_Get_Opt::NO_ARG);
  get_opts.long_option (ACE_TEXT("help"), 'h', ACE_Get_Opt::NO_ARG);
  get_opts.long_option (ACE_TEXT("node-map"), ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("domain-nc"), ACE_Get_Opt::ARG_REQUIRED);

  //get_opts.long_option ("help", '?');

  char c;
  while ( (c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'e':
          DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::parse_args -  "
                        "Output filename is %C\n",
                        get_opts.opt_arg ()));
          this->options_.exec_mgr_file_ = get_opts.opt_arg ();
          break;
        case 'n':
          DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::parse_args -  "
                        "--node-mgr option was parsed out for EM\n"));
          this->options_.node_managers_.push_back (get_opts.opt_arg ());
          break;

        case 'p':
          DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::parse_args -  "
                        "--process-ns enabled for EM\n"));
          this->options_.process_ns_ = true;
          this->options_.process_ns_file_ = get_opts.opt_arg ();
          break;

        case 'c':
          DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::parse_args - "
                        "--create-plan-ns enabled for EM\n"));
          this->options_.create_plan_ns_ = true;
          this->options_.create_plan_ns_ior_ = get_opts.opt_arg ();
          break;

        case 'r':
          DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::parse_args - "
                        "--rebind-plan-ns enabled for EM\n"));
          this->options_.rebind_plan_ns_ = true;
          this->options_.rebind_plan_ns_ior_ = get_opts.opt_arg ();
          break;

        case 'i':
          DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::parse_args - "
                        "--port-indirection enabled for EM\n"));
          this->options_.port_indirection_ = true;
          break;

        case 'f':
          DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::parse_args - "
                        "--ignore-failure enabled for EM\n"));
          this->options_.ignore_failure_ = true;
          break;

        case 0:
          if (ACE_OS::strcmp (get_opts.long_option (),
                              "node-map") == 0)
            {
              DANCE_DEBUG ((LM_DEBUG, DLINFO "Node_Manager_Module::parse_args - "
                            "Found Node map filename %C.\n",
                            get_opts.opt_arg ()));
              this->options_.node_map_ = get_opts.opt_arg ();
              break;

            }
          else if (ACE_OS::strcmp (get_opts.long_option (),
                                   "domain-nc") == 0)
            {
              DANCE_DEBUG ((LM_DEBUG, DLINFO "Node_Manager_Module::parse_args - "
                            "Binding to domain naming context %C.\n",
                            get_opts.opt_arg ()));
              this->options_.domain_nc_ = get_opts.opt_arg ();
              break;
            }

        case 'h':
          //case '?': // Display help for use of the server.
          //default:
          DANCE_ERROR_RETURN ((LM_ERROR,
                               "usage:  %C\n"
                               "\t--exec-mgr,-e [execution manager ior file name]\n"
                               "\t--node-mgr,-n <node name>[=node manager ior file name]\n"
                               "\t--node-map <file name> \t\tFile containing a node manager map\n"
                               "\t--domain-nc <nc ior> \t\tIOR for the Domain Naming Context\n"
                               //"--process-ns,-p [file name] \t\tcreate process name service and store its ior to file name\n"
                               //"--create-plan-ns,-c [NC] \t\tcreate plan objects (components and ports) representation in name context with ior NC\n"
                               //"--rebind-plan-ns,-r [NC] \t\tbind plan representation name context to NC\n"
                               //"-i \t\t\t\tenable plan objects indirection via servant locator\n",
                               ,argv [0]),
                              false);
          break;
        }
    }
  return true;
}

CORBA::Object_ptr
DAnCE_ExecutionManager_Module::create_object (CORBA::ORB_ptr orb,
                                              int argc,
                                              ACE_TCHAR *argv[])
{
  DANCE_TRACE ("DAnCE_ExecutionManager_Module::create_object");

  try
    {
      if (this->em_impl_ != 0)
        {
          DANCE_ERROR ( (LM_ERROR,
                         DLINFO "DAnCE_ExecutionManager_Module::create_object - "
                         "Error: ExecutionManager already exists.\n"));
          return CORBA::Object::_nil ();
        }

      DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::create_object - "
                    "before parsing arguments.\n"));

      if (!this->parse_args (argc, argv))
        return CORBA::Object::_nil ();

      DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::create_object - "
                    "before creating value factory.\n"));
      CORBA::ValueFactory_var vf = new Components::Cookie_init();
      DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::create_object - "
                    "before registering value factory.\n"));
      vf = orb->register_value_factory ("IDL:omg.org/Components/Cookie:1.0", vf.in());

      DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::create_object - "
                    "after creating value factory.\n"));
      // Get reference to Root POA.
      CORBA::Object_var poa_obj
        = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (poa_obj.in ());

      PortableServer::POAManager_var mgr = poa->the_POAManager ();

      PortableServer::POA_var persistent_poa;
      TAO::Utils::PolicyList_Destroyer policies (2);
      policies.length (2);
      try
        {
          DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::create_object - "
                        "before creating the \"Managers\" POA.\n"));

          policies[0] = poa->create_id_assignment_policy (PortableServer::USER_ID);
          policies[1] = poa->create_lifespan_policy (PortableServer::PERSISTENT);
          persistent_poa = poa->create_POA ("Managers", 
                                            mgr.in(), 
                                            policies);
        }
      catch (const PortableServer::POA::AdapterAlreadyExists &)
        {
          persistent_poa = poa->find_POA ("Managers", 0);
        }
      
      CosNaming::NamingContext_var domain_nc;

      // Resolve DomainNC
      try
        {
          if (this->options_.domain_nc_)
            {
              DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::create_object - "
                            "before resolving \"DomainNC\".\n"));
              CORBA::Object_var domain_obj = orb->string_to_object (this->options_.domain_nc_);
              if (!CORBA::is_nil (domain_obj.in ()))
                {
                  domain_nc = CosNaming::NamingContext::_narrow (domain_obj.in());
                  if (CORBA::is_nil (domain_nc.in ()))
                    {
                      DANCE_ERROR ( (LM_ERROR,
                                     DLINFO "Narrow to NamingContext return nil for DomainNC.\n"));
                      return CORBA::Object::_nil ();
                    }
                }
            }
        }
      catch (CORBA::Exception&)
        {
          DANCE_DEBUG ((LM_DEBUG,
                        DLINFO "DomainNC context not found!\n"));
        }

      // Initialize IOR table
      CORBA::Object_var table_object
        = orb->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter
        = IORTable::Table::_narrow (table_object.in ());

      if (CORBA::is_nil (adapter.in ()))
        {
          DANCE_ERROR ( (LM_ERROR,
                         DLINFO "Nil IORTable\n"));
          return CORBA::Object::_nil ();
        }

      // Create and install the DAnCE Daemon servant on child POA
      DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::create_object - before creating EM servant.\n"));
      ACE_NEW_RETURN (this->em_impl_,
                      DAnCE::ExecutionManager_Impl (orb,
                                                    poa.in (),
                                                    domain_nc.in ()),
                      CORBA::Object::_nil ());

      // Explicit activation through the persistent POA
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId ("ExecutionManager");
      persistent_poa->activate_object_with_id (oid, this->em_impl_);

      CORBA::Object_var em_obj = persistent_poa->id_to_reference (oid.in ());
      CORBA::String_var em_ior = orb->object_to_string (em_obj.in ());

      DAnCE::ExecutionManagerDaemon_var em_daemon
        = DAnCE::ExecutionManagerDaemon::_narrow (em_obj.in ());

      // Binding ior to IOR Table
      adapter->bind ("ExecutionManager", em_ior.in ());

      // Saving execution manager ior
      if (0 != this->options_.exec_mgr_file_)
        {
          DAnCE::ExecutionManager::write_IOR (this->options_.exec_mgr_file_, em_ior.in ());
        }

      // Binding execution manager to name service
      if (!CORBA::is_nil (domain_nc.in ()))
        {
          DANCE_DEBUG((LM_TRACE, DLINFO "Registering EM in NC.\n"));
          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup ("ExecutionManager");
          domain_nc->rebind (name, em_daemon.in());
        }

      // End ExecutionManager initialization part

      // Initializing NodeManagers
      DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::create_object - "
                    "before processing --node-mgr options(%u).\n", this->options_.node_managers_.size()));
      for (size_t i = 0; i < this->options_.node_managers_.size(); ++i)
        {
          size_t pos = this->options_.node_managers_[i].find ('=');
          ACE_CString node_name = this->options_.node_managers_[i];
          ACE_CString nm_ior;

          if (ACE_CString::npos == pos)
            {
              DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE_ExecutionManager_Module::create_object - "
                            "Execution manager received --node-mgr without IOR\n"));
              continue;
            }

          node_name = this->options_.node_managers_[i].substring (0, pos);
          nm_ior = this->options_.node_managers_[i].substring (pos + 1);
          /*
          CORBA::Object_var obj = orb->string_to_object (nm_ior.c_str ());
          Deployment::NodeManager_var nm_obj =
            Deployment::NodeManager::_narrow (obj.in ());

          if (CORBA::is_nil (nm_obj))
            {
              DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE_ExecutionManager::create_object - "
                             "Failed to narrow the object to node manager : %C\n", 
                             this->options_.node_managers_[i].c_str()));
              continue;
            }
          */
          DANCE_DEBUG ((LM_TRACE, DLINFO "Placing node \"%C\" to EM's map.\n", node_name.c_str()));
          this->em_impl_->add_node_manager (node_name.c_str(), nm_ior.c_str ());
        }
      
      if (this->options_.node_map_ != 0)
        {
          DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_ExecutionManager_Module::create_object - "
                        "Parsing node map %C\n",
                        this->options_.node_map_));
          this->em_impl_->load_node_map (this->options_.node_map_);
        }
      

      mgr->activate ();

      return em_obj._retn ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("DAnCE_ExecutionManager::run_main\n");
      return CORBA::Object::_nil ();
    }
}

ACE_FACTORY_DEFINE (DAnCE_ExecutionManager_Module, DAnCE_ExecutionManager_Module)



