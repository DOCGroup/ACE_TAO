// $Id$
#include "Plan_Launcher_Base_Impl.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/ORB_Core.h"
#include "ace/Sched_Params.h"
#include "ciao/CCM_ObjectC.h"
#include "ace/Env_Value_T.h"
#include "ace/OS.h"
#include "ace/Get_Opt.h"

namespace DAnCE
{
namespace Plan_Launcher
{

Plan_Launcher_Base_Impl::Plan_Launcher_Base_Impl(CORBA::ORB_ptr orb, int argc,
    ACE_TCHAR *argv[]) :
  orb_(CORBA::is_nil (orb) ? 0 : CORBA::ORB::_duplicate (orb)),
      em_(DAnCE::ExecutionManagerDaemon::_nil()), em_ior_("file://em.ior") //default
      , mode_(0x0)
{
  ACE_DEBUG ( (LM_TRACE, "[%M] Plan_Launcher_Base_Impl::Plan_Launcher_Base_Impl - Starting.\n"));
  if (CORBA::is_nil (this->orb_))
    {
      ACE_DEBUG ( (LM_INFO, "[%M] Plan_Launcher_Base_Impl::Plan_Launcher_Base_Impl - Creating internal ORB.\n"));
      this->orb_ = CORBA::ORB_init (argc, argv, "");
    }
  this->parse_args(argc, argv);

  ACE_DEBUG ( (LM_TRACE, "[%M] Plan_Launcher_i::init - em_ior = \"%s\"\n"
          , is_empty (this->em_ior_) ? "NULL" : this->em_ior_.c_str()));

  if (this->mode_ & (MODE_START_PLAN | MODE_STOP_PLAN))
    {
      CORBA::Object_var obj;
      if (this->em_ior_.length() > 0)
        {
          obj = this->orb_->string_to_object(this->em_ior_.c_str());
          if (CORBA::is_nil(obj))
            {
              ACE_ERROR((LM_ERROR, "[%M] Failed to retrieve EM object from \"%s\"\n", this->em_ior_.c_str()));
              throw Deployment_Failure ("Plan_Launcher_Base_Impl::Plan_Launcher_Base_Impl - failed to retrieve EM object");
            }
        }
      try
       {
         this->em_ = ::DAnCE::ExecutionManagerDaemon::_narrow (obj.in());
       }
      catch(CORBA::Exception&)
        {
          ACE_ERROR((LM_ERROR, "[%M] Failed to retrieve EM object from \"%s\"\n", this->em_ior_.c_str()));
          throw Deployment_Failure ("Plan_Launcher_Base_Impl::Plan_Launcher_Base_Impl - failed to retrieve EM object");
        }

      if (CORBA::is_nil (this->em_.in()))
        {
          ACE_ERROR ( (LM_ERROR, "[%M] Plan_Launcher_Base_Impl::Plan_Launcher_Base_Impl - failed to get an execution manager.\n"));
          throw Deployment_Failure ("Plan_Launcher_Base_Impl::Plan_Launcher_Base_Impl - failed to get an execution manager");
        }
    }
  ACE_DEBUG ( (LM_TRACE, "[%M] Plan_Launcher_Base_Impl::Plan_Launcher_Base_Impl - Obtained Execution Manager ref \n"));

  ACE_DEBUG ( (LM_TRACE, "[%M] Plan_Launcher_Base_Impl::Plan_Launcher_Base_Impl - Completed.\n"));
}

Plan_Launcher_Base_Impl::~Plan_Launcher_Base_Impl()
{
}

void Plan_Launcher_Base_Impl::execute()
{
  if (this->mode_ & MODE_START_PLAN)
    {
      size_t sz = this->cdr_plan_urls_.size();
      for (size_t i= 0; i < sz; ++i)
        {
          ACE_DEBUG ( (LM_TRACE, "[%M] execute - launching plan \"%s\"...\n", this->cdr_plan_urls_[i].c_str()));
          ::Deployment::DeploymentPlan_var plan = this->read_cdr_plan_file(this->cdr_plan_urls_[i].c_str());
          try
          {
            CORBA::String_var uuid = this->launch_plan (plan.in());
            if (0 == uuid.in ())
              {
                throw Deployment_Failure ("execute - Error launching plan\n");
              }
            ACE_DEBUG ( (LM_DEBUG, "[%M] execute - returned plan UUID is \"%s\"\n", uuid.in ()));
          }
          catch (...)
          {
            this->teardown_plan (plan->UUID.in());
            throw;
          }
        }
    }
  else if (this->mode_ & MODE_STOP_PLAN)
    {
      this->stop_plan();
    }
}

const char *Plan_Launcher_Base_Impl::launch_plan(
    const ::Deployment::DeploymentPlan &plan)
{
  try
    {
      ACE_DEBUG ( (LM_DEBUG, "[%M] launch_plan - Starting...\n"));
      if (CORBA::is_nil (this->em_.in ()))
        {
          ACE_ERROR ( (LM_ERROR, ACE_TEXT ("launch_plan - ")
                  ACE_TEXT ("launch_plan called witn an uninitialized EM.\n")));
          return 0;
        }

      ACE_DEBUG ( (LM_DEBUG,
              ACE_TEXT ("[%M] launch_plan - ")
              ACE_TEXT ("about to call this->em_->preparePlan\n")));

      ::Deployment::DomainApplicationManager_var dam;
      try
        {
          dam = this->em_->preparePlan (plan, 0);
        }
      catch(...)
        {
          ACE_ERROR((LM_ERROR, "[%M] launch_plan - An exception was thrown during EM->preparePlan.\n"));
          throw;
        }

      ACE_DEBUG ( (LM_DEBUG,
              ACE_TEXT ("[%M] launch_plan - ")
              ACE_TEXT ("after to call this->em_->preparePlan\n")));

      if (CORBA::is_nil (dam.in ()))
        {
          ACE_ERROR ( (LM_ERROR, "[%M] launch_plan - CIAO_PlanLauncher:preparePlan call failed: "
                  "nil DomainApplicationManager reference\n"));
          return 0;
        }
      else
        {
          ACE_DEBUG ( (LM_DEBUG, "[%M] launch_plan - DAM was received from preparePlan.\n"));
        }

      this->write_dam_ior (dam.in());

      ACE_DEBUG ( (LM_DEBUG, "[%M] launch_plan - Obtained DAM ref \n"));

      ::Deployment::Properties_var properties;
      ACE_NEW_RETURN (properties,
          Deployment::Properties,
          0);

      ACE_DEBUG ( (LM_DEBUG, "[%M] launch_plan - before startLaunch...\n"));

      ::Deployment::Connections_var conns;
      Deployment::Application_var da;
      try
        {
          da = dam->startLaunch (properties.in (), conns.out());
        }
      catch(...)
        {
          ACE_ERROR((LM_ERROR, "[%M] launch_plan - An exception was thrown during DAM->startLaunch.\n"));
          throw;
        }
      //Deployment::DomainApplication_var da = Deployment::DomainApplication::_narrow(obj)
      if (CORBA::is_nil (da.in()))
        {
          ACE_ERROR ( (LM_ERROR, "[%M] launch_plan - CIAO_PlanLauncher:startLaunch call failed: "
                  "nil DomainApplication reference\n"));
          return 0;
        }
      else
        {
          ACE_DEBUG ( (LM_DEBUG, "[%M] launch_plan - DomainApplication was received from startLaunch\n"));
        }
      //this is temporal workaround while cdmw checks object type in connect call
      ACE_DEBUG ( (LM_DEBUG, "[%M] Press any key after deployment on other node finished\n"));
      getchar();

      this->create_external_connections (plan, conns.inout());

      // Call finish Launch to complete the connections
      ACE_DEBUG ( (LM_DEBUG, "[%M] launch_plan - before finishLaunch\n"));
      try
        {
          da->finishLaunch (conns.in(), false); // is_ReDAC by default is <false>
        }
      catch(...)
        {
          ACE_ERROR((LM_ERROR, "[%M] launch_plan - An exception was thrown during DA->finishLaunch.\n"));
          throw;
        }

      ACE_DEBUG ( (LM_DEBUG, "[%M] launch_plan - after finishLaunch\n"));

      // Call start to activate components
      ACE_DEBUG ( (LM_DEBUG, "[%M] launch_plan - before start activating components...\n"));

      try
        {
          da->start ();
        }
      catch(...)
        {
          ACE_ERROR((LM_ERROR, "[%M] launch_plan - An exception was thrown during DA->start.\n"));
          throw;
        }

      ACE_DEBUG ( (LM_DEBUG, "[%M] launch_plan - [success]\n"));
      ACE_DEBUG ( (LM_DEBUG,
              ACE_TEXT ("[%M] Plan_Launcher_i::launch_plan - ")
              ACE_TEXT ("Application Deployed successfully\n")));

    }
  catch (const Deployment::ResourceNotAvailable& ex)
    {
      char buf[1024];
      ACE_OS::sprintf (buf, "Plan_Launcher_i::launch_plan - EXCEPTION: ResourceNotAvaiable exception caught: %s,\n"
          "Type: %s\n"
          "Property: %s\n"
          "Element: %s\n"
          "Resource: %s\n",
          ex.name.in (),
          ex.resourceType.in (),
          ex.propertyName.in (),
          ex.elementName.in (),
          ex.resourceName.in ());
      ACE_ERROR ( (LM_ERROR, "[%M] %s", buf));
      throw Deployment_Failure (buf);
    }
  catch (const Deployment::StartError& ex)
    {
      char buf[1024];
      ACE_OS::sprintf (buf, "Plan_Launcher_i::launch_plan - EXCEPTION: StartError exception caught: %s, %s\n",
          ex.name.in (),
          ex.reason.in ());
      ACE_ERROR ( (LM_ERROR, "[%M] %s", buf));
      throw Deployment_Failure (buf);
    }
  catch (const Deployment::InvalidProperty& ex)
    {
      char buf[1024];
      ACE_OS::sprintf (buf, "Plan_Launcher_i::launch_plan - EXCEPTION: InvalidProperty exception caught: %s, %s\n",
          ex.name.in (),
          ex.reason.in ());
      ACE_ERROR ( (LM_ERROR, "[%M] %s", buf));
      throw Deployment_Failure (buf);
    }
  catch (const Deployment::InvalidConnection& ex)
    {
      char buf[1024];
      ACE_OS::sprintf (buf, "Plan_Launcher_i::launch_plan - EXCEPTION: InvalidConnection exception caught: %s, %s\n",
          ex.name.in (),
          ex.reason.in ());
      ACE_ERROR ( (LM_ERROR, "[%M] %s", buf));
      throw Deployment_Failure (buf);
    }
  catch (const CORBA::Exception& ex)
    {
      char buf[1024];
      ACE_OS::sprintf (buf, "Plan_Launcher_i::launch_plan - CORBA EXCEPTION: %s\n",
          ex._info().fast_rep());
      ACE_ERROR ( (LM_ERROR, "[%M] %s", buf));
      throw Deployment_Failure (buf);
    }
  catch (...)
    {
      char buf[1024];
      ACE_OS::sprintf (buf, "Plan_Launcher_i::launch_plan - EXCEPTION: non-CORBA exception\n");
      ACE_ERROR ( (LM_ERROR, "[%M] %s", buf));
      throw Deployment_Failure (buf);
    }

  return CORBA::string_dup (plan.UUID.in());
}

bool Plan_Launcher_Base_Impl::teardown_plan(const char *uuid)
{
  try
    {
      ACE_DEBUG ( (LM_DEBUG, "[%M] teardown_plan - looking for uuid=\"%s\"\n", uuid));
      ::Deployment::DomainApplicationManagers_var mgrs = this->em_->getManagers();
      ::Deployment::DomainApplicationManager_var dapp_mgr;
      for (size_t i = 0; i < mgrs->length(); ++i)
        {
          ACE_CString s = (*mgrs) [i]->getPlan()->UUID.in();
          ACE_DEBUG ( (LM_DEBUG, "[%M] teardown_plan - comparing with existing uuid=\"%s\"\n", s.c_str()));
          if (s == uuid)
            {
              dapp_mgr = ::Deployment::DomainApplicationManager::_duplicate ( (*mgrs) [i].in());
              break;
            }
        }

      if (!::CORBA::is_nil (dapp_mgr.in()))
        {
          this->teardown_plan (dapp_mgr.in());
        }
      else
        {
          throw Deployment_Failure ("Failed to find DAM by UUID");
        }
    }
  catch (const CORBA::Exception&)
    {
      // @todo the destroy_dam_by_plan could give a stoperror exception
      // we should handle
      ACE_ERROR ( (LM_ERROR, "[%M] Unable to find DomainApplicationManager "
              "for plan with uuid: %s\n", uuid));
      return false;
    }

  return true;
}

void Plan_Launcher_Base_Impl::teardown_plan(
    ::Deployment::DomainApplicationManager_ptr dam)
{
  ACE_DEBUG ( (LM_DEBUG,
              "[%M] Plan_Launcher_Base_Impl::teardown_plan - destroy the application.....\n"));
  ::Deployment::Applications_var apps = dam->getApplications();
  if (0 == apps->length())
    {
      ACE_DEBUG((LM_ERROR, "[%M] Plan_Launcher_Base_Impl::teardown_plan - DAM has no application.\n"));
    }
  for (size_t i = 0; i < apps->length(); ++i)
    {
      dam->destroyApplication(apps[i]);
    }
  this->destroy_dam(dam);

  ACE_DEBUG ( (LM_DEBUG, "[%M] Plan_Launcher_Base_Impl::teardown_plan - [success]\n"));
}

void Plan_Launcher_Base_Impl::destroy_dam(
    ::Deployment::DomainApplicationManager_ptr dam)
{
  ACE_DEBUG ( (LM_DEBUG,
              "[%M] Plan_Launcher_Base_Impl::destroy_dam - destroy the manager.....\n"));
  this->em_->destroyManager(dam);

  ACE_DEBUG ( (LM_DEBUG, "[%M] Plan_Launcher_Base_Impl::destroy_dam - [success]\n"));
}

void Plan_Launcher_Base_Impl::usage(const ACE_TCHAR* program)
{
  if (0 == program)
    {
      ACE_ERROR ( (LM_ERROR, ACE_TEXT ("[(%P|%t) Executor] Usage: %s <options>\n"), program));
    }

  ACE_ERROR ( (LM_ERROR, ACE_TEXT ("Options : \n")
          ACE_TEXT ("-k|--em-ior <EXECUTION_MANAGER_IOR>")
          ACE_TEXT (" : Default file://em.ior\n")
          ACE_TEXT ("-d|--read-cdr-plan <CDR_DEPLOYMENT_PLAN_URL>\n")
          ACE_TEXT ("-w|--write-cdr-plan <CDR_DEPLOYMENT_PLAN_URL>\n")
          ACE_TEXT ("-t|--plan-uuid <PLAN_UUID>\n")
          ACE_TEXT ("-a|dam-ior <DOMAIN_APPLICATION_MANAGER_IOR>\n")
          //                    ACE_TEXT ("-n|--node-mgr : Use naming service to fetch EM\n")
          ACE_TEXT ("-q|--stop-plan : Stop the plan\n")
          ACE_TEXT ("-h|--help : Show this usage information\n")
          ACE_TEXT ("-x|--read-plan <XML_DEPLOYMENT_PLAN_URL>")));
}

void Plan_Launcher_Base_Impl::parse_args(int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ( (LM_TRACE, "[%M] PL options : \""));
  for (int i = 0; i < argc; ++i)
    {
      ACE_DEBUG ( (LM_TRACE, " %s", argv[i]));
    }
  ACE_DEBUG ( (LM_TRACE, "\"\n"));

  ACE_Get_Opt get_opt(argc, argv,
  ACE_TEXT ("k:d:w:t:a:qx:h"), 0);
  get_opt.long_option("em-ior", 'k', ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option("read-cdr-plan", 'd', ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option("write-cdr-plan", 'w', ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option("plan-uuid", 't', ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option("dam-ior", 'a', ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option("stop-plan", 'q', ACE_Get_Opt::NO_ARG);
  get_opt.long_option("read-plan", 'x', ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option("help", 'h', ACE_Get_Opt::NO_ARG);

  int c;
  ACE_CString s;
  while ( (c = get_opt ()) != EOF)
    {
      switch (c)
        {
          case 'k':
            this->em_ior_ = expand_env_vars (get_opt.opt_arg());
            break;
          case 'd':
            s = expand_env_vars (get_opt.opt_arg());
            // trim leading file://
            if (0 < s.length())
              {
                ssize_t pos = s.find("file://");
                if (0 == pos)
                  {
                    s = s.substring(7);
                  }
                this->cdr_plan_urls_.push_back(s);
              }
            break;
          case 'w':
            if (0 < this->cdr_dest_url_.length())
              {
                ACE_ERROR((LM_ERROR, "[%M] Ambiguity: more then one --write-cdr-plan option.\n"));
                throw Deployment_Failure("Ambiguity: more then one --write-cdr-plan option.");
              }
            this->cdr_dest_url_ = expand_env_vars (get_opt.opt_arg());
            this->mode_ |= MODE_WRITE_CDR;
            // trim leading file://
            if (0 < this->cdr_dest_url_.length())
              {
                ssize_t pos = this->cdr_dest_url_.find("file://");
                if (0 == pos)
                  {
                    this->cdr_dest_url_= this->cdr_dest_url_.substring(7);
                  }
              }
            break;
          case 't':
            this->plan_uuid_ = get_opt.opt_arg();
            break;
          case 'a':
            this->dam_ior_ = expand_env_vars (get_opt.opt_arg());
            ACE_DEBUG((LM_TRACE, "[%M] Parsed DAM IOR : %s\n", this->dam_ior_.c_str()));
            break;
          case 'q':
            this->mode_ |= MODE_STOP_PLAN;
            break;
          case 'x':
            this->xml_plan_urls_.push_back(expand_env_vars (get_opt.opt_arg()));
            break;
          case 'h':
          default:
            usage (argv[0]);
            throw Deployment_Failure ("parse_args : invalid arguments");
        }
    }
  if (0 == (this->mode_ & (MODE_STOP_PLAN | MODE_WRITE_CDR)))
    {
      this->mode_ |= MODE_START_PLAN;
    }
  // Post-check
  this->check_mode_consistentness();
}

void Plan_Launcher_Base_Impl::write_dam_ior(
    ::Deployment::DomainApplicationManager_ptr dam)
{
  if (is_empty (this->dam_ior_))
    return;

  CORBA::String_var ior = this->orb_->object_to_string(dam);

  FILE* ior_output_file = ACE_OS::fopen (this->dam_ior_.c_str(), "w");

  if (ior_output_file)
    {
      ACE_OS::fprintf (ior_output_file, "%s", ior.in());
      ACE_OS::fclose (ior_output_file);
    }
  else
    {
      ACE_ERROR ( (LM_ERROR, "[%M] PlanLauncher.cpp::write_dap_ior - Error in opening file %s to write DAM IOR: %m",
              this->dam_ior_.c_str()));
      throw Deployment_Failure (ACE_CString ("write_dap_ior - Error in opening file %s to write DAM IOR: %m") +
          this->dam_ior_);
    }
}

void Plan_Launcher_Base_Impl::stop_plan()
{
  bool stopped = false;
  if (!is_empty (this->plan_uuid_))
    {
      stopped = true;
      ACE_TRACE((LM_TRACE, "[%M] Stopping plan \"%s\"\n", this->plan_uuid_.c_str()));
      if (!this->teardown_plan(this->plan_uuid_.c_str()))
        {
          ACE_ERROR ( (LM_ERROR, "[%M] stop_plan - tear down assembly failed: unknown plan uuid.\n"));
        }
    }

  if (!is_empty (this->dam_ior_))
    {
      stopped = true;
      ACE_TRACE((LM_TRACE, "[%M] Stopping plan by DAM IOR.\n"));
      CORBA::Object_var
        obj = this->orb_->string_to_object(this->dam_ior_.c_str());
      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "[%M] stop_plan - tear down assembly failed: DAM IOR is invalid.\n"));
        }
      else
        {
          Deployment::DomainApplicationManager_var
            dam = Deployment::DomainApplicationManager::_narrow (obj.in());
          this->teardown_plan(dam.in());
        }
    }

  if (0 < this->cdr_plan_urls_.size())
    {
      stopped = true;
      size_t sz = this->cdr_plan_urls_.size();
      for (size_t i = 0; i < sz; ++i)
        {
          ACE_TRACE((LM_TRACE, "[%M] Stopping plan by plan file : %s\n", this->cdr_plan_urls_[i].c_str()));
          ::Deployment::DeploymentPlan_var plan = this->read_cdr_plan_file(this->cdr_plan_urls_[i].c_str());
          if (!this->teardown_plan(plan->UUID.in()))
            {
              ACE_ERROR ( (LM_ERROR, "[%M] stop_plan - tear down assembly failed: unknown plan uuid.\n"));
            }
        }
    }

  if (!stopped && 0 == this->xml_plan_urls_.size())
    {
      ACE_TRACE((LM_TRACE, "[%M] Stopping plan all running plans.\n"));
      ::Deployment::DomainApplicationManagers_var
          mgrs = this->em_->getManagers();
      ::Deployment::DomainApplicationManager_var dapp_mgr;
      for (size_t i = 0; i < mgrs->length(); ++i)
        {
          this->teardown_plan((*mgrs) [i]);
        }
    }
}

void Plan_Launcher_Base_Impl::create_external_connections(
    const ::Deployment::DeploymentPlan &plan, Deployment::Connections& conn)
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] create_external_connections - start\n"));
  /*    CORBA::Object_var obj = this->orb_->resolve_initial_references("NameService");
   CosNaming::NamingContext_var naming = CosNaming::NamingContext::_narrow(obj.in());
   CosNaming::BindingList_var bl;
   CosNaming::BindingIterator_var bi;
   //naming->list(10L, bl.out(), bi.out());
   CosNaming::Name name(1);
   name.length(3);
   name[0].id = CORBA::string_dup("CDMW");//CORBA::string_dup("CcmDance1Test");
   name[1].id = CORBA::string_dup ("SERVICES");
   name[2].id = CORBA::string_dup("ASSEMBLYANDDEPLOYMENT");
   obj = naming->resolve(name);
   ACE_DEBUG((LM_DEBUG, "[%M] create_external_connections - After resolve\n"));
   CosNaming::NamingContext_var CcmDance1Test = CosNaming::NamingContext::_narrow(obj.in());
   CcmDance1Test->list(10L, bl.out(), bi.out());
   bool exit = false;
   while (!exit)
   {
   char buf[1024];
   ACE_OS::sprintf(buf, "create_external_connections - naming size is %u\n", bl->length());
   ACE_DEBUG((LM_DEBUG, buf));
   for ( size_t i = 0; i< bl->length(); i++ )
   {
   for ( size_t j = 0; j < (*bl)[i].binding_name.length(); j++ )
   {
   ACE_OS::sprintf(buf, "create_external_connections - Binding name %s, binding kind %s, binding type %i\n"
   , (*bl)[i].binding_name[j].id.in()
   , (*bl)[i].binding_name[j].kind.in()
   , (*bl)[i].binding_type);
   ACE_DEBUG((LM_DEBUG, buf));
   }
   }
   if(!bi->next_n(10L, bl.out()))
   {
   exit = true;
   }
   }
   */
  for (unsigned int i = 0; i < plan.connection.length(); i++)
    {
      if (plan.connection[i].externalReference.length() > 0
          && plan.connection[i].externalReference[0].provider)
        {
          ACE_DEBUG ( (LM_DEBUG,
                  "Plan_Launcher_i::create_external_connections - create connection %C from IOR %C\n",
                  plan.connection[i].name.in(),
                  plan.connection[i].externalReference[0].location.in()));
          unsigned int indx = conn.length();
          conn.length(indx + 1);
          conn[indx].name= CORBA::string_dup (plan.connection[i].name.in());
          conn[indx].endpoint.length(1L);
          CORBA::Object_ptr
              obj = this->orb_->string_to_object(plan.connection[i].externalReference[0].location.in());
          if (!CORBA::is_nil (obj))
            {
              //Components::CCMObject_ptr ccmobj = Components::CCMObject::_narrow (obj);
              conn[indx].endpoint[0] = obj;
            }
          else
            {
              ACE_DEBUG ( (LM_DEBUG,
                      "Plan_Launcher_i::create_external_connections - can't create object for IOR %C\n",
                      plan.connection[i].externalReference[0].location.in()));
              throw 0;
            }
        }
    }
}

Deployment::DeploymentPlan*Plan_Launcher_Base_Impl::read_cdr_plan_file(
    const char *cdr_plan_uri)
{
  Deployment::DeploymentPlan* res = 0;
  try
    {
      FILE* file = ACE_OS::fopen (cdr_plan_uri, "r");
      if (0 == file)
        {
          ACE_CString s = "read_cdr_plan_file : failed to open file \"";
          s += cdr_plan_uri;
          s += "\"";
          throw Deployment_Failure (s.c_str());
        }

      size_t buf_size;
      ACE_OS::fread (&buf_size, sizeof (buf_size), 1, file);

      char * buf = new char[buf_size];
      ACE_OS::fread (buf, 1, buf_size, file);

        {
          TAO_InputCDR cdr (buf, buf_size);
          ACE_NEW_THROW_EX (res,
              Deployment::DeploymentPlan (),
              CORBA::NO_MEMORY ());
          cdr >> *res;
        }
      delete [] buf;
      ACE_OS::fclose (file);
    }
  catch(...)
    {
      ACE_ERROR((LM_ERROR, "[%M] Failed to read CDR plan file.\n"));
      throw Deployment_Failure("Failed to read CDR plan file.");
    }
  return res;
}

void Plan_Launcher_Base_Impl::write_cdr_plan_file(const char * filename,
    const Deployment::DeploymentPlan & plan)
{
  try
    {
      TAO_OutputCDR cdr;
      cdr << plan;
      size_t buf_size = cdr.total_length();
      FILE * file = ACE_OS::fopen (filename, "w");
      if (0 == file)
        {
          throw Deployment_Failure ("write_cdr_plan_file : failed to open file.");
        }
      ACE_OS::fwrite (&buf_size, sizeof (buf_size), 1, file);
      for (const ACE_Message_Block *i = cdr.begin ();
          i != 0;
          i = i->cont ())
        {
          ACE_OS::fwrite (i->rd_ptr (), 1, i->length (), file);
        }
      ACE_OS::fclose (file);
    }
  catch(...)
    {
      ACE_ERROR((LM_ERROR, "[%M] Failed to write CDR plan file.\n"));
      throw Deployment_Failure("Failed to write CDR plan file.");
    }
}

void
Plan_Launcher_Base_Impl::check_mode_consistentness()
{
  if (this->mode_ & MODE_START_PLAN)
    {
      if (0 == this->cdr_plan_urls_.size() && 0 == this->xml_plan_urls_.size())
        {
          throw Deployment_Failure ("check_mode_consistentness : no plan to start");
          }
      }
    else if (this->mode_ & MODE_STOP_PLAN)
      {
        //          if (is_empty (this->cdr_plan_url_)
        //              && is_empty (this->xml_plan_url_)
        //              && is_empty (this->plan_uuid_)
        //              && is_empty (this->dam_ior_))
        //            {
        //              throw Deployment_Failure ("check_mode_consistentness : no plan to stop");
        //            }
      }
    else if (this->mode_ & MODE_WRITE_CDR)
      {
        if (is_empty (this->cdr_dest_url_))
          {
            throw Deployment_Failure ("check_mode_consistentness : no cdr url for writing plan.");
          }
        if (1 != this->xml_plan_urls_.size())
          {
            throw Deployment_Failure ("check_mode_consistentness : not one source url for writing plan.");
          }
      }
    else
      {
        throw Deployment_Failure ("check_mode_consistentness : no action specified");
      }
  }

ACE_CString
Plan_Launcher_Base_Impl::expand_env_vars (const char * s)
  {
    ACE_CString src = s;
    ACE_CString res;
    ssize_t pos_done = 0;
    while (pos_done < (ssize_t) src.length())
      {
        ssize_t pos_start = src.find ('$', pos_done);
        if (ACE_CString::npos == pos_start)
          {
            res += src.substring (pos_done);
            pos_done = src.length();
          }
        else // take the substring before '$' and append value

          {
            if (pos_start > pos_done)
              {
                res += src.substring (pos_done, pos_start - pos_done);
                pos_done = pos_start;
              }

            ssize_t pos_end = src.length();

            ssize_t p;

            p = src.find (' ', pos_start + 1);
            if (ACE_CString::npos != p && pos_end > p) pos_end = p;

            p = src.find ('/', pos_start + 1);
            if (ACE_CString::npos != p && pos_end > p) pos_end = p;

            p = src.find ('\\', pos_start + 1);
            if (ACE_CString::npos != p && pos_end > p) pos_end = p;

            p = src.find ('$', pos_start + 1);
            if (ACE_CString::npos != p && pos_end > p) pos_end = p;

            if (pos_end - pos_start > 1)
              {
                ACE_Env_Value<const char*> val (src.substring (pos_start + 1, pos_end - pos_start - 1).c_str(), 0);
                res += val;
                pos_done = pos_end;
              }
            else
              {
                ACE_DEBUG ( (LM_WARNING
                        , "Envvar can not be parsed out at %i in \"%s\""
                        , pos_start
                        , src.c_str()));
              }
          }
      }
    return res;
  }

} // Plan_Launcher
} // DAnCE
