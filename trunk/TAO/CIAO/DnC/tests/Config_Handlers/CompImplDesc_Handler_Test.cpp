// $Id$

#include "Config_Handlers/CompImplDesc_Handler.h"
#include "Handler_Test.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return run_test <CIAO::Config_Handler::CompImplDesc_Handler, Deployment::ComponentImplementationDescription>
    (argc, argv, &CIAO::Config_Handler::CompImplDesc_Handler::process_ComponentImplementationDescription);
}
