// $Id$

#include "Handler_Test.h"

#ifndef ACE_HAS_BROKEN_NESTED_TEMPLATES

#include "Config_Handlers/IAD_Handler.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return run_test <CIAO::Config_Handler::IAD_Handler, Deployment::ImplementationArtifactDescription>
    (argc, argv, &CIAO::Config_Handler::IAD_Handler::process_ImplementationArtifactDescription);
}

#endif // ACE_HAS_BROKEN_NESTED_TEMPLATES
