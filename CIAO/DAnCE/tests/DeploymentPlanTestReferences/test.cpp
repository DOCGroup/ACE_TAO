// $Id$
#include "ace/Auto_Ptr.h"
#include "tools/Config_Handlers/DnC_Dump.h"
#include "tools/Config_Handlers/XML_File_Intf.h"
#include "DAnCE/Logger/Log_Macros.h"

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  try
    {
      if (argc < 1)
        {
          DANCE_ERROR((LM_ERROR, "[%M] Incorrect count of arguments. Path to deployment plan has not been specified.\n"));
          return 1;
        }
      //parsing plan
      CIAO::Config_Handlers::XML_File_Intf file (argv[1]);
      file.add_search_path ("CIAO_ROOT", "/docs/schema/");
      file.add_search_path ("TAO_ROOT", "/docs/schema/");
      auto_ptr<Deployment::DeploymentPlan>  plan (file.release_plan());

      //check instance references to corresponding implementations
      for (unsigned int i = 0; i < plan->instance.length(); i++)
        {
          if (plan->instance[i].implementationRef > plan->implementation.length() - 1)
            {
              DANCE_ERROR((LM_ERROR, "[%M] incorrect implementationRef in instance %s.\n", plan->instance[i].name.in()));
              return 1;
            }
        }

      //check artifact reference for implementations of Home objects
      for (unsigned int i = 0; i < plan->implementation.length(); i++)
        {
          if (plan->implementation[i].artifactRef.length() > 0)
            {
              if (plan->implementation[i].artifactRef[0] > plan->artifact.length() - 1)
                {
                  DANCE_ERROR((LM_ERROR, "[%M] incorrect artifactRef in the implementation %s.\n", plan->implementation[i].name.in()));
                  return 1;
                }
            }
        }
    }
  catch (...)
    {
      DANCE_ERROR((LM_ERROR, "[%M] Unexpected exception"));
      return 1;
    }

  return 0;
}
