// $Id$

#include "Config_Handlers/CompPkgDesc_Handler.h"
#include "Handler_Test.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return run_test <CIAO::Config_Handler::CompPkgDesc_Handler, Deployment::ComponentPackageDescription>
    (argc, argv, &CIAO::Config_Handler::CompPkgDesc_Handler::process_ComponentPackageDescription);
}
