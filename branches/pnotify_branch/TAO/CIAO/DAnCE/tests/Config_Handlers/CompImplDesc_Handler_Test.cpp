// $Id$

#include "Handler_Test.h"

#ifndef ACE_HAS_BROKEN_NESTED_TEMPLATES

#include "Config_Handlers/CompImplDesc_Handler.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return run_test <CIAO::Config_Handler::CompImplDesc_Handler, Deployment::ComponentImplementationDescription>
    (argc, argv, &CIAO::Config_Handler::CompImplDesc_Handler::process_ComponentImplementationDescription);
}

#endif // ACE_HAS_BROKEN_NESTED_TEMPLATES
