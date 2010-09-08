// $Id$

#include "ace/Auto_Ptr.h"
#include "DAnCE/Deployment/Deployment_TargetDataC.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/Logger/Logger_Service.h"
#include "Config_Handlers/DnC_Dump.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "Config_Handlers/Common.h"

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  DANCE_DISABLE_TRACE ();

  try
    {
      DAnCE::Logger_Service
        *dlf = ACE_Dynamic_Service<DAnCE::Logger_Service>::instance ("DAnCE_Logger");

      if (dlf)
        {
          dlf->init (argc, argv);
        }

      if (argc < 1)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Incorrect count of arguments. Path to deployment plan has not been specified.\n")));
          return 1;
        }

      DAnCE::Config_Handlers::XML_File_Intf file (argv[1]);
      file.add_search_path (ACE_TEXT ("DANCE_ROOT"), ACE_TEXT ("/docs/schema/"));
      file.add_search_path (ACE_TEXT ("CIAO_ROOT"), ACE_TEXT ("/docs/schema/"));
      file.add_search_path (ACE_TEXT ("TAO_ROOT"), ACE_TEXT ("/docs/schema/"));
      ::Deployment::Domain *plan = file.release_domain ();
      if (plan)
        {
          ACE_ERROR ((LM_ERROR, "No problems were found in domain <%C>\n",
                    argv[1]));
          return 0;
        }
    }
  catch (const DAnCE::Config_Handlers::Config_Error &ex)
    {
      DANCE_ERROR (1, (LM_ERROR, "Caught config error while parsing XML into IDL: %C:%C\n",
                      ex.name_.c_str (), ex.error_.c_str ()));
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: Unexpected exception caught while parsing domain <%C>\n",
                argv[1]));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "ERROR: Problems were found in <%C>\n",
              argv[1]));
  return 1;
}
