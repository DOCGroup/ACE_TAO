// $Id$

#include "Static_Assembly_Visitors.h"
#include "../XML_Helpers/Assembly_Spec.h"
#include "../XML_Helpers/XML_Utils.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"

char *config_filename = 0;
char *cads_filename = 0;
char *cad_filename = 0;
char *installation_file = "CIAO_Installation_Data.ini";

CIAO::Static_Config::Static_Config_Info static_config_info;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "a:c:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        cad_filename = get_opts.opt_arg ();
        break;

      case 'c': // Get the configuration file for deployment configuration info.
        config_filename = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s \n"
                           "-a <file containing assembly descriptor (.cad) file names>\n"
                           "-c <deployment configuration file>\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (cad_filename == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Must specify an assembly descriptor using -a flag.\n"),
                      -1);

  if (config_filename == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Require a deployment configuration flag.  Use -c to set it \n"
                         ),
                        -1);
    }

  // Indicates sucessful parsing of the command line
  return 0;
}


int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      parse_args (argc, argv);
      CIAO::Assembly_Spec assembly_spec;

      if (CIAO::XML_Utils::parse_componentassembly (cad_filename,
                                                    &assembly_spec) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Assembly failed : Invalid assembly location\n"));
        }

      CIAO::Static_Assembly_Builder_Visitor builder (assembly_spec.componentfiles_,
                                                     installation_file,
                                                     static_config_info);

      int build_result = assembly_spec.partitioning_.accept (builder
                                                             ACE_ENV_ARG_PARAMETER);

      builder.build_connections (assembly_spec.connections_);

      builder.dump_static_config_info ();
      builder.generate_static_header_file ("Static_Assembly_Config.h");
      builder.generate_static_app_driver ("Static_CCM_App.cpp");
      builder.generate_static_app_mpc ("Static_CCM_App.mpc");
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
