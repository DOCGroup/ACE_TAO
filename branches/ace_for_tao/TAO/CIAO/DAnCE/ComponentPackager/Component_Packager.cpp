// $Id$

#include "ace/Get_Opt.h"
#include "ciao/DeploymentC.h"
#include "ciao/CIAO_common.h"
#include "CPK_Packager.h"
#include "PDL_Handler.h"

namespace CIAO
{
  namespace Component_Packager
  {
    // URL for flattened component deployment plan
    const char* cdp_url = 0;
    // URL for .cid file, which will help to archive
    // the standard descriptors into package
    const char* pdl_url = 0;
    // URL for component package to be produced
    const char* pkg_url = "ComponentPackage.cpk";
    // Indicate whether to include standard descriptors
    // in the package or not
    bool include_std_desc = false;


    static void
    display_usage (const ACE_TCHAR* program)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("[CIAO::Component_Packager] Usage:\n")
                  ACE_TEXT ("  -h                   Display usage information for commands\n")
                  ACE_TEXT ("  -p flatplan(.cdp)    Use 'flatplan'as the input to produce the package\n")
                  ACE_TEXT ("  -i descriptor(.cid)  Use 'descriptor' as addition input to archive all\n")
                  ACE_TEXT ("                       the standard descriptors\n")
                  ACE_TEXT ("  -o package(.cpk)     Set the output package name.Use 'ComponentPackage.cpk'\n")
                  ACE_TEXT ("                       if this option is not specified.\n")));
    }


    static bool
    parse_args (int argc,
                ACE_TCHAR *argv[])
    {
      ACE_Get_Opt get_opt (argc,
                           argv,
                           ACE_TEXT ("p:i:o:h"));
      int c;

      while ((c = get_opt ()) != EOF)
        {
          switch (c)
            {
            case 'p':
              cdp_url = get_opt.opt_arg ();
              break;
            case 'i':
              include_std_desc = true;
              pdl_url = get_opt.opt_arg ();
              break;
            case 'o':
              pkg_url = get_opt.opt_arg ();
              break;
            case 'h':
            default:
              display_usage (argv[0]);
              return false;
            }
        }

      if (cdp_url == 0)
      {
        display_usage (argv[0]);
        return false;
      }

      if (pdl_url == 0 && include_std_desc)
      {
        display_usage (argv[0]);
        return false;
      }

      if (pkg_url == 0)
      {
        display_usage (argv[0]);
        return false;
      }

      return true;
    }


    static int
    run_main (int argc, char *argv[])
    {
      ACE_DECLARE_NEW_CORBA_ENV;

      ACE_TRY
      {
        if (parse_args (argc, argv) == false)
        {
          return -1;
        }

        CPK_Packager packager (pkg_url,
                               cdp_url,
                               pdl_url,
                               include_std_desc);

        Component_Packager::Base_Packager::PACKAGE_PLAN pkg_plan;

        if (-1 == packager.preparePackage (pkg_plan))
        {
          ACE_ERROR ((LM_ERROR,
                     "(%P|%t) Component_Packager: Error preparing package plan\n"));
          throw;
        };
        if (-1 == packager.createPackage (pkg_plan))
        {
          ACE_ERROR ((LM_ERROR,
                     "(%P|%t) Component_Packager: Error creating package plan\n"));
          throw;
        };

         ACE_DEBUG ((LM_INFO,
                     "(%P|%t)[success] The Component Package has been created!"));

      }
      ACE_CATCHANY
      {
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "(%P|%t) Component_Packager: ");
        return -1;
      }
      ACE_CATCHALL
      {
        ACE_ERROR ((LM_ERROR,
                   "(%P|%t) Component_Packager: Unknown exception.\n"));
        return -1;
      }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (-1);

      return 0;
    }
  }
}

using namespace CIAO::Component_Packager;

int
ACE_TMAIN (int argc,
           ACE_TCHAR *argv[])
{
  return run_main (argc, argv);
}
