// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS.h"

#include "tao/TAO_Singleton_Manager.h"
#include "tao/StringSeqC.h"
#include "tao/IORTable/IORTable.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/orbsvcs/Naming/Naming_Loader.h"
#include "ciao/CIAO_common.h"
#include "ciao/Valuetype_Factories/Cookies.h"
#include "ciao/ComponentServer/CIAO_PropertiesC.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/Deployment/DAnCE_PropertiesC.h"
#include "Node_Manager_Module.h"
#include "NodeManager_Impl.h"
#include "RedirectionService/RedirectionService.h"

ACE_RCSID (DAnCE,
           Node_Manager_Module,
           "$Id$")

namespace DAnCE
{
  namespace Node_Manager
  {
    bool
    write_IOR (const ACE_TCHAR * ior_file_name, const char* ior)
    {
      FILE* ior_output_file_ =
        ACE_OS::fopen (ior_file_name, "w");

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

DAnCE_NodeManager_Module::DAnCE_NodeManager_Module (void)
  : redirection_ (0)
{
  DANCE_TRACE("DAnCE_NodeManager_Module::DAnCE_NodeManager_Module");
}

DAnCE_NodeManager_Module::~DAnCE_NodeManager_Module (void)
{
  DANCE_TRACE ("DAnCE_NodeManager_Module::~DAnCE_NodeManager_Module");

  for (Servant_Map::iterator it = this->nm_map_.begin ();
       it != this->nm_map_.end ();
       ++it)
    {
      delete (*it).int_id_;
    }

  delete this->redirection_;
}

const char *
DAnCE_NodeManager_Module::usage (void)
{
  DANCE_TRACE ("DAnCE_NodeManager_Module::usage");
  return "Node Manager Options:\n"
    "\t-e,--exec-mgr\t\t\t [execution manager ior file name]\n"
    "\t-n,--node-mgr\t\t\t <node name> [=node manager ior file name]\n"
    "\t-p,--process-ns\t\t\t [file name] create process name service and store its ior to file name\n"
    "\t-c,--create-plan-ns\t\t [NC] create plan objects (components and ports) representation in name context with ior NC\n"
    "\t-r,--rebind-plan-ns\t\t [NC] bind plan representation name context to NC\n"
    "\t-i,--port-indirection\t\t enable plan objects indirection via servant locator\n"
    "\t-f,--ignore-failure\t\t ignore deployment failures\n"
    "\t-s,--server-executable\t\t default component server executable\n"
    "\t--server-args\t\t additional arguments to supply to the component server\n"
    "\t--standalone-nm\t\t Indicates that this NodeManager is not managed by an ExecutionManager\n"
    "\t-t,--timeout\t\t\t default timeout in seconds to wait for component server spawn\n"
    "\t-h,help\t\t\t\t print this help message\n";
}

bool
DAnCE_NodeManager_Module::parse_args (int argc, ACE_TCHAR * argv[])
{
  ACE_Get_Opt get_opts (argc,
                        argv,
                        "e:n:p::c::r::ifs:t:h",
                        0,
                        0,
                        ACE_Get_Opt::RETURN_IN_ORDER);

  get_opts.long_option ("exec-mgr", 'e', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option ("node-mgr", 'n', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option ("process-ns", 'p', ACE_Get_Opt::ARG_OPTIONAL);
  get_opts.long_option ("create-plan-ns", 'c', ACE_Get_Opt::ARG_OPTIONAL);
  get_opts.long_option ("rebind-plan-ns", 'r', ACE_Get_Opt::ARG_OPTIONAL);
  get_opts.long_option ("port-indirection", 'i', ACE_Get_Opt::NO_ARG);
  get_opts.long_option ("ignore-failure", 'f', ACE_Get_Opt::NO_ARG);
  get_opts.long_option ("server-executable", 's', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option ("server-args", ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option ("standalone-nm", ACE_Get_Opt::NO_ARG);
  get_opts.long_option ("timeout", 't', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option ("help", 'h', ACE_Get_Opt::NO_ARG);
  //get_opts.long_option ("help", '?');

  char c;
  while ( (c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'n':
          DANCE_DEBUG ((LM_DEBUG, DLINFO "Node_Manager_Module::parse_args - "
                        "Provided Node Manager name: %s\n", get_opts.opt_arg ()));
          this->options_.node_managers_.push_back (get_opts.opt_arg ());
          break;

        case 'p':
          DANCE_DEBUG ((LM_DEBUG, DLINFO "Node_Manager_Module::parse_args - "
                        "Instructed to create process ns with file %s\n", get_opts.opt_arg ()));
          this->options_.process_ns_ = true;
          this->options_.process_ns_file_ = get_opts.opt_arg ();
          break;

        case 'c':
          DANCE_DEBUG ((LM_DEBUG, DLINFO "Node_Manager_Module::parse_args - "
                        "Instructed to create plan NS in context: %s\n", get_opts.opt_arg ()));
          this->options_.create_plan_ns_ = true;
          this->options_.create_plan_ns_ior_ = get_opts.opt_arg ();
          break;

        case 'r':
          DANCE_DEBUG ((LM_DEBUG, DLINFO "Node_Manager_Module::parse_args - "
                        "Instructed to rebind plan NS in context: %s\n", get_opts.opt_arg ()));
          this->options_.rebind_plan_ns_ = true;
          this->options_.rebind_plan_ns_ior_ = get_opts.opt_arg ();
          break;

        case 'i':
          DANCE_DEBUG ((LM_DEBUG, DLINFO "Node_Manager_Module::parse_args - "
                        "Instructed to create port indirection service\n"));
          this->options_.port_indirection_ = true;
          break;

        case 'f':
          DANCE_DEBUG ((LM_DEBUG, DLINFO "Node_Manager_Module::parse_args - "
                        "Instructed to ignore deployment errors\n"));
          this->options_.ignore_failure_ = true;
          break;

        case 's':
          DANCE_DEBUG ((LM_DEBUG, DLINFO "Node_Manager_Module::parse_args - "
                        "Using provided component server executable:%s\n",
                        get_opts.opt_arg ()));
          this->options_.cs_path_ = get_opts.opt_arg ();
          break;

        case 't':
          DANCE_DEBUG ((LM_DEBUG, DLINFO "Node_Manager_Module::parse_args - "
                        "Using provided component server spawn timeout:%s\n",
                        get_opts.opt_arg ()));
          this->options_.timeout_ = ACE_OS::atoi (get_opts.opt_arg ());
          break;

        case 'h':
          //case '?': // Display help for use of the server.
          //default:
          DANCE_ERROR_RETURN ( (LM_ERROR,
                                this->usage (),
                                argv [0], c),
                               false);
          break;

        case 0:
          if (ACE_OS::strcmp (get_opts.long_option (),
                              "standalone-nm") == 0)
            {
              DANCE_DEBUG ((LM_DEBUG, DLINFO "Node_Manager_Module::parse_args - "
                            "Found option directing NodeManager to run as standalone entity.\n"));
              this->options_.standalone_ = true;
            }
          else if (ACE_OS::strcmp (get_opts.long_option (),
                                   "server-args") == 0)
            {
              DANCE_DEBUG ((LM_DEBUG, DLINFO "Node_Manager_Module::parse_args - "
                            "Using provided compoent server arguments: '%s'\n",
                            get_opts.opt_arg ()));
              this->options_.server_args_ = get_opts.opt_arg ();
            }
          break;

        default:
          DANCE_DEBUG ((LM_TRACE, DLINFO "Node_Manager_Module::parse_args - ignoring unknown option %c\n",
                        c));
        }
    }
  return true;
}

CORBA::Object_ptr
DAnCE_NodeManager_Module::create_object (CORBA::ORB_ptr orb,
                                         int argc,
                                         ACE_TCHAR *argv[])
{
  try
    {
      if (CORBA::is_nil(orb))
        {
          DANCE_ERROR((LM_ERROR, DLINFO "DAnCE_NodeManager_Module::create_object - "
                       "Attempted to create Node Manager with a nil orb.\n"));
          return CORBA::Object::_nil();
        }

      if (CORBA::is_nil(this->orb_.in()))
        {
          DANCE_DEBUG((LM_TRACE, DLINFO "DAnCE_NodeManager_Module::create_object - "
                       "NM's orb is nil.\n"));
          this->orb_ = CORBA::ORB::_duplicate (orb);
        }
      else if (ACE_OS::strcmp(orb->id(), this->orb_->id()) != 0)
        {
          DANCE_DEBUG((LM_TRACE, DLINFO "DAnCE_NodeManager_Module::create_object - "
                       "Resetting NM's orb.\n"));
          this->orb_ = CORBA::ORB::_duplicate (orb);
          this->domain_nc_ = CosNaming::NamingContext::_nil();
        }

      if (!this->parse_args (argc, argv))
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE_NodeManager_Module::create_object - "
                        "Failed to parse command line arguments, exiting\n"));
          return CORBA::Object::_nil ();
        }


      /* @@CDMW stuff, figure out what to do with this later....
         try
         {
         CORBA::Object_var obj = orb->resolve_initial_references ("ProcessDestinationNC");
         if (CORBA::is_nil(obj))
         {
         DANCE_ERROR((LM_ERROR, DLINFO "DAnCE_NodeManager_Module::create_object - "
         "ProcessDestinationNC can not be resolved. RIR returned nil.\n"));
         return CORBA::Object::_nil();
         }

         CosNaming::NamingContext_var pdnc = CosNaming::NamingContext::_narrow(obj.in());
         if (CORBA::is_nil(pdnc.in()))
         {
         DANCE_ERROR((LM_ERROR, DLINFO "DAnCE_NodeManager_Module::create_object - "
         "ProcessDestinationNC can not be narrowed to NC.\n"));
         return CORBA::Object::_nil();
         }
         }
         catch (CORBA::ORB::InvalidName &)
         {
         DANCE_ERROR((LM_ERROR, DLINFO "DAnCE_NodeManager_Module::create_object - "
         "ProcessDestinationNC can not be resolved. InvalidName.\n"));
         return CORBA::Object::_nil();
         }
         catch(...)
         {
         DANCE_ERROR((LM_ERROR, DLINFO "DAnCE_NodeManager_Module::create_object - "
         "ProcessDestinationNC can not be retrieved. Unknown exception.\n"));
         return CORBA::Object::_nil();
         }
      */


      this->register_value_factories ();
      this->create_poas ();

      // Resolve DomainNC
      if (CORBA::is_nil(this->orb_.in()))
        {
          try
            {
              DANCE_DEBUG((LM_TRACE, DLINFO "DAnCE_NodeManager_Module::create_object - "
                           "Setting NM's orb and resolving DomainNC.\n"));
              this->orb_ = CORBA::ORB::_duplicate(orb);
              CORBA::Object_var domain_obj = orb->resolve_initial_references ("DomainNC");
              if (!CORBA::is_nil (domain_obj.in ()))
                {
                  this->domain_nc_ = CosNaming::NamingContext::_narrow (domain_obj.in());
                  if (CORBA::is_nil (this->domain_nc_.in ()))
                    {
                      DANCE_ERROR ((LM_ERROR,DLINFO "DAnCE_NodeManager_Module::create_object - "
                                    "Narrow to NamingContext return nil for DomainNC.\n"));
                      return CORBA::Object::_nil ();
                    }
                }
            }
          catch (CORBA::Exception&)
            {
              DANCE_DEBUG  ((LM_WARNING, DLINFO "DAnCE_NodeManager_Module::create_object - "
                             "DomainNC context not found!\n"));
            }
        }

      DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_NodeManager_Module::create_object - "
                    "Resolving plan NS naming context.\n"));
      CosNaming::NamingContext_var rebind_nc;
      if (this->options_.rebind_plan_ns_)
        {
          if (!this->options_.create_plan_ns_)
            {
              DANCE_ERROR((LM_ERROR, DLINFO "DAnCE_NodeManager_Module::create_object - "
                           "Rebind_plan_ns option is enabled but Create_plan_ns is not.\n"));
              return CORBA::Object::_nil();
            }
          CORBA::Object_var tmp_obj;
          if (0 != this->options_.rebind_plan_ns_ior_)
            {
              tmp_obj = orb->string_to_object (this->options_.rebind_plan_ns_ior_);
              rebind_nc = CosNaming::NamingContext::_narrow (tmp_obj);
            }
          else
            {
              if (CORBA::is_nil (this->domain_nc_.in ()))
                {
                  DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE_NodeManager_Module::create_object - "
                                "The rebind plan is enabled but neither NC "
                                "nor DomainNC are not supplied. No rebinding will be done.\n"
                                "Use the \"-rebind-plan-ns NC ior\" "
                                "or \"-ORBInitRef DomainNC\" option.\n"));
                  return CORBA::Object::_nil ();
                }
              rebind_nc = CosNaming::NamingContext::_duplicate (this->domain_nc_.in ());
            }
        }

      // Initialize IOR table
      CORBA::Object_var table_object = orb->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter = IORTable::Table::_narrow (table_object.in ());

      if (CORBA::is_nil (adapter.in ()))
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE_NodeManager_Module::create_object - "
                         "Unable to RIR the IORTable.\n"));
          return CORBA::Object::_nil ();
        }

      // Creating in process nameservice
      CosNaming::NamingContext_var plan_nc;
      TAO_Naming_Loader loader;
      if (this->options_.create_plan_ns_)
        {
          DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_NodeManager_Module::create_object - "
                        "Resolving plan-specific naming context.\n"));
          CORBA::Object_var naming_obj;
          if (0 != this->options_.create_plan_ns_ior_)
            {
              naming_obj = orb->string_to_object (this->options_.create_plan_ns_ior_);
              plan_nc = CosNaming::NamingContext::_narrow (naming_obj.in());
              if (CORBA::is_nil (plan_nc.in()))
                {
                  DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE_NodeManager_Module::create_object - "
                                "Failed trying to narrow naming context "
                                "for dance creating plan NC.\n"
                                "Use the \"--create-plan-ns NC ior\" "
                                "or \"--process-ns\" option.\n"));
                  return CORBA::Object::_nil ();
                }
            }
          else if (this->options_.process_ns_)
            {
              const int argc_ns = 1;
              char* argv_ns[argc_ns];
              argv_ns[0] = argv[0];
              naming_obj = orb->resolve_initial_references ("NameService");
              if (0 != this->options_.process_ns_file_)
                {
                  CORBA::String_var ior = orb->object_to_string (naming_obj.in ());
                  DAnCE::Node_Manager::write_IOR (this->options_.process_ns_file_, ior.in ());
                }
              plan_nc = CosNaming::NamingContext::_narrow (naming_obj.in ());
              if (CORBA::is_nil (plan_nc.in ()))
                {
                  DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE_NodeManager_Module::create_object - "
                                "Fails trying to narrow naming context "
                                "for dance creating plan NC.\n"
                                "Use the \"--create-plan-ns NC ior\" "
                                "or \"--process-ns\" option.\n"));
                  return CORBA::Object::_nil ();
                }
            }
          else if (CORBA::is_nil (this->domain_nc_.in ()))
            {
              DANCE_ERROR ((LM_ERROR,DLINFO "DAnCE_NodeManager_Module::create_object - "
                            "The create plan is enabled but neither NC ior "
                            "nor --process-ns or DomainNC option are not supplied.\n"
                            "Use the \"--create-plan-ns NC ior\", \"--process-ns\" "
                            "or \"-ORBInitRef DomainNC\" option.\n"));
              return CORBA::Object::_nil ();
            }
          else
            {
              plan_nc = CosNaming::NamingContext::_duplicate (this->domain_nc_.in ());
            }
        }

      //Creating redirection object
      if (this->redirection_ == 0)
        {
          DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_NodeManager_Module::create_object - "
                        "Creating redirection service object\n"));
          this->redirection_ = new DAnCE::RedirectionService (orb,
                                                              this->root_poa_.in (),
                                                              plan_nc.in (),
                                                              rebind_nc.in (),
                                                              this->options_.create_plan_ns_,
                                                              this->options_.port_indirection_);
        }

      // Make sure that we have only one Node Manager
      if (this->options_.node_managers_.size () != 1)
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE_NodeManager_Module::create_object - "
                        "For now only one node manager creation is supported.\n"));
          return CORBA::Object::_nil ();
        }

      DANCE_DEBUG ((LM_DEBUG, DLINFO "DAnCE_NodeManager_Module::create_object - "
                    "DAnCE_NodeManager::run_main - creating NodeManager for node %C\n",
                    this->options_.node_managers_[0].c_str()));

      // Parsing Node name and node manager ior file name
      ACE_CString node_name = this->options_.node_managers_[0].c_str ();
      ACE_CString node_file;
      size_t npos = node_name.find ('=');
      if (ACE_CString::npos != npos)
        {
          node_file = node_name.substring (npos + 1, node_name.length() - npos + 1);
          node_name = node_name.substring (0, npos);
        }

      DANCE_DEBUG ((LM_INFO, DLINFO "DAnCE_NodeManager_Module::create_object - "
                    "Creating node named '%s' and outputting ior to file '%s'\n",
                    node_name.c_str (),
                    node_file.c_str ()));

      //Creating node manager servant
      DAnCE::NodeManager_Impl * nm = 0;
      if (this->nm_map_.find (node_name, nm) == -1)
        {
          DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_NodeManager_Module::create_object - "
                        "Allocating new NodeManger servant instance for NodeManager\n"));
          int size = 64;
          DAnCE::PROPERTY_MAP properties (size);
          this->create_nm_properties (properties);

          ACE_NEW_RETURN (nm,
                          DAnCE::NodeManager_Impl (orb,
                                                   this->root_poa_.in (),
                                                   node_name.c_str(),
                                                   *this->redirection_,
                                                   properties),
                          CORBA::Object::_nil ());
          DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_NodeManager_Module::create_object - "
                        "New NodeManger servant instance for NodeManager allocated.\n"));
          this->nm_map_.bind (node_name, nm);
        }

      ACE_CString node_manager_oid = node_name + ".NodeManager";

      // Registering servant in poa
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId (node_manager_oid.c_str());
      this->nm_poa_->activate_object_with_id (oid, nm);

      // Getting node manager ior
      CORBA::Object_var nm_obj = this->nm_poa_->id_to_reference (oid.in ());
      CORBA::String_var ior = orb->object_to_string (nm_obj.in ());

      // Binding ior to IOR Table
      adapter->bind (node_manager_oid.c_str (), ior.in ());

      // Binding node menager to DomainNC
      if (!CORBA::is_nil (this->domain_nc_.in ()))
        {
          DANCE_DEBUG((LM_TRACE, DLINFO "DAnCE_NodeManager_Module::create_object - "
                       "Registering NM in NC as \"%s.NodeManager\".\n", node_name.c_str ()));
          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup (node_name.c_str ());
          name[0].kind = CORBA::string_dup ("NodeManager");
          this->domain_nc_->rebind (name, nm_obj.in ());
        }

      // Writing ior to file
      if (0 != node_file.length ())
        {
          DANCE_DEBUG ((LM_TRACE,  DLINFO "DAnCE_NodeManager_Module::create_object - "
                        "Writing node IOR to file.\n"));
          DAnCE::Node_Manager::write_IOR (node_file.c_str (), ior.in ());
        }

      // Activate POA manager
      PortableServer::POAManager_var mgr = this->root_poa_->the_POAManager ();
      mgr->activate ();

      // Finishing Deployment part
      DANCE_DEBUG ((LM_DEBUG, DLINFO "DAnCE_NodeManager_Module::create_object - "
                    "DAnCE_NodeManager is running...\n"));

      return nm_obj._retn ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("DAnCE_NodeManager::main\t\n");
      return CORBA::Object::_nil ();
    }
}

void
DAnCE_NodeManager_Module::register_value_factories (void)
{
  DANCE_TRACE("DAnCE_Node_Manager_Module::register_value_factories (void)");
  DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_NodeManager_Module::create_object - "
                "Registering Cookie value factory.\n"));
  CORBA::ValueFactory_var vf = new Components::Cookie_init();
  vf = this->orb_->register_value_factory ("IDL:omg.org/Components/Cookie:1.0", vf.in());
}

void
DAnCE_NodeManager_Module::create_poas (void)
{
  DANCE_TRACE("DAnCE_Node_Manager_Module::create_poas");
  // Get reference to Root POA.
  DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_NodeManager_Module::create_poas - "
                "Resolving root POA\n"));
  CORBA::Object_var obj = this->orb_->resolve_initial_references ("RootPOA");

  this->root_poa_ = PortableServer::POA::_narrow (obj.in ());

  DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_NodeManager_Module::create_poas - "
                "Obtaining the POAManager\n"));
  PortableServer::POAManager_var mgr = this->root_poa_->the_POAManager ();

  TAO::Utils::PolicyList_Destroyer policies (2);
  policies.length (2);

  try
    {
      DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_NodeManager_Module::create_poas - "
                    "DAnCE_NodeManager_Module::create_poas - "
                    "Creating the \"Managers\" POA.\n"));

      policies[0] = this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);
      policies[1] = this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);
      this->nm_poa_ = this->root_poa_->create_POA ("Managers",
                                       mgr.in(),
                                       policies);
    }
  catch (const PortableServer::POA::AdapterAlreadyExists &)
    {
      DANCE_DEBUG ((LM_INFO, DLINFO "DAnCE_NodeManager_Module::create_poas - "
                    "Using exiswting \"Managers\" POA\n"));
      this->nm_poa_ = this->root_poa_->find_POA ("Managers", 0);
    }
}

void
DAnCE_NodeManager_Module::create_nm_properties (DAnCE::PROPERTY_MAP &props)
{
  {
    CORBA::Any val;
    val <<= this->options_.timeout_;
    props.bind (CIAO::Deployment::SERVER_TIMEOUT,
                val);
  }
  {
    CORBA::Any val;
    val <<= CORBA::Any::from_string (CORBA::string_dup (this->options_.cs_path_),0);
    props.bind (CIAO::Deployment::SERVER_EXECUTABLE,
                val);
  }
  {
    CORBA::Any val;
    val <<= CORBA::Any::from_boolean (this->options_.standalone_);
    props.bind (DAnCE::STANDALONE_NM, val);
  }
  {
    CORBA::Any val;
    val <<= CORBA::Any::from_string (CORBA::string_dup (this->options_.server_args_),0);
    props.bind (CIAO::Deployment::SERVER_ARGUMENTS, val);
  }
}

#ifndef BUILD_NODE_MANAGER_EXE
ACE_FACTORY_DEFINE (DAnCE_NodeManager_Module, DAnCE_NodeManager_Module)
#endif /*BUILD_NODE_MANAGER_EXE */
