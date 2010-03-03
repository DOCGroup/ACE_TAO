// $Id$

#include "Plan_Launcher_Impl.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "Config_Handlers/DnC_Dump.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"

Plan_Launcher_Impl::~Plan_Launcher_Impl (void)
{
}

::Deployment::DeploymentPlan *
Plan_Launcher_Impl::load_xml_plan(const ACE_TCHAR *deployment_plan_uri)
{
  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("Plan_Launcher_Impl::load_xml_plan - ")
                ACE_TEXT("Parsing plan \"%C\"...\n"), deployment_plan_uri));

  ::Deployment::DeploymentPlan * plan = 0;
  try
    {
      CIAO::Config_Handlers::XML_File_Intf intf (deployment_plan_uri);
      intf.add_search_path (ACE_TEXT("DANCE_ROOT"), ACE_TEXT("/docs/schema/"));
      intf.add_search_path (ACE_TEXT("CIAO_ROOT"), ACE_TEXT("/docs/schema/"));
      intf.add_search_path (ACE_TEXT("TAO_ROOT"), ACE_TEXT("/docs/schema/"));
      plan = intf.release_plan ();
      if (0 == plan)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Impl::load_xml_plan - ")
                        ACE_TEXT("Failed to parse plan \"%C\".\n"), deployment_plan_uri));
          throw Deployment_Failure("Failed to parse plan.");
        }
    }
  catch (...)
    {
      ACE_TString s = ACE_TEXT ("failed to parse deployment plan \"");
      s += deployment_plan_uri;
      s += ACE_TEXT ("\"");
      DANCE_DEBUG (6, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Impl::load_xml_plan - %s\n"), s.c_str()));
      throw Deployment_Failure (ACE_TEXT_ALWAYS_CHAR (s.c_str()));
    }

  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("Plan_Launcher_Impl::load_xml_plan - Parsing complete....\n")));
  return plan;
}

void Plan_Launcher_Impl::execute()
{
  DANCE_TRACE ("Plan_Launcher_Impl::execute()");

  if (this->mode_ & MODE_START_PLAN)
    {
      size_t const sz = this->xml_plan_urls_.size();
      for (size_t i= 0; i < sz; ++i)
        {
          DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("Plan_Launcher_Impl::execute - ")
                        ACE_TEXT("launching plan \"%C\"...\n"), this->xml_plan_urls_[i].c_str()));
          ::Deployment::DeploymentPlan_var plan =
              this->load_xml_plan(ACE_TEXT_CHAR_TO_TCHAR (this->xml_plan_urls_[i].c_str()));
          try
          {
            CORBA::String_var uuid = this->launch_plan (plan.in());
            if (0 == uuid.in ())
              {
                throw Deployment_Failure ("execute - Error launching plan\n");
              }
            DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("Plan_Launcher_Impl::execute - ")
                          ACE_TEXT("returned plan UUID is \"%C\"\n"), uuid.in ()));
          }
          catch (...)
          {
            this->teardown_plan (plan->UUID.in());
            throw;
          }
        }
      this->Plan_Launcher_Base_Impl::execute();
    }

  if (this->mode_ & MODE_STOP_PLAN)
    {
      this->stop_plan();
    }

  if (this->mode_ & MODE_WRITE_CDR)
    {
      ::Deployment::DeploymentPlan_var plan = this->load_xml_plan(ACE_TEXT_CHAR_TO_TCHAR (this->xml_plan_urls_[0].c_str()));
      this->write_cdr_plan_file(this->cdr_dest_url_.c_str(), plan);
    }
}

void Plan_Launcher_Impl::stop_plan()
{
  DANCE_TRACE ("Plan_Launcher_Impl::stop_plan");

  if (0 < this->xml_plan_urls_.size())
    {
      size_t const sz = this->xml_plan_urls_.size();
      for (size_t i= 0; i < sz; ++i)
        {
          DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("Plan_Launcher_Impl::stop_plan - ")
                       ACE_TEXT("Stopping plan by plan file: %C\n"), this->xml_plan_urls_[i].c_str()));
          ::Deployment::DeploymentPlan_var plan =
            this->load_xml_plan(ACE_TEXT_CHAR_TO_TCHAR (this->xml_plan_urls_[i].c_str()));
          if (!this->teardown_plan(plan->UUID.in()))
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Impl::stop_plan - ")
                            ACE_TEXT("tear down assembly failed: unknown plan uuid.\n")));
            }
        }
    }
  this->Plan_Launcher_Base_Impl::stop_plan();
}

