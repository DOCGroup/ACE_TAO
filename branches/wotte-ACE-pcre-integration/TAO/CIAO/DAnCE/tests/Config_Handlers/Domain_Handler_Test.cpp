// $Id$

#include "Handler_Test.h"

#ifndef ACE_HAS_BROKEN_NESTED_TEMPLATES

#include "Config_Handlers/Domain_Handler.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return run_test <CIAO::Config_Handler::Domain_Handler, Deployment::Domain>
    (argc, argv, &CIAO::Config_Handler::Domain_Handler::process_domain);
}

#endif // ACE_HAS_BROKEN_NESTED_TEMPLATES
