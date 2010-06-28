// $Id$

#include "ace/Dynamic_Service.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "DAnCE/Deployment/Deployment_DeploymentPlanC.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/Logger/Logger_Service.h"
#include "DAnCE/Config_Handlers/XML_File_Intf.h"
#include "DAnCE/tools/Convert_Plan/Convert_Plan_Impl.h"
#include "Artifact_Installation_Impl.h"
#include "Artifact_Installation_Handler_Svc.h"
#include "Installation_Repository_Manager_Impl.h"
#include "File_Installation_Handler.h"
#include "Http_Installation_Handler.h"

bool cdr_encoded_ = false;
const ACE_TCHAR *input_filename = 0;

void
usage (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("usage: dance_artifact_installation [options]\n")
              ACE_TEXT ("Reads a deployment plan (XML or CDR) and installs any described artifacts\n")
              ACE_TEXT ("\t-c <plan>\t\tCDR Encoded input plan\n")
              ACE_TEXT ("\t-x <plan>\t\tXML Encoded input plan\n")
              ));
}

bool
parse_args (int argc, ACE_TCHAR *argv [])
{
  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT ("dance_artifact_installation options: ")));

  for (int i = 0; i < argc; ++i)
    {
      DANCE_DEBUG (9, (LM_TRACE, ACE_TEXT("\t%s\n"), argv[i]));
    }

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("c:x:h"), 0);

  int c;
  ACE_CString s;
  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 'c':
          cdr_encoded_ = true;
          input_filename = get_opt.opt_arg ();
          break;

        case 'x':
          cdr_encoded_ = false;
          input_filename = get_opt.opt_arg ();
          break;

        case 'h':
          usage ();
          return false;
        default:
          usage ();
          return false;
        }
    }

  return true;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  DANCE_DISABLE_TRACE ();

  int retval = 0;

  try
    {
      DAnCE::Logger_Service
        * dlf = ACE_Dynamic_Service<DAnCE::Logger_Service>::instance ("DAnCE_Logger");

      if (dlf)
        {
          dlf->init (argc, argv);
        }

      DAnCE::InstallationRepositoryManagerSvc
        * dirms = ACE_Dynamic_Service<DAnCE::InstallationRepositoryManagerSvc>::instance ("InstallationRepositoryManager");

      if (dirms)
        {
          dirms->init (argc, argv);
        }

      DAnCE::ArtifactInstallationHandlerSvc
        * dfihs = ACE_Dynamic_Service<DAnCE::ArtifactInstallationHandlerSvc>::instance ("FileInstallationHandler");

      if (dfihs)
        {
          dfihs->init (argc, argv);
        }

      DAnCE::ArtifactInstallationHandlerSvc
        * dhihs = ACE_Dynamic_Service<DAnCE::ArtifactInstallationHandlerSvc>::instance ("HttpInstallationHandler");

      if (dhihs)
        {
          dhihs->init (argc, argv);
        }

      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                       ACE_TEXT("dance_artifact_installation - initializing ORB\n")));

      // Need an ORB for the Config handlers
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (!parse_args (argc, argv))
        {
          return -1;
        }

      auto_ptr<Deployment::DeploymentPlan> plan;

      if (!cdr_encoded_)
        {
          plan.reset (DAnCE::Convert_Plan::read_xml_plan (input_filename));
        }
      else
        {
          plan.reset (DAnCE::Convert_Plan::read_cdr_plan (input_filename));
        }

      if (plan.get () == 0)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO "dance_artifact_installation - "
                           "Unable to convert provided plan into IDL representation\n"));
          return 0;
        }

      // instantiate artifact installation service
      DAnCE::ArtifactInstallation_Impl* installer;
      ACE_NEW_RETURN (installer,
                      DAnCE::ArtifactInstallation_Impl (),
                      1);
      PortableServer::Servant_var<DAnCE::ArtifactInstallation_Impl> pinstaller (installer);

      pinstaller->initialize ();

      // install artifacts
      for (CORBA::ULong i=0;
           i < plan->artifact.length ();
           ++i)
        {
          pinstaller->install(plan->UUID.in (),
                  plan->artifact[i]);
        }

      pinstaller->clear ();
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("dance_artifact_installation");
      retval = 1;
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, "dance_artifact_installation - error: unknown c++ exception\n"));
      retval = 1;
    }

  return retval;
}
