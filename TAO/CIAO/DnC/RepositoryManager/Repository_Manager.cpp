// $Id$

#include "RepositoryManager_Impl.h"

static void
usage (const ACE_TCHAR* program)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Usage: %s -u <URI>\n")
              ACE_TEXT (" <URI>: URI identifying the package\n"),
              program));
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR* package_url = 0;
  ACE_TCHAR* plan_url = 0;
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:d"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
          case 'p':
            package_url = get_opt.opt_arg();
            break;
          case 'd':
            plan_url = get_opt.opt_arg();
            break;
          default:
            usage(argv[0]);
            return -1;
        }
    }

  if (package_url == 0) {
    usage(argv[0]);
    return -1;
  }

  // Initialize the ORB so that CORBA::Any will work
  //
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "");

  try
    {
      CIAO::RepositoryManager_Impl rep_impl;
      rep_impl.installPackage ("PC", package_url);

      DOMDocument* doc = CIAO::Config_Handler::Utils::
                         create_document (plan_url);
      CIAO::Config_Handler::Plan_Handler plan_handler (doc,
                                                DOMNodeFilter::SHOW_ELEMENT |
                                                DOMNodeFilter::SHOW_TEXT);
      Deployment::DeploymentPlan plan;
      plan_handler.process_plan (plan);
      Deployment::DnC_Dump::dump (plan);
    }
  catch (CORBA::Exception& ex)
    {
      ACE_PRINT_EXCEPTION (ex, "Caught CORBA Exception: ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Caught unknown exception\n"));
      return 1;
    }

  return 0;
}
