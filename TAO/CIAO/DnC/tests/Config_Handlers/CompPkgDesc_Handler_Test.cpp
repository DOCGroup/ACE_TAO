// $Id$

#include "Handler_Test.h"

#ifndef ACE_HAS_BROKEN_NESTED_TEMPLATES

#include "Config_Handlers/CompPkgDesc_Handler.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return run_test <CIAO::Config_Handler::CompPkgDesc_Handler, Deployment::ComponentPackageDescription>
    (argc, argv, &CIAO::Config_Handler::CompPkgDesc_Handler::process_ComponentPackageDescription);
}

#endif // ACE_HAS_BROKEN_NESTED_TEMPLATES
