// $Id$

#include "Handler_Test.h"

#include "Config_Handlers/CompImplDesc_Handler.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return Config_Handler_Test::run_test 
    <CIAO::Config_Handler::CompImplDesc_Handler, 
    Deployment::ComponentImplementationDescription>
    (argc, 
     argv, 
     &CIAO::Config_Handler::CompImplDesc_Handler::process_ComponentImplementationDescription);
}

