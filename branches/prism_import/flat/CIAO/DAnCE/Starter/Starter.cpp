// -*- C++ -*-

//=============================================================================
/**
 * @file Starter.cpp
 *
 * $Id$
 *
 * @Brief Dynamically runs libraries
 *
 * @author Vinzenz Tornow <vt@prismtech.com>
 */
//=============================================================================

#include "Starter.h"
#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"
#include "ace/DLL.h"
#include "ace/Get_Opt.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/PortableServer/PortableServer.h"
#include "DAnCELoggerFactory.h"
#include "tao/Object_Loader.h"
#include "tao/IORTable/IORTable.h"
#include "DAnCE/Logger/Log_Macros.h"

using namespace DAnCE;

int main(int argc, ACE_TCHAR * argv[])
{
  try
    {
      Starter starter (argc, argv);
      starter.execute();
      return 0;
    }
  catch (ACE_CString & e)
    {
      DANCE_ERROR ( (LM_ERROR, "[%M] dance starter failed with an exception : \"%s\"\n", e.c_str()));
    }
  catch (CORBA::Exception & e)
    {
      DANCE_ERROR ( (LM_ERROR, "[%M] dance starter failed with an CORBA exception : \"%s\"\n", e._info().c_str()));
    }
  catch (...)
    {
      DANCE_ERROR ( (LM_ERROR, "[%M] dance starter failed with an unknown exception.\n"));
    }
  return -1;
}

namespace DAnCE
{

Starter::Starter(int argc, ACE_TCHAR * argv[]) :
      orb_(CORBA::ORB_init (argc, argv, ""))
      , optLogLevel_(5) //default
      , argc_(argc), argv_(argv), optNS_(false), optEM_(false), optPLB_(false)
      , optPL_(false)
{
  this->getLogLevel(argc, argv);
  this->setLogLevel();
  DANCE_DEBUG ( (LM_TRACE, "[%M] Creating starter...\n"));
  this->parseArgs(argc, argv);

  DAnCELoggerFactory
      * dlf = ACE_Dynamic_Service<DAnCELoggerFactory>::instance ("DAnCE_Logger_Backend_Factory");
  if (dlf)
    {
      DANCE_DEBUG ( (LM_TRACE, "[%M] Replacing logger backend.\n"));
      ACE_Log_Msg_Backend * backend = dlf->get_logger_backend(this->orb_);
      backend->open(0);
      ACE_Log_Msg::msg_backend (backend);
      ACE_Log_Msg * ace = ACE_Log_Msg::instance();
      ace->clr_flags(ace->flags());
      ace->set_flags(ACE_Log_Msg::CUSTOM);
    }
  DANCE_DEBUG ( (LM_TRACE, "[%M] Starter was created successfully.\n"));
}

Starter::~Starter()
{
  DANCE_DEBUG ( (LM_TRACE, "[%M] Starter::~Starter - starting ...\n"));
/*  TAO_Object_Loader
      * loader = ACE_Dynamic_Service<TAO_Object_Loader>::instance ("ExecutionManager_Loader");
  if (0 != loader)
    {
      DANCE_DEBUG ( (LM_TRACE, "[%M] Starter::~Starter - removing EM ...\n"));
      loader = 0;
      ACE_Service_Config::remove ("ExecutionManager_Loader");
    }
  loader
      = ACE_Dynamic_Service<TAO_Object_Loader>::instance ("NodeManager_Loader");
  if (0 != loader)
    {
      DANCE_DEBUG ( (LM_TRACE, "[%M] Starter::~Starter - removing NM ...\n"));
      loader = 0;
      ACE_Service_Config::remove ("NodeManager_Loader");
    }

  this->orb_._retn()->destroy(); */
  DANCE_DEBUG ( (LM_TRACE, "[%M] Starter::~Starter - completed.\n"));
}

void Starter::getLogLevel(int argc, ACE_TCHAR* argv[])
  {
    ACE_Get_Opt opts(argc, argv, "l:", 1, 0, ACE_Get_Opt::RETURN_IN_ORDER);
    opts.long_option("log-level", 'l', ACE_Get_Opt::ARG_REQUIRED);
    int c = -1;
    while ( (c = opts ()) != -1)
      {
        if ( c  == 'l')
          {
            int j = ACE_OS::atoi (opts.opt_arg());
            if (j != 0)
              {
                this->optLogLevel_ = j;
              }
            else
              {
                DANCE_ERROR ( (LM_WARNING, "--log-level without argument. Using default.\n"));
              }
          }
      }
  }

void Starter::parseArgs(int argc, ACE_TCHAR * argv[])
{
  DANCE_DEBUG ( (LM_TRACE, "[%M] Parsing starter's arguments...\n"));

  ACE_Get_Opt opts(argc, argv, "p::n:e::c::r::il:hg:x:d:qk:w:t:a:", 1, 0,
      ACE_Get_Opt::RETURN_IN_ORDER);
  opts.long_option("process-ns", 'p', ACE_Get_Opt::ARG_OPTIONAL);
  opts.long_option("process-ns-options", ACE_Get_Opt::ARG_REQUIRED);
  opts.long_option("node-mgr", 'n', ACE_Get_Opt::ARG_REQUIRED);
  opts.long_option("exec-mgr", 'e', ACE_Get_Opt::ARG_OPTIONAL);
  opts.long_option("create-plan-ns", 'c', ACE_Get_Opt::ARG_OPTIONAL);
  opts.long_option("rebind-plan-ns", 'r', ACE_Get_Opt::ARG_OPTIONAL);
  opts.long_option("port-indirection", 'i', ACE_Get_Opt::NO_ARG);
  opts.long_option("log-level", 'l', ACE_Get_Opt::ARG_REQUIRED);
  opts.long_option("help", 'h', ACE_Get_Opt::NO_ARG);
  opts.long_option("gen-object-key", 'g', ACE_Get_Opt::ARG_REQUIRED);
  opts.long_option("read-plan", 'x', ACE_Get_Opt::ARG_REQUIRED);
  opts.long_option("read-cdr-plan", 'd', ACE_Get_Opt::ARG_REQUIRED);
  opts.long_option("stop-plan", 'q', ACE_Get_Opt::NO_ARG);
  opts.long_option("em-ior", 'k', ACE_Get_Opt::ARG_REQUIRED);
  opts.long_option("write-cdr-plan", 'w', ACE_Get_Opt::ARG_REQUIRED);
  opts.long_option("plan-uuid", 't', ACE_Get_Opt::ARG_REQUIRED);
  opts.long_option("dam-ior", 'a', ACE_Get_Opt::ARG_REQUIRED);

  int j;
  char c;
  ACE_CString s;
  while ( (c = opts ()) != -1)
    {
      DANCE_DEBUG((LM_TRACE, "[%M] Option : \"%s\" with argument \"%s\"\n", opts.last_option(), opts.opt_arg()));
      switch (c)
        {
          case '?':
            DANCE_ERROR ( (LM_ERROR, "[%M] Wrong option \"%s\" or this option is requred attribute!\n", opts.last_option()));
            this->usage();
            throw ACE_CString("Error parsing starter arguments");
            break;
          case 'p':
            this->optNS_ = true;
            DANCE_DEBUG ( (LM_TRACE, "[%M] Naming will be started.\n"));
            this->optNSFile_ = opts.opt_arg();
            break;
          case 'n':
            s = opts.opt_arg();
            if (0 < s.length())
              {
                ACE_CString nodename;
                Node node;
                size_t pos = s.find("=");
                if (ACE_CString::npos != pos)
                  {
                    nodename = s.substring(0, pos);
                    node.ior_ = s.substring(pos + 1);
                    /*
                    node.obj = this->orb_->string_to_object(objstr.c_str());
                    if (CORBA::is_nil (node.obj))
                      {
                        DANCE_ERROR ( (LM_ERROR, "[%M] Failed create object for node \"%s\"\n", nodename.c_str()));
                        throw ACE_CString ("Invalid IOR in --node-mgr option");
                      }
                      */
                  }
                else
                  {
                    nodename = s;
                    DANCE_DEBUG ( (LM_TRACE, "[%M] Node \"%s\" will be started.\n", nodename.c_str()));
                    if (opts.optind < opts.argc_&& '-' != (s = opts.argv_[opts.optind])[0])
                      {
                        ++opts.optind;
                        node.iorfile_ = s;
                        DANCE_DEBUG ( (LM_TRACE, "[%M] and its IOR will be written to file \"%s\".\n", node.iorfile_.c_str()));
                      }
                    
                  }
                if (0 == this->nodes_.find(nodename))
                  {
                    DANCE_ERROR((LM_ERROR, "[%M] Duplication of NM name \"%s\"\n", nodename.c_str()));
                    this->usage();
                    throw ACE_CString("Duplication of NM name");
                  }
                this->nodes_.bind(nodename, node);
              }
            else
              {
                DANCE_ERROR ( (LM_ERROR, "[%M] --node-mgr option without arguments.\n"));
                this->usage();
                throw ACE_CString ("--node-mgr option without arguments.");
              }
            break;
          case 'e':
            if (this->optEM_)
              {
                DANCE_DEBUG((LM_WARNING, "[%M] ExecutionManager option is encountered more than once. Second and following ignored.\n"));
                break;
              }
            this->optEM_ = true;
            DANCE_DEBUG ( (LM_TRACE, "[%M] ExecutionManager will be started.\n"));
            this->optEMFile_ = opts.opt_arg();
            break;
          case 'l':
            j = ACE_OS::atoi (opts.opt_arg());
            if (j != 0)
              {
                this->optLogLevel_ = j;
              }
            else
              {
                DANCE_ERROR ( (LM_WARNING, "--log-level without argument. Using default.\n"));
              }
            break;
          case 'h':
            this->usage();
            break;
          case 'x':
            this->optPL_ = true;
            DANCE_DEBUG ( (LM_TRACE, "[%M] PlanLauncher will be started.\n"));
            break;
          case 'd':
          case 'q':
            this->optPLB_ = true;
            DANCE_DEBUG ( (LM_TRACE, "[%M] PlanLauncherBase will be started.\n"));
            break;
          case 'g':
            DANCE_DEBUG ( (LM_TRACE, "[%M] Object key will be generated.\n"));
            this->optGenObjKey_ = opts.opt_arg();
            if (0 == this->optGenObjKey_.length())
              {
                DANCE_ERROR ( (LM_ERROR, "[%M] --gen-object-key without argument. Doing nothing.\n"));
              }
            break;
          case 0: // long options that do not have short
            s = opts.last_option();
            if (s == "process-ns-options")
              {
                this->optNSOptions_ = opts.opt_arg();
                if (0 == this->optNSOptions_.length())
                  {
                    DANCE_ERROR ( (LM_ERROR, "[%M] --process-ns-options without argument\n"));
                  }
              }
            else
              {
                if (!isPossibleOption(s.c_str()))
                  {
                    DANCE_ERROR((LM_ERROR, "[%M] Invalid option : %s\n", s.c_str()));
                  }
              }
            break;
          default:
            if (!isPossibleOption(opts.last_option()))
              {
                DANCE_ERROR((LM_ERROR, "[%M] Invalid option : %s\n", opts.last_option()));
                this->usage();
              }
            break;
        }//switch
    }//while

  DANCE_DEBUG ( (LM_TRACE, "[%M] Parsing starter's arguments completed.\n"));
}

void Starter::execute()
{
  DANCE_DEBUG ( (LM_TRACE, "[%M] Executing starter...\n"));
  bool orb_run = false;

  // Generate object key
  if (0 < this->optGenObjKey_.length())
    {
      this->generateObjectKey(this->optGenObjKey_.c_str());
    }

  // Naming
  if (this->optNS_)
    {
      this->initNaming();
      orb_run = true;
    }

  // NodeManagers
  for (ACE_Map_Manager<ACE_CString, Node, ACE_Null_Mutex>::iterator
      it = this->nodes_.begin(); it != this->nodes_.end(); ++it)
    {
      if (!CORBA::is_nil ((*it).int_id_.obj) || 0 < (*it).int_id_.ior_.length())
        continue;
      orb_run = true;
      (*it).int_id_.obj = this->initNodeManager((*it).ext_id_.c_str());
      if (0 != (*it).int_id_.iorfile_.length())
        {
          this->write_IOR((*it).int_id_.iorfile_.c_str(), this->orb_->object_to_string((*it).int_id_.obj));
        }
    }

  // ExecutionManager
  CORBA::Object_var em;
  if (this->optEM_)
    {
      em = this->initExecutionManager();
      if ( !CORBA::is_nil(em) && 0 < this->optEMFile_.length())
        {
          this->write_IOR(this->optEMFile_.c_str(),
              this->orb_->object_to_string(em.in()));
        }
      orb_run = true;
    }

  if (this->optPLB_ || this->optPL_)
    {
      this->runPlanLauncher();
    }

  if (orb_run)
    {
      DANCE_DEBUG ( (LM_TRACE, "[%M] Running starter's ORB...\n"));
      this->orb_->run();
    }
  else
    {
      DANCE_DEBUG ( (LM_TRACE, "[%M] Skipping starter's ORB->run.\n"));
    }
  DANCE_DEBUG ( (LM_TRACE, "[%M] Executing starter has completed.\n"));
}

void Starter::usage()
{
  DANCE_ERROR ( (LM_EMERGENCY, "Usage : dance <options>\n"
          "Options :\n"
          "\t-l|--log-level <log level> - sets log level (default 5). 1 - most detailed.\n"
          "\t-g|--gen-object-key \"<NODE_NAME> <PLAN_ID> <COMPONENT_ID> [<PORT_NAME>]\" - generates a corbaloc URL\n"
          "\t-h|--help - shows this help\n"
          "\t-p|--process-ns [IOR_FILE_NAME] - Instantiate a local name service within the DAnCE process. Export the root context to IOR_FILE if any.\n"
          "\t--process-ns-options <options> - Specifies quoted string of options to be passed to name service. Depends on --process-ns option.\n"
          "\t-e|--exec-mgr [IOR_FILE_NAME] - This process will have an execution manager and it's IOR will optionally be exported to IOR_FILE_NAME.\n"
          "\t-n|--node-mgr name [IOR_FILE_NAME] - Create a named NodeManager in process and optionally export its IOR to IOR_FILE_NAME\n"
          "\t-n|--node-mgr name=<IOR> - Place the NodeManager instance whose reference is IOR under the control of the ExecutionManager in this process (requires --exec-mgr).\n"
          "In addition to the options above, the Service Configurator options are processed too.\n"));
}

void Starter::setLogLevel()
{
  u_long new_mask = 0;
  if (this->optLogLevel_ <= 1)
    {
      new_mask |= LM_TRACE;
    }
  if (this->optLogLevel_ <= 2)
    {
      new_mask |= LM_DEBUG;
    }
  if (this->optLogLevel_ <= 3)
    {
      new_mask |= LM_INFO;
    }
  if (this->optLogLevel_ <= 4)
    {
      new_mask |= LM_NOTICE;
    }
  if (this->optLogLevel_ <= 5)
    {
      new_mask |= LM_WARNING;
    }
  if (this->optLogLevel_ <= 6)
    {
      new_mask |= LM_ERROR;
    }
  if (this->optLogLevel_ <= 7)
    {
      new_mask |= LM_CRITICAL;
    }
  if (this->optLogLevel_ <= 8)
    {
      new_mask |= LM_ALERT;
    }
  if (this->optLogLevel_ <= 9)
    {
      new_mask |= LM_EMERGENCY;
    }
  ACE_Log_Msg::instance()->priority_mask(new_mask, ACE_Log_Msg::PROCESS);
  DANCE_DEBUG ( (LM_TRACE, "[%M] Logging level is set to %i\n", this->optLogLevel_));
}

void Starter::generateObjectKey(const char * keyargs)
{
  ACE_CString args = keyargs;
  ssize_t pos_start = 0;
  ACE_CString node;
  ACE_CString plan;
  ACE_CString component;
  ACE_CString port;

  // node
  if (pos_start < (ssize_t) args.length())
    {
      size_t pos_end = args.find(' ', pos_start);
      if (ACE_CString::npos == pos_end)
        {
          node = args.substring(pos_start);
          pos_start = args.length();
        }
      else
        {
          node = args.substring(pos_start, pos_end - pos_start);
          pos_start = pos_end + 1;
        }
    }

  // plan
  if (pos_start < (ssize_t) args.length())
    {
      size_t pos_end = args.find(' ', pos_start);
      if (ACE_CString::npos == pos_end)
        {
          plan = args.substring(pos_start);
          pos_start = args.length();
        }
      else
        {
          plan = args.substring(pos_start, pos_end - pos_start);
          pos_start = pos_end + 1;
        }
    }

  // component
  if (pos_start < (ssize_t) args.length())
    {
      size_t pos_end = args.find(' ', pos_start);
      if (ACE_CString::npos == pos_end)
        {
          component = args.substring(pos_start);
          pos_start = args.length();
        }
      else
        {
          component = args.substring(pos_start, pos_end - pos_start);
          pos_start = pos_end + 1;
        }
    }

  // port
  if (pos_start < (ssize_t) args.length())
    {
      size_t pos_end = args.find(' ', pos_start);
      if (ACE_CString::npos == pos_end)
        {
          port = args.substring(pos_start);
          pos_start = args.length();
        }
      else
        {
          port = args.substring(pos_start, pos_end - pos_start);
          pos_start = pos_end + 1;
        }
    }

  // check
  if (0 == node.length() || 0 == plan.length() || 0 == component.length())
    {
      DANCE_ERROR ( (LM_ERROR, "[%M] Invalid object attributes received : \"s\"\n", args.c_str()));
      this->usage();
      return;
    }

  this->generateObjectKey(node.c_str(), plan.c_str(), component.c_str(), 0
      == port.length() ? 0 : port.c_str());
}

void Starter::generateObjectKey(const char * node, const char * plan,
    const char * component, const char * port)
{
  DANCE_DEBUG ( (LM_TRACE, "[%M] Starter::generateObjectKey starting...\n"));
  CORBA::Boolean prev_format = this->orb_->_use_omg_ior_format();
  this->orb_->_use_omg_ior_format(false);
  // warning : parent POA supposed to be RootPOA
  CORBA::Object_var obj = this->orb_->resolve_initial_references("RootPOA");
  PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in());

  CORBA::PolicyList policies(4);
  policies.length(4);
  policies[0]= root_poa->create_id_assignment_policy(PortableServer::USER_ID);
  policies[1]
      = root_poa->create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);
  policies[2]
      = root_poa->create_servant_retention_policy(PortableServer::NON_RETAIN);
  policies[3] = root_poa->create_lifespan_policy(PortableServer::PERSISTENT);
  PortableServer::POAManager_var mgr = root_poa->the_POAManager();
  PortableServer::POA_var
      myPOA = root_poa->create_POA(node, mgr.in(), policies);
  for (size_t i = 0; i < policies.length(); ++i)
    {
      policies[i]->destroy();
    }

  const char delim = '/';

  ACE_CString s = plan;
  s += delim;
  s += component;
  if (0 != port)
    {
      s += delim;
      s += port;
    }

  PortableServer::ObjectId_var
      oid = PortableServer::string_to_ObjectId (s.c_str());
  CORBA::Object_var o = myPOA->create_reference_with_id(oid.in(),
      "IDL:omg.org/CORBA/Object:1.0");
  s = this->orb_->object_to_string(o);
  size_t pos = s.find(delim);
  if (ACE_CString::npos != pos)
    {
      s = s.substr(pos + 1);
    }
  else
    {
      DANCE_ERROR ( (LM_WARNING, "Failed to cut off the host specific part of URL.\n"));
    }
  DANCE_DEBUG ( (LM_TRACE, "[%M] Starter::generateObjectKey printing result : %s\n", s.c_str()));
  ACE_OS::printf ("%s\n", s.c_str());
  
  this->orb_->_use_omg_ior_format(prev_format);
  DANCE_DEBUG ( (LM_TRACE, "[%M] Starter::generateObjectKey completed.\n"));
}

void Starter::write_IOR(const char * ior_file_name, const char* ior)
{
  FILE* ior_output_file_ = ACE_OS::fopen (ior_file_name, "w");

  if (ior_output_file_)
    {
      ACE_OS::fprintf (ior_output_file_, "%s", ior);
      ACE_OS::fclose (ior_output_file_);
      DANCE_DEBUG ( (LM_DEBUG, "[%M] ior was written into file \"%s\"\n", ior_file_name));
    }
  else
    {
      DANCE_ERROR ( (LM_ERROR, "[%M] Unable to open IOR output file %s : %m\n",
              ior_file_name));
    }
}

void Starter::initNaming()
{
  DANCE_DEBUG ( (LM_TRACE, "[%M] Starting naming...\n"));
  TAO_Object_Loader
      * loader = ACE_Dynamic_Service<TAO_Object_Loader>::instance ("Naming_Loader");
  if (0 == loader)
    {
      ACE_CString directive = 
        "dynamic Naming_Loader Service_Object * TAO_CosNaming_Serv:_make_TAO_Naming_Loader() \"";
      directive += this->optNSOptions_ + "\"";
      ACE_Service_Config::process_directive(directive.c_str());
    }
    
    DANCE_DEBUG ( (LM_TRACE, "[%M] Putting ior to file if necessary...\n"));
    if (0 < this->optNSFile_.length())
      {
        CORBA::Object_var obj = this->orb_->resolve_initial_references("NameService");
        if (CORBA::is_nil(obj))
          {
            DANCE_ERROR((LM_ERROR, "[%M] Failed to rir \"NameService\" after creation to write it to file.\n"));
          }
        else
          {
            this->write_IOR(this->optNSFile_.c_str(), this->orb_->object_to_string(obj));
          }
      }
    
/*    CORBA::Object_var table_object = this->orb_->resolve_initial_references ("IORTable");
    IORTable::Table_var table = IORTable::Table::_narrow (table_object.in ());
    if (CORBA::is_nil (table.in()))
      {
        DANCE_ERROR ( (LM_ERROR, "[%M] Failed to register Naming in IORTable.Nil IORTable\n"));
        return;
      }
    try
      {
        table->bind ("NameService", this->orb_->object_to_string (this->process_naming_));
      }
    catch (...)
      {
        DANCE_ERROR ( (LM_ERROR, "[%M] Failed to register Naming in IORTable with \"NameService\". Exception is caught.\n"));
      }*/
    DANCE_DEBUG ( (LM_TRACE, "[%M] Starting naming completed.\n"));
  }

CORBA::Object_ptr
Starter::initNodeManager (const char * node)
  {
    DANCE_DEBUG ( (LM_TRACE, "[%M] Starting NodeManager \"%s\"...\n", node));
    TAO_Object_Loader * loader = ACE_Dynamic_Service<TAO_Object_Loader>::instance ("NodeManager_Loader");
    if (0 == loader)
      {
        ACE_Service_Config::process_directive (
            ACE_DYNAMIC_SERVICE_DIRECTIVE ("NodeManager_Loader"
                , "DAnCE_NodeManager"
                , "_make_DAnCE_NodeManager_Module"
                , ""));
        loader = ACE_Dynamic_Service<TAO_Object_Loader>::instance ("NodeManager_Loader");
      }
    if (0 == loader)
      {
        DANCE_ERROR ( (LM_ERROR, "[%M] Failed to load node manager \"%s\".\n", node));
        throw ACE_CString ("Failed to load NodeManager.");
      }
    int c = 0;
    char ** v = 0;
    this->argCopyForNode (node, c, v);
    CORBA::Object_var res = loader->create_object (this->orb_, c, v);
    this->releaseArgs (c, v);
    if (CORBA::is_nil(res.in()))
      {
        DANCE_ERROR ( (LM_ERROR, "[%M] Failed to create node manager \"%s\".\n", node));
        throw ACE_CString ("Failed to create NodeManager.");
      }
    DANCE_DEBUG ( (LM_TRACE, "[%M] Starting NodeManager \"%s\" completed.\n", node));
    return res._retn();
  }

CORBA::Object_ptr
Starter::initExecutionManager()
  {
    DANCE_DEBUG ( (LM_TRACE, "[%M] Starting ExecutionManager...\n"));
    ACE_Service_Config::process_directive (
        ACE_DYNAMIC_SERVICE_DIRECTIVE ("ExecutionManager_Loader"
            , "DAnCE_ExecutionManager"
            , "_make_DAnCE_ExecutionManager_Module"
            , ""));
    TAO_Object_Loader * loader = ACE_Dynamic_Service<TAO_Object_Loader>::instance ("ExecutionManager_Loader");
    if (0 == loader)
      {
        DANCE_ERROR ( (LM_ERROR, "[%M] Failed to load execution manager .\n"));
        throw ACE_CString ("Failed to load ExecutionManager.");
      }
    int c = 0;
    char ** v = 0;
    this->argCopyForEM (c, v);
    CORBA::Object_var em = loader->create_object (this->orb_, c, v);
    this->releaseArgs (c, v);
    if (CORBA::is_nil(em.in()))
      {
        DANCE_ERROR ( (LM_ERROR, "[%M] Failed to create execution manager.\n"));
        throw ACE_CString ("Failed to create ExecutionManager.");
      }
    DANCE_DEBUG ( (LM_TRACE, "[%M] Starting ExecutionManager completed.\n"));
    return em._retn();
  }

void
Starter::runPlanLauncher()
  {
    if (this->optPL_)
      {
        DANCE_DEBUG ( (LM_TRACE, "[%M] Starting PlanLauncher...\n"));
        ACE_Service_Config::process_directive (
            ACE_DYNAMIC_SERVICE_DIRECTIVE ("PlanLauncher_Loader"
                , "DAnCE_Plan_Launcher"
                , "_make_DAnCE_Plan_Launcher_Module"
                , ""));
      }
    else
      {
        DANCE_DEBUG ( (LM_TRACE, "[%M] Starting PlanLauncherBase...\n"));
        ACE_Service_Config::process_directive (
            ACE_DYNAMIC_SERVICE_DIRECTIVE ("PlanLauncher_Loader"
                , "DAnCE_Plan_Launcher_Base"
                , "_make_DAnCE_Plan_Launcher_Base_Module"
                , ""));
      }
    TAO_Object_Loader * loader = ACE_Dynamic_Service<TAO_Object_Loader>::instance ("PlanLauncher_Loader");
    if (0 == loader)
      {
        DANCE_ERROR ( (LM_ERROR, "[%M] Failed to load plan launcher.\n"));
        throw ACE_CString ("Failed to load PlanLauncher.");
      }
    int c = 0;
    char ** v = 0;
    this->argCopyForPL (c, v);
    loader->create_object (this->orb_, c, v);
    this->releaseArgs (c, v);
    DANCE_DEBUG ( (LM_TRACE, "[%M] Starting PlanLauncher(Base) completed.\n"));
  }

void
Starter::argCopyForNaming (int & c, char **& v)
  {
    int total_sz = 2;
    for (size_t pos = this->optNSOptions_.find (' ');
        ACE_CString::npos != pos;
        pos = this->optNSOptions_.find (' ', pos + 1))
      {
        ++total_sz;
      }
    v = new char*[total_sz];
    c = 0;
    //take the 0-th argument anyway
    v[c++] = CORBA::string_dup (this->argv_[0]);

    for (int i = 1; i < total_sz; ++i) v[i] = 0;

    ssize_t p0 = 0;

    for (ssize_t p1 = this->optNSOptions_.find (' ');
        (size_t) p0 < this->optNSOptions_.length();
        p1 = this->optNSOptions_.find (' ', p0))
      {
        if (p1 == p0)
          {
            p0 = p1 + 1;
            continue;
          }
        v[c++] = CORBA::string_dup (this->optNSOptions_.substring (p0, p1 - p0).c_str());
        p0 = p1 + 1;
      }
  }

void
Starter::argCopyForNode (const char * node, int & c, char **& v)
  {
    char * validOptions[] =
      { //"--node-mgr", "-n"
        "--process-ns", "-p"
        , "--create-plan-ns", "-c"
        , "--rebind-plan-ns", "-r"
        , "--port-indirection", "-i"
        , 0
      };

    int total_sz = this->argc_ + 1;
    v = new char*[total_sz];
    for (int i = 0; i < total_sz; ++i) v[i] = 0;

    c = 0;
    v[c++] = CORBA::string_dup ("-n");
    v[c++] = CORBA::string_dup (node);
    Node n;
    if (0 == this->nodes_.find(node, n) && 0 < n.iorfile_.length())
      {
        v[c++] = CORBA::string_dup (n.iorfile_.c_str());
      }
    
    bool take = false;
    for (int i = 0; i < this->argc_; ++i)
      {
        if ('-' == this->argv_[i][0])
          {
            take = false;
            for (int j = 0; 0 != validOptions[j]; ++j)
              {
                if (this->argv_[i] == ACE_OS::strstr (this->argv_[i], validOptions[j]))
                  {
                    if (i + 1 < this->argc_
                        && (0 == ACE_OS::strcmp (this->argv_[i], "--node-mgr") || 0 == ACE_OS::strcmp (this->argv_[i], "-n")))
                      {
                        ACE_CString s = this->argv_[i+1];
                        if (ACE_CString::npos != s.find ('='))
                          {
                            break;
                          }
                        else if (s == node)
                          {
                            v[c++] = CORBA::string_dup (this->argv_[i]);
                            take = true;
                            break;
                          }
                      }
                    else
                      {
                        v[c++] = CORBA::string_dup (this->argv_[i]);
                        take = true;
                        break;
                      }
                  }
              }
          }
        else if (take)
          {
            v[c++] = CORBA::string_dup (this->argv_[i]);
          }
      }
    v[c] = 0;
  }

void
Starter::argCopyForEM (int & c, char **& v)
  {
    char * validOptions[] =
      {
        //"--node-mgr", "-n"
        "--exec-mgr", "-e"
        , "--process-ns", "-p"
        , "--create-plan-ns", "-c"
        , "--rebind-plan-ns", "-r"
        , "--port-indirection", "-i"
        , 0
      };

    int total_sz = this->argc_ + 2 * this->nodes_.total_size() + 1;
    v = new char*[total_sz];
    for (int i = 0; i < total_sz; ++i) v[i] = 0;

    bool take = false;
    c = 0;
    for (int i = 0; i < this->argc_; ++i)
      {
        if ('-' == this->argv_[i][0])
          {
            take = false;
            for (int j = 0; 0 != validOptions[j]; ++j)
              {
                if (this->argv_[i] == ACE_OS::strstr (this->argv_[i], validOptions[j]))
                  {
                    v[c++] = CORBA::string_dup (this->argv_[i]);
                    take = true;
                    break;
                  }
              }
          }
        else if (take)
          {
            v[c++] = CORBA::string_dup (this->argv_[i]);
          }
      }

    for (ACE_Map_Manager<ACE_CString, Node, ACE_Null_Mutex>::iterator it = this->nodes_.begin();
        it != this->nodes_.end();
        ++it)
      {
        //v[c++] = CORBA::string_dup("--node-mgr");
        v[c++] = CORBA::string_dup ("-n");
        ACE_CString s = (*it).ext_id_;
        s += "=";
        if ( 0 < (*it).int_id_.ior_.length() )
          {
            s += (*it).int_id_.ior_;
          }
        else if (!CORBA::is_nil((*it).int_id_.obj.in()))
          {
            s += this->orb_->object_to_string ( (*it).int_id_.obj.in());
          }
        else
          {
            DANCE_ERROR((LM_ERROR, "[%M] No IOR for node \"%s\"\n", (*it).ext_id_.c_str()));
            continue;
          }
        v[c++] = CORBA::string_dup (s.c_str());
      }

    v[c] = 0;
  }

void
Starter::argCopyForPL (int & c, char **& v)
  {
    char * validOptions[] =
      { "--em-ior", "-k"
        , "--read-plan", "-x"
        , "--read-cdr-plan", "-d"
        , "--write-cdr-plan", "-w"
        , "--plan-uuid", "-t"
        , "--dam-ior", "-a"
        , "--stop-plan", "-q"
        , 0
      };

    int total_sz = this->argc_ + 1;
    v = new char*[total_sz];
    for (int i = 0; i < total_sz; ++i) v[i] = 0;

    bool take = false;
    c = 0;
    for (int i = 0; i < this->argc_; ++i)
      {
        if ('-' == this->argv_[i][0])
          {
            take = false;
            for (int j = 0; 0 != validOptions[j]; ++j)
              {
                if (this->argv_[i] == ACE_OS::strstr (this->argv_[i], validOptions[j]))
                  {
                    v[c++] = CORBA::string_dup (this->argv_[i]);
                    take = true;
                    break;
                  }
              }
          }
        else if (take)
          {
            v[c++] = CORBA::string_dup (this->argv_[i]);
          }
      }
    v[c] = 0;
  }

void
Starter::releaseArgs (int c, char ** v)
  {
    for (int i = 0; i < c && 0 != v[i]; ++i)
      {
        CORBA::string_free (v[i]);
        v[i] = 0;
      }
    delete [] v;
  }

bool 
Starter::isPossibleOption(const char* opt)
{
  char * validOptions[] =
        { "node-mgr", "n"
          , "exec-mgr", "e"
          , "process-ns", "p"
          , "create-plan-ns", "c"
          , "rebind-plan-ns", "r"
          , "port-indirection", "i"
          , "gen-object-key", "g"
          , "log-level", "l"
          , "em-ior", "k"
          , "read-plan", "x"
          , "read-cdr-plan", "d"
          , "write-cdr-plan", "w"
          , "plan-uuid", "t"
          , "dam-ior", "a"
          , "stop-plan", "q"
          , 0
        };
  
  ACE_CString option = opt;
  for (int i = 0; 0 != validOptions[i]; ++i)
    {
      if ( option == validOptions[i]) return true;
    }
  return false;
}

} // DAnCE

