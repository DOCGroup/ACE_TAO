// $Id$

/// Run with -ORBInifRef CIAODaemon=file://daemon.ior


#include "controller_i.h"
#include "ace/Get_Opt.h"

int
write_IOR(const char* ior)
{
  FILE* ior_output_file_ =
    ACE_OS::fopen ("hello.ior", "w");

  if (ior_output_file_)
    {
      ACE_OS::fprintf (ior_output_file_,
                       "%s",
                       ior);
      ACE_OS::fclose (ior_output_file_);
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CIAO::Daemon_i::controller control_module;

      int res = control_module.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (res != 0)
        return -1;

      res = control_module.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (res != 0)
        return -1;

      control_module.fini ();
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
