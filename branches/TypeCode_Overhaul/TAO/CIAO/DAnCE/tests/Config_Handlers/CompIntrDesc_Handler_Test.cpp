// $Id$

#include "Handler_Test.h"

#ifndef ACE_HAS_BROKEN_NESTED_TEMPLATES

#include "Config_Handlers/CompIntrDesc_Handler.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return run_test <CIAO::Config_Handler::CompIntrDesc_Handler, Deployment::ComponentInterfaceDescription>
    (argc, argv, &CIAO::Config_Handler::CompIntrDesc_Handler::process_ComponentInterfaceDescription);
}

#endif // ACE_HAS_BROKEN_NESTED_TEMPLATES
