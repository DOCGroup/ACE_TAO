// $Id$
#include "Config_Handlers/DnC_Dump.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "DAnCE/Logger/Log_Macros.h"

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  try
    {
      if (argc < 1)
        {
          ACE_ERROR ((LM_ERROR,
                       ACE_TEXT ("Incorrect count of arguments. ")
                       ACE_TEXT ("Path to deployment plan has not been specified.\n")));
          return 1;
        }
      DAnCE::Config_Handlers::XML_File_Intf file (argv[1]);
      file.add_search_path (ACE_TEXT ("DANCE_ROOT"), ACE_TEXT ("/docs/schema/"));
      file.add_search_path (ACE_TEXT ("CIAO_ROOT"), ACE_TEXT ("/docs/schema/"));
      file.add_search_path (ACE_TEXT ("TAO_ROOT"), ACE_TEXT ("/docs/schema/"));
      Deployment::DeploymentPlan * plan = file.release_plan();
      Deployment::DnC_Dump::dump (*plan);
      delete plan;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("DAnCE_DeploymentPlanDump::main\n");
      return -1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unexpected exception\n")));
      return 1;
    }

  return 0;
}
