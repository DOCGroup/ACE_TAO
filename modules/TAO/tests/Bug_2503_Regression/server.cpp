// $Id$

#include "test_i.h"
#include "common.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/High_Res_Timer.h"

void parse_args(int argc, ACE_TCHAR* argv[]);
void write_ior_to_file(char const * ior);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  // Fetching the high res timer's global scale factor ensures that it
  // is calibrated (if necessary on this platform) at the beginning of
  // the test.  While the timer would otherwise be calibrated on first
  // use, this introduces delay in the middle of the test's execution.
  // This leads to failures due to timing assumptions (timeouts, etc.)
  // within the test itself.
  (void) ACE_High_Res_Timer::global_scale_factor();

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
const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");
}

void
parse_args(int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
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
