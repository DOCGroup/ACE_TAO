// $Id$

#include "Handler_Test.h"

#include "Config_Handlers/CompIntrDesc_Handler.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return Config_Handler_Test::run_test <CIAO::Config_Handler::CompIntrDesc_Handler, Deployment::ComponentInterfaceDescription>
    (argc, argv, &CIAO::Config_Handler::CompIntrDesc_Handler::process_ComponentInterfaceDescription);
}

