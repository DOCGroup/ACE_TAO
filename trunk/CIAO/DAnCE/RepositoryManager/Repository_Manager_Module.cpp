// -*- C++ -*-
// $Id$

#include "Repository_Manager_Module.h"
#include "ace/Get_Opt.h"
#include "ace/OS.h"
#include "ace/Task.h"
#include "ace/Atomic_Op.h"
#include "tao/IORTable/IORTable.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/orbsvcs/Naming/Naming_Loader.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/Deployment/DAnCE_PropertiesC.h"

#ifdef DANCE_RM_USES_JAWS
#include "JAWS/server/HTTP_Server.h"
#endif

#include "RepositoryManager_Impl.h"

ACE_RCSID (DAnCE,
           Repository_Manager_Module,
           "$Id$")

namespace DAnCE
{
  namespace Repository_Manager
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
    
    class Worker : public virtual ACE_Task_Base
    {
    public:
      Worker (CORBA::ORB_ptr orb) : 
        orb_(CORBA::ORB::_duplicate (orb))
      {
      }
      
      virtual int svc (void)
      {
        DANCE_TRACE ("DAnCE::Repository_Manager::Worker::svc");
        
        size_t thread_id = ++this->thread_counter_;
        
        try
          {
            DANCE_DEBUG ((LM_INFO, DLINFO "DAnCE::Repository_Manager::Worker::svc - "
                          "Spawning thread %u for the ORB.\n", thread_id));
            this->orb_->run ();
            DANCE_DEBUG ((LM_INFO, DLINFO "DAnCE::Repository_Manager::Worker::svc - "
                          "ORB event loop for thread %u completed successfully.\n", thread_id));
          }
        catch (CORBA::Exception &ex)
          {
            DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE::Repository_Manager::Worker::svc - "
                          "Caught CORBA exception from ORB event loop for thread %u: %s\n",
                          thread_id, ex._info ().c_str ()));
            return -1;
          }
        catch (...)
          {
            DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE::Repository_Manager::Worker::svc - "
                          "Caught unknown C++ exception from ORB event loop for thread %u\n",
                          thread_id));
            return -1;
          }
        
        return 0;
      }
      
    private:
      ACE_Atomic_Op<TAO_SYNCH_MUTEX, unsigned long> thread_counter_;
      CORBA::ORB_var orb_;
    };
  }
}

DAnCE_RepositoryManager_Module::DAnCE_RepositoryManager_Module (void)
{
  DANCE_TRACE("DAnCE_RepositoryManager_Module::DAnCE_RepositoryManager_Module");
}

DAnCE_RepositoryManager_Module::~DAnCE_RepositoryManager_Module (void)
{
  DANCE_TRACE ("DAnCE_RepositoryManager_Module::~DAnCE_RepositoryManager_Module");

  for (Servant_Map::iterator it = this->rm_map_.begin ();
       it != this->rm_map_.end ();
       ++it)
    {
      delete (*it).int_id_;
    }
}

const char *
DAnCE_RepositoryManager_Module::usage (void)
{
  DANCE_TRACE ("DAnCE_RepositoryManager_Module::usage");
  return "Node Manager Options:\n"
    "\t-n,--name [name]\t Name to register in the naming service\n"
    "\t-t,--threads [num]\t Number of threads to run.\n"
    "\t-f,--file [name]\t Filename to output IOR.\n"
    "\t-p,--package-dir [directory]\t Directory to store packages.\n"
    "\t-s,--server-address [address]\t Address of the HTTP server associated with this RM.\n"
    "\t-d,--domain-nc [NC]\t Default naming context for domain objects.\n"
#ifdef DANCE_RM_USES_JAWS
    "\t--spawn-http\t\t Spawn a JAWS http server, controlled by following options:\n"
    "\t--http-port [port]\t Spawn JAWS HTTP server with provided port, default is 5432\n"
    "\t--http-threads [number]\t Spawn provided number of threads in HTTP server, default is 1\n"
    "\t--http-threading [strategy]\t Use provided threading strategy. POOL (Default), PER_REQUEST, THROTTLE.\n"
    "\t--http-io [stratety]\t Use provided I/O strategy.  SYNCH (default), ASYNCH.\n"
    "\t--http-caching [strategy]\t Use provided caching strategy. NO_CACHE (default), CACHE\n"
#endif
    ;
  
}

bool
DAnCE_RepositoryManager_Module::parse_args (int argc, ACE_TCHAR * argv[])
{
  DANCE_TRACE ("DAnCE_RepositoryManager_Module::parse_args");
  
  ACE_Get_Opt get_opts (argc - 1,
                        argv + 1,
                        ACE_TEXT(":hd:t:f:p:n:"),
                        0,
                        0,
                        ACE_Get_Opt::RETURN_IN_ORDER,
                        1);

  get_opts.long_option (ACE_TEXT("help"), 'h', ACE_Get_Opt::NO_ARG);
  get_opts.long_option (ACE_TEXT("domain-nc"), 'd', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("file"), 'f', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("threads"), 't', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("package-dir"), 'p', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("name"), 'n', ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("server-address"), 's', ACE_Get_Opt::ARG_REQUIRED);
#ifdef DANCE_RM_USES_JAWS
  get_opts.long_option (ACE_TEXT("spawn-http"), ACE_Get_Opt::NO_ARG);
  get_opts.long_option (ACE_TEXT("http-port"), ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("http-threads"), ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("http-threading"), ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("http-io"), ACE_Get_Opt::ARG_REQUIRED);
  get_opts.long_option (ACE_TEXT("http-caching"), ACE_Get_Opt::ARG_REQUIRED);
#endif DANCE_RM_USES_JAWS
  
  //get_opts.long_option ("help", '?');

  char c;
  while ( (c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'd':
          DANCE_DEBUG ((LM_DEBUG, DLINFO "Repository_Manager_Module::parse_args - "
                        "Binding to provided Domain Naming Context: '%s'\n",
                        get_opts.opt_arg ()));
          this->options_.domain_nc_ = get_opts.opt_arg ();
          break;
          
        case 'f':
          DANCE_DEBUG ((LM_DEBUG, DLINFO "Repository_Manager_Module::parse_args - "
                        "Output filename for IOR is %C\n",
                        get_opts.opt_arg ()));
          this->options_.ior_file_ = get_opts.opt_arg ();
          break;
          
        case 't':
          DANCE_DEBUG ((LM_DEBUG, DLINFO "Repository_Manager_Module::parse_args - "
                        "Number of threads is %C\n", get_opts.opt_arg ()));
          this->options_.threads_ = (ACE_OS::atoi (get_opts.opt_arg ()));
          break;
          
        case 'p':
          DANCE_DEBUG ((LM_DEBUG, DLINFO "Repository_Manager_Module::parse_args - "
                        "Package directory is %C\n",
                        get_opts.opt_arg ()));
          this->options_.package_dir_ = get_opts.opt_arg ();
          break;
          
        case 'n':
          DANCE_DEBUG ((LM_DEBUG, DLINFO "Repository_Manager_Module::parse_args - "
                        "Name is %C\n",
                        get_opts.opt_arg ()));
          this->options_.name_ = get_opts.opt_arg ();
          break;

        case 's':
          DANCE_DEBUG ((LM_DEBUG, DLINFO "Repository_Manager_Module::parse_args - "
                        "Server address is %C\n",
                        get_opts.opt_arg ()));
          this->options_.server_address_ = get_opts.opt_arg ();
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
          if (ACE_OS::strcmp (get_opts.long_option (), ACE_TEXT("spawn-http")) == 0)
            {
              DANCE_DEBUG ((LM_DEBUG, DLINFO "Repository_Manager_Module::parse_args - "
                            "Found option to spawn in-process HTTP server\n"));
              this->options_.spawn_http_ = true;
            }
          else if (ACE_OS::strcmp (get_opts.long_option (), ACE_TEXT("http-port")) == 0)
            {
              DANCE_DEBUG ((LM_DEBUG, DLINFO "Repository_Manager_Module::parse_args - "
                            "Using %C as the spawned HTTP server port\n",
                            get_opts.opt_arg ()));
              this->options_.spawn_http_ = true;
              this->options_.http_port_ = get_opts.opt_arg ();
            }
          else if (ACE_OS::strcmp (get_opts.long_option (), ACE_TEXT("http-")) == 0)
            {
              DANCE_DEBUG ((LM_DEBUG, DLINFO "Repository_Manager_Module::parse_args - "
                            "Using %C as the spawned HTTP number of threads\n",
                            get_opts.opt_arg ()));
              this->options_.spawn_http_ = true;
              this->options_.http_threads_ = get_opts.opt_arg ();
            }
          else if (ACE_OS::strcmp (get_opts.long_option (), ACE_TEXT("http-")) == 0)
            {
              DANCE_DEBUG ((LM_DEBUG, DLINFO "Repository_Manager_Module::parse_args - "
                            "Using %C as the spawned HTTP server threading model\n",
                            get_opts.opt_arg ()));
              this->options_.spawn_http_ = true;
              this->options_.http_threading_ = get_opts.opt_arg ();
            }
          else if (ACE_OS::strcmp (get_opts.long_option (), ACE_TEXT("http-")) == 0)
            {
              DANCE_DEBUG ((LM_DEBUG, DLINFO "Repository_Manager_Module::parse_args - "
                            "Using %C as the spawned HTTP server IO strategy\n",
                            get_opts.opt_arg ()));
              this->options_.spawn_http_ = true;
              this->options_.http_io_ = get_opts.opt_arg ();
            }
          else if (ACE_OS::strcmp (get_opts.long_option (), ACE_TEXT("http-")) == 0)
            {
              DANCE_DEBUG ((LM_DEBUG, DLINFO "Repository_Manager_Module::parse_args - "
                            "Using %C as the spawned HTTP server caching strategy\n",
                            get_opts.opt_arg ()));
              this->options_.spawn_http_ = true;
              this->options_.http_caching_ = get_opts.opt_arg ();
            }
          else
            {
              DANCE_ERROR ((LM_ERROR, DLINFO "Repository_Manager_Module::parse_args - "
                            "ERROR: unknown long option %C\n",
                            get_opts.long_option ()));
            }

          break;
          
        case '\?':
        case ':':
          DANCE_ERROR ((LM_ERROR, DLINFO  "Repository_Manager_Module::parse_args - "
                        "Options %c:%C requires an argument\n", c, get_opts.opt_opt ()));
          break;
          
        default:
          DANCE_DEBUG ((LM_TRACE, DLINFO "Repository_Manager_Module::parse_args - ignoring unknown option %c:%C\n",
                        c, get_opts.opt_arg ()));
        }
      
    }

  if (this->options_.server_address_ == 0)
    {
      DANCE_ERROR ((LM_ERROR, DLINFO "Repository_Manager_Module::parse_args - "
                    "Must provider server address using --server-address option.\n"));
      return false;
    }

   return true;
}

CORBA::Object_ptr
DAnCE_RepositoryManager_Module::create_object (CORBA::ORB_ptr orb,
                                               int argc,
                                               ACE_TCHAR *argv[])
{
  DANCE_TRACE ("DAnCE_RepositoryManager_Module::create_object");
  
  try
    {
      if (CORBA::is_nil(orb))
        {
          DANCE_ERROR((LM_ERROR, DLINFO "DAnCE_RepositoryManager_Module::create_object - "
                       "Attempted to create Repository Manager with a nil orb.\n"));
          return CORBA::Object::_nil();
        }
      else
        {
          this->orb_ = CORBA::ORB::_duplicate (orb);
        }
      
      if (ACE_OS::strcmp(orb->id(), this->orb_->id()) != 0)
        {
          DANCE_DEBUG((LM_TRACE, DLINFO "DAnCE_RepositoryManager_Module::create_object - "
                       "Resetting NM's orb.\n"));
          this->orb_ = CORBA::ORB::_duplicate (orb);
          this->domain_nc_ = CosNaming::NamingContext::_nil();
        }

      if (!this->parse_args (argc, argv))
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE_RepositoryManager_Module::create_object - "
                        "Failed to parse command line arguments, exiting\n"));
          return CORBA::Object::_nil ();
        }
      
      if (this->options_.spawn_http_)
        this->spawn_http ();
      
      this->create_poas ();

      if (this->options_.domain_nc_)
        {
          try
            {
              DANCE_DEBUG((LM_TRACE, DLINFO "DAnCE_RepositoryManager_Module::create_object - "
                           "Resolving DomainNC.\n"));
              CORBA::Object_var domain_obj = this->orb_->string_to_object (this->options_.domain_nc_);
              if (!CORBA::is_nil (domain_obj.in ()))
                {
                  this->domain_nc_ = CosNaming::NamingContext::_narrow (domain_obj.in());
                  if (CORBA::is_nil (this->domain_nc_.in ()))
                    {
                      DANCE_ERROR ((LM_ERROR,DLINFO "DAnCE_RepositoryManager_Module::create_object - "
                                    "Narrow to NamingContext return nil for DomainNC.\n"));
                      return CORBA::Object::_nil ();
                    }
                }
            }
          catch (CORBA::Exception&)
            {
              DANCE_DEBUG  ((LM_WARNING, DLINFO "DAnCE_RepositoryManager_Module::create_object - "
                             "DomainNC context not found!\n"));
            }
        }

      
      DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_RepositoryManager_Module::create_object - "
                    "Initializing the IOR Table\n"));
      // Initialize IOR table
      CORBA::Object_var table_object = orb->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter = IORTable::Table::_narrow (table_object.in ());

      if (CORBA::is_nil (adapter.in ()))
        {
          DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE_RepositoryManager_Module::create_object - "
                         "Unable to RIR the IORTable.\n"));
          return CORBA::Object::_nil ();
        }


      //Creating repository manager servant
      DAnCE::RepositoryManagerDaemon_i * rm = new DAnCE::RepositoryManagerDaemon_i (orb,
                                                                                    this->options_.server_address_,
                                                                                    this->options_.package_dir_);
      PortableServer::ServantBase_var safe_svt (rm);
      
      ACE_CString repository_manager_oid;
      
      if (this->options_.name_ == 0)
        repository_manager_oid = "RepositoryManager";
      else
        {
          repository_manager_oid = this->options_.name_;
          repository_manager_oid += ".RepositoryManager";
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

      // Binding repository menager to DomainNC
      if (!CORBA::is_nil (this->domain_nc_.in ()))
        {
          ACE_CString ns_name;
          if (this->options_.name_ == 0)
            ns_name = "RepositoryManager";
          else ns_name = this->options_.name_;
          
          DANCE_DEBUG((LM_TRACE, DLINFO "DAnCE_RepositoryManager_Module::create_object - "
                       "Registering NM in NC as \"%C\".\n", ns_name.c_str ()));
          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup (ns_name.c_str ());
          name[0].kind = CORBA::string_dup ("RepositoryManager");
          this->domain_nc_->rebind (name, nm_obj.in ());
        }

      // Writing ior to file
      if (0 != this->options_.ior_file_)
        {
          DANCE_DEBUG ((LM_TRACE,  DLINFO "DAnCE_RepositoryManager_Module::create_object - "
                        "Writing RM IOR %C to file %C.\n", this->options_.ior_file_, ior.in ()));
          if (!DAnCE::Repository_Manager::write_IOR (this->options_.ior_file_, ior.in ()))
            DANCE_ERROR ((LM_ERROR, DLINFO "DAnCE_RepositoryManager_Module::create_object - "
                          "Error: Unable to write IOR to file %C\n",
                          this->options_.ior_file_));
        }
      
      // Activate POA manager
      PortableServer::POAManager_var mgr = this->root_poa_->the_POAManager ();
      mgr->activate ();

      // Finishing Deployment part
      DANCE_DEBUG ((LM_NOTICE, DLINFO "DAnCE_RepositoryManager_Module::create_object - "
                    "DAnCE_RepositoryManager is running...\n"));
      
      DANCE_DEBUG ((LM_DEBUG, DLINFO "DAnCE_RepositoryManager_Module::create_object - "
                    "RepositoryManager IOR: %s\n", ior.in ()));

      return nm_obj._retn ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("DAnCE_RepositoryManager::main\t\n");
      return CORBA::Object::_nil ();
    }
}

void
DAnCE_RepositoryManager_Module::create_poas (void)
{
  DANCE_TRACE("DAnCE_Repository_Manager_Module::create_poas");
  // Get reference to Root POA.
  DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_RepositoryManager_Module::create_poas - "
                "Resolving root POA\n"));
  CORBA::Object_var obj = this->orb_->resolve_initial_references ("RootPOA");

  this->root_poa_ = PortableServer::POA::_narrow (obj.in ());

  DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_RepositoryManager_Module::create_poas - "
                "Obtaining the POAManager\n"));
  PortableServer::POAManager_var mgr = this->root_poa_->the_POAManager ();

  TAO::Utils::PolicyList_Destroyer policies (2);
  policies.length (2);

  try
    {
      DANCE_DEBUG ((LM_TRACE, DLINFO "DAnCE_RepositoryManager_Module::create_poas - "
                    "DAnCE_RepositoryManager_Module::create_poas - "
                    "Creating the \"Repository\" POA.\n"));

      policies[0] = this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);
      policies[1] = this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);
      this->rm_poa_ = this->root_poa_->create_POA ("Repository",
                                       mgr.in(),
                                       policies);
    }
  catch (const PortableServer::POA::AdapterAlreadyExists &)
    {
      DANCE_DEBUG ((LM_INFO, DLINFO "DAnCE_RepositoryManager_Module::create_poas - "
                    "Using exiswting \"Repository\" POA\n"));
      this->rm_poa_ = this->root_poa_->find_POA ("Repository", 0);
    }
}

void 
DAnCE_RepositoryManager_Module::spawn_http (void)
{
  DANCE_TRACE ("DAnCE_RepositoryManager_Module::spawn_http");
  
  const ACE_TCHAR *name = "HTTP_Server";
  
  ACE_CString args;
  args += "-p ";
  args += this->options_.http_port_;
  args += " -n ";
  args += this->options_.http_threads_;
  args += " -i ";
  args += this->options_.http_io_;
  args += " -t ";
  args += this->options_.http_threading_;
  args += " -c ";
  args += this->options_.http_caching_;
  args += " -b 50 -f THR_NEW_LWP";
  
  ACE_Service_Config::current ()->initialize (name, args.c_str ());
}

#ifndef BUILD_REPOSITORY_MANAGER_EXE
ACE_FACTORY_DEFINE (DAnCE_RepositoryManager_Module, DAnCE_RepositoryManager_Module)
#endif /*BUILD_REPOSITORY_MANAGER_EXE */
