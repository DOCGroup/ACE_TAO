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
#include "orbsvcs/orbsvcs/LWFT/ReplicationManagerC.h"
#include "ciao/CIAO_common.h"
#include "ciao/Valuetype_Factories/Cookies.h"
#include "ciao/Base/CIAO_PropertiesC.h"
#include "dance/Logger/Log_Macros.h"
#include "dance/DAnCE_PropertiesC.h"
#include "FaultCorrelationManager_Module.h"
#include "FaultCorrelationManager_Impl.h"


const char * FCM_OID = "FaultCorrelationManager";

namespace DAnCE
{
  namespace FCM
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

FaultCorrelationManager_Module::FaultCorrelationManager_Module (void)
{
  DANCE_TRACE("FaultCorrelationManager_Module::FaultCorrelationManager_Module");
}

FaultCorrelationManager_Module::~FaultCorrelationManager_Module (void)
{
  DANCE_TRACE ("FaultCorrelationManager_Module::~FaultCorrelationManager_Module");
}

const char *
FaultCorrelationManager_Module::usage (void)
{
  DANCE_TRACE ("FaultCorrelationManager_Module::usage");
  return "Node Manager Options:\n"
    "\t-o,--ior-file\t\t [ior file name for FCM reference]\n"
    "\t-e,--exec-mgr\t\t [execution manager ior file name]\n"
    "\t-r,--rep-mgr\t\t [replication manager ior file name]\n"
    "\t-d,--domain-nc [NC]\t Default naming context for domain objects.\n"
    "\t--instance-nc [NC]\t Default naming context for instance registration directives. No argument indicates Domain NC.\n"
    "\t-h,help\t\t\t print this help message\n";
}

bool
FaultCorrelationManager_Module::parse_args (int argc, ACE_TCHAR * argv[])
{
  ACE_Get_Opt get_opts (argc,
                        argv,
                        ACE_TEXT("d:r:e:o:p::c::h"),
                        0,
                        0,
                        ACE_Get_Opt::RETURN_IN_ORDER);

  get_opts.long_option (ACE_TEXT("ior-file"), 'o', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("exec-mgr"), 'e', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("rep-mgr"), 'r', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("process-ns"), 'p', ACE_Get_Opt::ARG_OPTIONAL);
  get_opts.long_option (ACE_TEXT("create-plan-ns"), 'c', ACE_Get_Opt::ARG_OPTIONAL);
  get_opts.long_option (ACE_TEXT("domain-nc"), 'd', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("help"), 'h', ACE_Get_Opt::NO_ARG);
  get_opts.long_option (ACE_TEXT("instance-nc"), ACE_Get_Opt::ARG_REQUIRED);

  char c;
  while ( (c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'p':
          DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                         ACE_TEXT("FaultCorrelationManager_Module::parse_args - ")
                        ACE_TEXT("Instructed to create process ns with file %C\n"), get_opts.opt_arg ()));
          this->options_.process_ns_ = true;
          this->options_.process_ns_file_ = get_opts.opt_arg ();
          break;

        case 'c':
          DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::parse_args - ")
                        ACE_TEXT("Instructed to create plan NS in context: %C\n"), get_opts.opt_arg ()));
          this->options_.create_plan_ns_ = true;
          this->options_.create_plan_ns_ior_ = get_opts.opt_arg ();
          break;
          
        case 'd':
          DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::parse_args - ")
                        ACE_TEXT("Binding to provided Domain Naming Context: '%s'\n"),
                        get_opts.opt_arg ()));
          this->options_.domain_nc_ = get_opts.opt_arg ();
          break;
        case 'o':
          this->options_.ior_file_ = get_opts.opt_arg ();
          break;
        case 'e':
          this->options_.exec_mgr_ior_ = get_opts.opt_arg ();
          break;
        case 'r':
          this->options_.rep_mgr_ior_ = get_opts.opt_arg ();
          break;
          
        case 'h':
          //case '?': // Display help for use of the server.
          //default:
          DANCE_ERROR_RETURN (DANCE_LOG_TERMINAL_ERROR,(LM_ERROR,
                                this->usage (),
                                argv [0], c),
                               false);
          break;

        case 0:
          if (ACE_OS::strcmp (get_opts.long_option (),
                              "instance-nc") == 0)
            {
              if (get_opts.opt_arg () == 0)
                {
                  if (this->options_.domain_nc_ == 0)
                    {
                      DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR, (LM_ERROR, DLINFO "FaultCorrelationManager_Module::parse_args - "
                                    "ERROR: instance-nc provided no argument, but domain-nc did not appear before.\n"));
                      DANCE_ERROR_RETURN (DANCE_LOG_TERMINAL_ERROR,(LM_ERROR, this->usage (), argv[0], c), false);
                    }
                  DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::parse_args - ")
                                ACE_TEXT("Instance NC defaulting to Domain NC\n")));
                  this->options_.instance_nc_ = this->options_.domain_nc_;
                }
              else
                {
                  DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::parse_args - ")
                                ACE_TEXT("Using provided instance NC: %C\n"),
                                get_opts.opt_arg ()));
                  this->options_.instance_nc_ = get_opts.opt_arg ();
                }
            }
          else
            {
              DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR, (LM_ERROR, DLINFO ACE_TEXT("FaultCorrelationManager_Module::parse_args - ")
                            ACE_TEXT("ERROR: unknown long option %C\n"),
                            get_opts.long_option ()));
            }

          break;

        default:
          DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::parse_args - ignoring unknown option %i\n"),
                        c));
        }
    }
  return true;
}

CORBA::Object_ptr
FaultCorrelationManager_Module::create_object (CORBA::ORB_ptr orb,
                                               int argc,
                                               ACE_TCHAR *argv[])
{
  try
    {
      if (CORBA::is_nil(orb))
        {
          DANCE_ERROR(DANCE_LOG_NONFATAL_ERROR, (LM_ERROR, DLINFO "FaultCorrelationManager_Module::create_object - "
                       "Attempted to create FaultCorrelationManager with a nil orb.\n"));
          return CORBA::Object::_nil();
        }

      if (CORBA::is_nil(this->orb_.in()))
        {
          DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - ")
                       ACE_TEXT("FCM's orb is nil.\n")));
          this->orb_ = CORBA::ORB::_duplicate (orb);
        }
      else if (ACE_OS::strcmp(orb->id(), this->orb_->id()) != 0)
        {
          DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - ")
                       ACE_TEXT("Resetting FCM's orb.\n")));
          this->orb_ = CORBA::ORB::_duplicate (orb);
          this->domain_nc_ = CosNaming::NamingContext::_nil();
        }

      if (!this->parse_args (argc, argv))
        {
          DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR, (LM_ERROR, DLINFO "FaultCorrelationManager_Module::create_object - "
                        "Failed to parse command line arguments, exiting\n"));
          return CORBA::Object::_nil ();
        }

      this->register_value_factories ();
      this->create_poas ();

      if (this->options_.domain_nc_)
        {
          try
            {
              DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - ")
                           ACE_TEXT("Resolving DomainNC.\n")));
              CORBA::Object_var domain_obj = this->orb_->string_to_object (this->options_.domain_nc_);
              if (!CORBA::is_nil (domain_obj.in ()))
                {
                  this->domain_nc_ = CosNaming::NamingContext::_narrow (domain_obj.in());
                  if (CORBA::is_nil (this->domain_nc_.in ()))
                    {
                      DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR, (LM_ERROR,DLINFO "FaultCorrelationManager_Module::create_object - "
                                    "Narrow to NamingContext return nil for DomainNC.\n"));
                      return CORBA::Object::_nil ();
                    }
                }
            }
          catch (CORBA::Exception&)
            {
              DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - ")
                             ACE_TEXT("DomainNC context not found!\n")));
            }
        }

      // Initialize IOR table
      CORBA::Object_var table_object = orb->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter = IORTable::Table::_narrow (table_object.in ());

      if (CORBA::is_nil (adapter.in ()))
        {
          DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR, (LM_ERROR, DLINFO "FaultCorrelationManager_Module::create_object - "
                         "Unable to RIR the IORTable.\n"));
          return CORBA::Object::_nil ();
        }

      // Creating in process nameservice
      CosNaming::NamingContext_var plan_nc;
      TAO_Naming_Loader loader;
      if (this->options_.create_plan_ns_)
        {
          DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - ")
                        ACE_TEXT("Resolving plan-specific naming context.\n")));
          CORBA::Object_var naming_obj;
          if (0 != this->options_.create_plan_ns_ior_)
            {
              naming_obj = orb->string_to_object (this->options_.create_plan_ns_ior_);
              plan_nc = CosNaming::NamingContext::_narrow (naming_obj.in());
              if (CORBA::is_nil (plan_nc.in()))
                {
                  DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR, (LM_ERROR, DLINFO "FaultCorrelationManager_Module::create_object - "
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
                  DAnCE::FCM::write_IOR (this->options_.process_ns_file_, ior.in ());
                }
              plan_nc = CosNaming::NamingContext::_narrow (naming_obj.in ());
              if (CORBA::is_nil (plan_nc.in ()))
                {
                  DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR, (LM_ERROR, DLINFO "FaultCorrelationManager_Module::create_object - "
                                "Fails trying to narrow naming context "
                                "for dance creating plan NC.\n"
                                "Use the \"--create-plan-ns NC ior\" "
                                "or \"--process-ns\" option.\n"));
                  return CORBA::Object::_nil ();
                }
            }
          else if (CORBA::is_nil (this->domain_nc_.in ()))
            {
              DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR, (LM_ERROR,DLINFO "FaultCorrelationManager_Module::create_object - "
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

      /*
      DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - "
                    "starting thread.\n"));

      if (task_.activate () != 0)
        {
          DANCE_DEBUG ((LM_ERROR, DLINFO "FaultCorrelationManager_Module::create_object - "
                        "ERROR - could not start new thread.\n"));
        }
      */

      DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - ")
                    ACE_TEXT("creating FaultCorrelationManager\n")));

      CORBA::Object_var obj = orb->string_to_object (this->options_.exec_mgr_ior_);

      ::Deployment::ExecutionManager_var exec_mgr =
        ::Deployment::ExecutionManager::_narrow (obj.in ());

      if (CORBA::is_nil (exec_mgr.in ()))
        {
          DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR, (LM_ERROR, 
                        DLINFO "FaultCorrelationManager_Module::create_object - "
                        "could not resolve ExecutionManager.\n"));

          return CORBA::Object::_nil ();
        }

      // register FCM as a listener to ReplicationManager failure reports

      // TODO somewhere we have to unregister this again!

      obj = orb->string_to_object (this->options_.rep_mgr_ior_);
            
      ReplicationManager_var rep_mgr =
        ReplicationManager::_narrow (obj.in ());

      if (CORBA::is_nil (rep_mgr.in ()))
        {
          DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - ")
                        ACE_TEXT("could not resolve ReplicationManager.\n")));

          return CORBA::Object::_nil ();
        }

      //Creating node manager servant
      DAnCE::FaultCorrelationManager_Impl * fcm = 0;

      int size = 64;
      DAnCE::PROPERTY_MAP properties (size);
      this->create_fcm_properties (properties);

      ACE_NEW_RETURN (fcm,
                      DAnCE::FaultCorrelationManager_Impl (orb,
                                                           exec_mgr.in (),
                                                           rep_mgr.in (),
                                                           properties),
                      CORBA::Object::_nil ());

      DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - ")
                    ACE_TEXT("New FaultCorrelationMananger servant instance for NodeManager allocated.\n")));

      PortableServer::ServantBase_var owner_transfer (fcm);
  
      // Registering servant in poa
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId (FCM_OID);
      this->fcm_poa_->activate_object_with_id (oid, fcm);
  
      // Getting node manager ior
      CORBA::Object_var fcm_obj = this->fcm_poa_->id_to_reference (oid.in ());
      CORBA::String_var ior = orb->object_to_string (fcm_obj.in ());

      // Binding ior to IOR Table
      adapter->bind (FCM_OID, ior.in ());

      // Binding node menager to DomainNC
      if (!CORBA::is_nil (this->domain_nc_.in ()))
        {
          DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - ")
                       ACE_TEXT("Registering FCM in NC as \"%C\".\n"), FCM_OID));
          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup (FCM_OID);
          this->domain_nc_->rebind (name, fcm_obj.in ());
        }

      // Writing ior to file
      DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - ")
                    ACE_TEXT("Writing node IOR %C to file %C.\n"), ior.in (), options_.ior_file_));
          if (!DAnCE::FCM::write_IOR (options_.ior_file_, ior.in ()))
            DANCE_ERROR (DANCE_LOG_NONFATAL_ERROR, (LM_ERROR, DLINFO "FaultCorrelationManager_Module::create_object - "
                          "Error: Unable to write IOR to file %C\n",
                          options_.ior_file_));
      
      // Activate POA manager
      PortableServer::POAManager_var mgr = this->root_poa_->the_POAManager ();
      mgr->activate ();

      FLARE::FaultNotification_var fn = 
        FLARE::FaultNotification::_narrow (fcm_obj.in ());

      rep_mgr->register_fault_notification (fn.in ());

      // Finishing Deployment part
      DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - ")
                    ACE_TEXT("FaultCorrelationManager is running...\n")));
      
      DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - ")
                    ACE_TEXT("FaultCorrelationManager IOR: %s\n"), ior.in ()));

      // make object active
      fcm->activate ();

      DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - ")
                    ACE_TEXT("activated thread for processing failure notifications.\n")));

      return fcm_obj._retn ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("FaultCorrelationManager::main\t\n");
      return CORBA::Object::_nil ();
    }
}

void
FaultCorrelationManager_Module::register_value_factories (void)
{
  DANCE_TRACE("DAnCE_FaultCorrelationManager_Module::register_value_factories (void)");
  /*
  DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_object - ")
                ACE_TEXT("Registering Cookie value factory.\n")));
  CORBA::ValueFactory_var vf = new Components::Cookie_init();
  vf = this->orb_->register_value_factory ("IDL:omg.org/Components/Cookie:1.0", vf.in());
  */
}

void
FaultCorrelationManager_Module::create_poas (void)
{
  DANCE_TRACE("DAnCE_FaultCorrelationManager_Module::create_poas");
  // Get reference to Root POA.
  DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_poas - ")
                ACE_TEXT("Resolving root POA\n")));
  CORBA::Object_var obj = this->orb_->resolve_initial_references ("RootPOA");

  this->root_poa_ = PortableServer::POA::_narrow (obj.in ());

  DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_poas - ")
                ACE_TEXT("Obtaining the POAManager\n")));
  PortableServer::POAManager_var mgr = this->root_poa_->the_POAManager ();

  TAO::Utils::PolicyList_Destroyer policies (2);
  policies.length (2);

  try
    {
      DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_poas - ")
                    ACE_TEXT("FaultCorrelationManager_Module::create_poas - ")
                    ACE_TEXT("Creating the \"Managers\" POA.\n")));

      policies[0] = this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);
      policies[1] = this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);
      this->fcm_poa_ = this->root_poa_->create_POA ("Managers",
                                                    mgr.in(),
                                                    policies);
    }
  catch (const PortableServer::POA::AdapterAlreadyExists &)
    {
      DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_TRACE, DLINFO
                          ACE_TEXT("FaultCorrelationManager_Module::create_poas - ")
                    ACE_TEXT("Using existing \"Managers\" POA\n")));
      this->fcm_poa_ = this->root_poa_->find_POA ("Managers", 0);
    }
}

void
FaultCorrelationManager_Module::create_fcm_properties (DAnCE::PROPERTY_MAP &props)
{
  CORBA::Any value;
  value <<= CORBA::Any::from_string (CORBA::string_dup (this->options_.exec_mgr_ior_), 0);
  props.bind ("exec-mgr", value);
  if (this->options_.instance_nc_)
    {
      CORBA::Any val;
      val <<= CORBA::Any::from_string (CORBA::string_dup (this->options_.instance_nc_), 0);
      props.bind (DAnCE::INSTANCE_NC, val);
    }
  if (this->options_.domain_nc_)
    {
      CORBA::Any val;
      val <<= CORBA::Any::from_string (CORBA::string_dup (this->options_.domain_nc_), 0);
      props.bind (DAnCE::DOMAIN_NC, val);
    }
}

#ifndef BUILD_FAULTCORRELATIONMANAGER_EXE
ACE_FACTORY_DEFINE (FaultCorrelationManager_Module, FaultCorrelationManager_Module)
#endif /* BUILD_FAULTCORRELATIONMANAGER_EXE */
