// $Id$
#include "ace/Auto_Ptr.h"
#include "Config_Handlers/DnC_Dump.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "DAnCE/Logger/Log_Macros.h"

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  try
    {
      if (argc < 1)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Incorrect count of arguments. Path to deployment plan has not been specified.\n")));
          return 1;
        }
      //parsing plan
      DAnCE::Config_Handlers::XML_File_Intf file (argv[1]);
      file.add_search_path (ACE_TEXT ("DANCE_ROOT"), ACE_TEXT ("/docs/schema/"));
      file.add_search_path (ACE_TEXT ("CIAO_ROOT"), ACE_TEXT ("/docs/schema/"));
      file.add_search_path (ACE_TEXT ("TAO_ROOT"), ACE_TEXT ("/docs/schema/"));
      auto_ptr<Deployment::DeploymentPlan>  plan (file.release_plan());

      //check instance references to corresponding implementations
      for ( ::CORBA::ULong i = 0; i < plan->instance.length(); i++)
        {
          if (plan->instance[i].implementationRef > plan->implementation.length() - 1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("incorrect implementationRef in instance %C.\n"), plan->instance[i].name.in()));
              return 1;
            }
        }

      //check artifact reference for implementations of Home objects
      for ( ::CORBA::ULong i = 0; i < plan->implementation.length(); i++)
        {
          if (plan->implementation[i].artifactRef.length() > 0)
            {
              if (plan->implementation[i].artifactRef[0] > plan->artifact.length() - 1)
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("incorrect artifactRef in the implementation %C.\n"), plan->implementation[i].name.in()));
                  return 1;
                }
            }
        }
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unexpected exception")));
      return 1;
    }

  return 0;
}
