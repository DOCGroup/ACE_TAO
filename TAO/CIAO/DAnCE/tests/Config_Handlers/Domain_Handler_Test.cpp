// $Id$

#include "Handler_Test.h"

#include "Config_Handlers/Domain_Handler.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return Config_Handler_Test::run_test <CIAO::Config_Handler::Domain_Handler, Deployment::Domain>
    (argc, argv, &CIAO::Config_Handler::Domain_Handler::process_domain);
}

