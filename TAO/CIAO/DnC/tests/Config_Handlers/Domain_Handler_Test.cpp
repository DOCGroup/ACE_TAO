// $Id$

#include "Config_Handlers/Domain_Handler.h"
#include "Handler_Test.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return run_test <CIAO::Config_Handler::Domain_Handler, Deployment::Domain>
    (argc, argv, &CIAO::Config_Handler::Domain_Handler::process_domain);
}
