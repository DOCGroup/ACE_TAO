// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS.h"

#include "tao/TAO_Singleton_Manager.h"
#include "tao/StringSeqC.h"
#include "tao/IORTable/IORTable.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/orbsvcs/Naming/Naming_Loader.h"
#include "ciao/CIAO_common.h"
#include "ciao/Cookies.h"

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
    struct SOptions
      {
        bool process_ns_;
        const char* process_ns_file_;
        bool create_plan_ns_;
        const char* create_plan_ns_ior_;
        bool rebind_plan_ns_;
        const char* rebind_plan_ns_ior_;
        bool port_indirection_;
        ACE_Vector<ACE_CString> node_managers_;
        bool ignore_failure_;

        SOptions()
            : process_ns_ (false)
            , process_ns_file_ (0)
            , create_plan_ns_ (false)
            , create_plan_ns_ior_ (0)
            , rebind_plan_ns_ (false)
            , rebind_plan_ns_ior_ (0)
            , ignore_failure_ (false)
        {
        }
      };

    bool
    parse_args (int argc, char *argv[], SOptions &options)
    {
      ACE_Get_Opt get_opts (argc,
                            argv,
                            "n:p::c::r::ifh",
                            0,
                            0,
                            ACE_Get_Opt::RETURN_IN_ORDER);

      get_opts.long_option ("node-mgr", 'n', ACE_Get_Opt::ARG_REQUIRED);
      get_opts.long_option ("process-ns", 'p', ACE_Get_Opt::ARG_OPTIONAL);
      get_opts.long_option ("create-plan-ns", 'c', ACE_Get_Opt::ARG_OPTIONAL);
      get_opts.long_option ("rebind-plan-ns", 'r', ACE_Get_Opt::ARG_OPTIONAL);
      get_opts.long_option ("port-indirection", 'i', ACE_Get_Opt::NO_ARG);
      get_opts.long_option ("ignore-failure", 'f', ACE_Get_Opt::NO_ARG);
      get_opts.long_option ("help", 'h', ACE_Get_Opt::NO_ARG);
      //get_opts.long_option ("help", '?');

      char c;
      while ( (c = get_opts ()) != -1)
        {
          switch (c)
            {
            case 'n':
              options.node_managers_.push_back (get_opts.opt_arg ());
              break;

            case 'p':
              options.process_ns_ = true;
              options.process_ns_file_ = get_opts.opt_arg ();
              break;

            case 'c':
              options.create_plan_ns_ = true;
              options.create_plan_ns_ior_ = get_opts.opt_arg ();
              break;

            case 'r':
              options.rebind_plan_ns_ = true;
              options.rebind_plan_ns_ior_ = get_opts.opt_arg ();
              break;

            case 'i':
              options.port_indirection_ = true;
              break;

            case 'f':
              options.ignore_failure_ = true;
              break;

            case 'h':
              //case '?': // Display help for use of the server.
              //default:
              ACE_ERROR_RETURN ( (LM_ERROR,
                                  "usage:  %s\n"
                                  "-e [execution manager ior file name]\n"
                                  "-n <node name> [=node manager ior file name]\n"
                                  "-p [file name] create process name service and store its ior to file name\n"
                                  "-c [NC] create plan objects (components and ports) representation in name context with ior NC\n"
                                  "-r [NC] bind plan representation name context to NC\n"
                                  "-i enable plan objects indirection via servant locator\n"
                                  "invalid option: \" %c \"\n",
                                  argv [0], c),
                                 false);
              break;
            }
        }
      return true;
    }

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
          return true;
        }
      return false;
    }
  }
}

DAnCE_NodeManager_Module::DAnCE_NodeManager_Module (void)
    : redirection_ (0)
{
  ACE_ERROR ( (LM_TRACE, "[%M] DAnCE_NodeManager_Module constructor.\n"));
}

DAnCE_NodeManager_Module::~DAnCE_NodeManager_Module (void)
{
  ACE_ERROR ( (LM_TRACE, "[%M] ~DAnCE_NodeManager_Module starting...\n"));
  for (Servant_Map::iterator it = this->nm_map_.begin ();
       it != this->nm_map_.end ();
       ++it)
    {
      delete (*it).int_id_;
    }

  delete this->redirection_;
  ACE_ERROR ( (LM_TRACE, "[%M] ~DAnCE_NodeManager_Module finished.\n"));
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
          ACE_ERROR((LM_ERROR, "[%M] Creating NM with a nil orb.\n"));
          return CORBA::Object::_nil();
        }
      if (CORBA::is_nil(this->orb_.in())) ACE_DEBUG((LM_TRACE, "[%M] NM's orb is nil.\n"));
      if (CORBA::is_nil(this->orb_.in()) || 0 != ACE_OS::strcmp(orb->id(), this->orb_->id()))
        {
          ACE_DEBUG((LM_TRACE, "[%M] Resetting NM's orb.\n"));
          this->orb_ = CORBA::ORB::_nil();
          this->domain_nc_ = CosNaming::NamingContext::_nil();
        }

      DAnCE::Node_Manager::SOptions options;
      if (!DAnCE::Node_Manager::parse_args (argc, argv, options))
        return CORBA::Object::_nil ();

      try
      {
        CORBA::Object_var obj = orb->resolve_initial_references ("ProcessDestinationNC");
        if (CORBA::is_nil(obj))
          {
            ACE_ERROR((LM_ERROR, "[%M] ProcessDestinationNC can not be resolved. RIR returned nil.\n"));
            return CORBA::Object::_nil();
          }

        CosNaming::NamingContext_var pdnc = CosNaming::NamingContext::_narrow(obj.in());
        if (CORBA::is_nil(pdnc.in()))
          {
            ACE_ERROR((LM_ERROR, "[%M] ProcessDestinationNC can not be narrowed to NC.\n"));
            return CORBA::Object::_nil();
          }
      }
      catch (CORBA::ORB::InvalidName &)
      {
        ACE_ERROR((LM_ERROR, "[%M] ProcessDestinationNC can not be resolved. InvalidName.\n"));
        return CORBA::Object::_nil();
      }
      catch(...)
      {
        ACE_ERROR((LM_ERROR, "[%M] ProcessDestinationNC can not be retrieved. Unknown exception.\n"));
        return CORBA::Object::_nil();
      }

      CORBA::ValueFactory_var vf = new Components::Cookie_init();
      vf = orb->register_value_factory ("IDL:omg.org/Components/Cookie:1.0", vf.in());

      // Get reference to Root POA.
      CORBA::Object_var obj
      = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa
      = PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var mgr = poa->the_POAManager ();

      PortableServer::POA_var persistent_poa;
      CORBA::PolicyList policies (2);
      policies.length (2);
      try
        {
          ACE_DEBUG ( (LM_TRACE,
                       "[%M] DAnCE_NodeManager_Module::create_object - "
                       "before creating the \"Managers\" POA.\n"));

          policies[0] = poa->create_id_assignment_policy (PortableServer::USER_ID);
          policies[1] = poa->create_lifespan_policy (PortableServer::PERSISTENT);
          persistent_poa = poa->create_POA ("Managers"
                                            , mgr.in()
                                            , policies);
        }
      catch (const PortableServer::POA::AdapterAlreadyExists &)
        {
          persistent_poa = poa->find_POA ("Managers", 0);
        }

      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy ();
        }

      // Resolve DomainNC

      if (CORBA::is_nil(this->orb_.in()))
        {
          try
            {
              ACE_DEBUG((LM_TRACE, "[%M] Setting NM's orb and resolving DomainNC.\n"));
              this->orb_ = CORBA::ORB::_duplicate(orb);
              CORBA::Object_var domain_obj = orb->resolve_initial_references ("DomainNC");
              if (!CORBA::is_nil (domain_obj.in ()))
                {
                  this->domain_nc_ = CosNaming::NamingContext::_narrow (domain_obj.in());
                  if (CORBA::is_nil (this->domain_nc_.in ()))
                    {
                      ACE_ERROR ( (LM_ERROR,
                              "[%M] Narrow to NamingContext return nil for DomainNC.\n"));
                      return CORBA::Object::_nil ();
                    }
                }
            }
          catch (CORBA::Exception&)
            {
              ACE_DEBUG ( (LM_DEBUG, "[%M] DomainNC context not found!\n"));
            }
        }
      CosNaming::NamingContext_var rebind_nc;
      if (options.rebind_plan_ns_)
        {
          if (!options.create_plan_ns_)
            {
              ACE_ERROR((LM_ERROR, "[%M] Rebind_plan_ns option is enabled but Create_plan_ns is not.\n"));
              return CORBA::Object::_nil();
            }
          CORBA::Object_var tmp_obj;
          if (0 != options.rebind_plan_ns_ior_)
            {
              tmp_obj = orb->string_to_object (options.rebind_plan_ns_ior_);
              rebind_nc = CosNaming::NamingContext::_narrow (tmp_obj);
            }
          else
            {
              if (CORBA::is_nil (this->domain_nc_.in ()))
                {
                  ACE_ERROR ( (LM_ERROR,
                               "[%M] The rebind plan is enabled but neither NC "
                               "nor DomainNC are not supplied. No rebinding will be done.\n"
                               "Use the \"-rebind-plan-ns NC ior\" "
                               "or \"-ORBInitRef DomainNC\" option.\n"));
                  return CORBA::Object::_nil ();
                }
              rebind_nc = CosNaming::NamingContext::_duplicate (this->domain_nc_.in ());
            }
        }

      // Initialize IOR table
      CORBA::Object_var table_object
      = orb->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter
      = IORTable::Table::_narrow (table_object.in ());

      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ( (LM_ERROR,
                       "Nil IORTable.\n"));
          return CORBA::Object::_nil ();
        }

      // Creating in process nameservice
      CosNaming::NamingContext_var plan_nc;
      TAO_Naming_Loader loader;
      if (options.create_plan_ns_)
        {
          CORBA::Object_var naming_obj;
          if (0 != options.create_plan_ns_ior_)
            {
              naming_obj = orb->string_to_object (options.create_plan_ns_ior_);
              plan_nc = CosNaming::NamingContext::_narrow (naming_obj.in());
              if (CORBA::is_nil (plan_nc.in()))
                {
                  ACE_ERROR ( (LM_ERROR,
                               "[%M] Fails trying to narrow naming context "
                               "for dance creating plan NC.\n"
                               "Use the \"--create-plan-ns NC ior\" "
                               "or \"--process-ns\" option.\n"));
                  return CORBA::Object::_nil ();
                }
            }
          else if (options.process_ns_)
            {
              int argc_ns = 1;
              char* argv_ns[argc_ns];
              argv_ns[0] = argv[0];
              naming_obj = orb->resolve_initial_references ("NameService");
              if (0 != options.process_ns_file_)
                {
                  CORBA::String_var ior = orb->object_to_string (naming_obj.in ());
                  DAnCE::Node_Manager::write_IOR (options.process_ns_file_, ior.in ());
                }
              plan_nc = CosNaming::NamingContext::_narrow (naming_obj.in ());
              if (CORBA::is_nil (plan_nc.in ()))
                {
                  ACE_ERROR ( (LM_ERROR,
                               "[%M] Fails trying to narrow naming context "
                               "for dance creating plan NC.\n"
                               "Use the \"--create-plan-ns NC ior\" "
                               "or \"--process-ns\" option.\n"));
                  return CORBA::Object::_nil ();
                }
            }
          else if (CORBA::is_nil (this->domain_nc_.in ()))
            {
              ACE_ERROR ( (LM_ERROR,
                           "[%M] The create plan is enabled but neither NC ior "
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
          this->redirection_ = new DAnCE::RedirectionService (orb,
                                                              poa.in (),
                                                              plan_nc.in (),
                                                              rebind_nc.in (),
                                                              options.create_plan_ns_,
                                                              options.port_indirection_);
        }

      // Make sure that we have only one Node Manager
      if (options.node_managers_.size () != 1)
        {
          ACE_ERROR ( (LM_ERROR,
                       "[%M] For now only one node manager creation is supported.\n"));
          return CORBA::Object::_nil ();
        }

      ACE_DEBUG ( (LM_DEBUG,
                   "[%M] DAnCE_NodeManager::run_main - creating NodeManager for node %C\n",
                   options.node_managers_[0].c_str()));

      // Parsing Node name and node manager ior file name
      ACE_CString node_name = options.node_managers_[0].c_str ();
      ACE_CString node_file;
      size_t npos = node_name.find (' ');
      if (ACE_CString::npos != npos)
        {
          node_file = node_name.substring (npos + 1, node_name.length() - npos + 1);
          node_name = node_name.substring (0, npos);
        }

      //Creating node manager servant
      DAnCE::NodeManager_Impl * nm = 0;
      if (this->nm_map_.find (node_name, nm) == -1)
        {
          ACE_NEW_RETURN (nm,
                          DAnCE::NodeManager_Impl (orb,
                                                   poa.in (),
                                                   node_name.c_str(),
                                                   *this->redirection_),
                          CORBA::Object::_nil ());

          this->nm_map_.bind (node_name, nm);
        }

      ACE_CString node_manager_oid = node_name + ".NodeManager";

      // Registering servant in poa
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId (node_manager_oid.c_str());
      persistent_poa->activate_object_with_id (oid, nm);

      // Getting node manager ior
      CORBA::Object_var nm_obj = persistent_poa->id_to_reference (oid.in ());
      CORBA::String_var ior = orb->object_to_string (nm_obj.in ());

      // Binding ior to IOR Table
      adapter->bind (node_manager_oid.c_str (), ior.in ());

      // Binding node menager to DomainNC
      if (!CORBA::is_nil (this->domain_nc_.in ()))
        {
          ACE_DEBUG((LM_TRACE, "[%M] Registering NM in NC as \"%s.NodeManager\".\n", node_name.c_str ()));
          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup (node_name.c_str ());
          name[0].kind = CORBA::string_dup ("NodeManager");
          this->domain_nc_->rebind (name, nm_obj.in ());
        }

      // Writing ior to file
      if (0 != node_file.length ())
        {
          node_file += ".ior";
          DAnCE::Node_Manager::write_IOR (node_file.c_str (), ior.in ());
        }

      // Activate POA manager
      mgr->activate ();

      // Finishing Deployment part
      ACE_DEBUG ( (LM_DEBUG, "[%M] DAnCE_NodeManager is running...\n"));

      return nm_obj._retn ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("DAnCE_NodeManager::main\t\n");
      return CORBA::Object::_nil ();
    }
}

ACE_FACTORY_DEFINE (DAnCE_NodeManager_Module, DAnCE_NodeManager_Module)
