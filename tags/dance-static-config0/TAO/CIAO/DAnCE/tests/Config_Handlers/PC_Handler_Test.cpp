// $Id$

#include "Handler_Test.h"

#include "Config_Handlers/PC_Handler.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return Config_Handler_Test::run_test <CIAO::Config_Handler::PC_Handler, Deployment::PackageConfiguration>
    (argc, argv, &CIAO::Config_Handler::PC_Handler::process_PackageConfiguration);
}

