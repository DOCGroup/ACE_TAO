// $Id$

#include "test_i.h"
#include "common.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

void parse_args(int argc, char * argv[]);
void write_ior_to_file(char const * ior);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    CORBA::ORB_var orb = initialize_orb_and_poa(argc, argv);

    parse_args(argc, argv);

    test_i servant (orb.in());
    CORBA::String_var ior =
      servant.create_and_activate_server();

    write_ior_to_file(ior.in());

    orb->run();
  }
  catch(...)
  {
    report_exception();
    return 1;
  }
  return 0;
}

namespace
{
const char *ior_output_file = "test.ior";
}

void
parse_args(int argc, char * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        throw "Usage: server [-o iorfile]\n";
      }
  }
}

void write_ior_to_file(char const * ior)
{
  if (ior_output_file == 0)
  {
    return;
  }

  FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
  if (output_file == 0)
  {
    throw "Cannot open output file to write the IOR";
  }

  ACE_OS::fprintf (output_file, "%s", ior);
  ACE_OS::fclose (output_file);
}
