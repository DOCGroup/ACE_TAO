// $Id$

#include "Handler_Test.h"

#include "Config_Handlers/Plan_Handler.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return Config_Handler_Test::run_test <CIAO::Config_Handler::Plan_Handler, Deployment::DeploymentPlan>
           (argc, argv, &CIAO::Config_Handler::Plan_Handler::process_plan);
}

