// $Id$

// Test program for different methods of copying files.

#include "ace/OS.h"
#include "ace/Profile_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Signal.h"
#include "IO_Test.h"

// Name of program.
static const char *program_name;

// Name of default input file.
static char *input_filename = "/usr/dict/words";

// Name of default output file.
static char *output_filename = "/tmp/foo";

// Check if removing output file upon completion...
static int remove_output = 1;

// Count of the number of iterations to run the tests.
static int iteration_count = 100;

// Profiler used to keep track of file I/O time.
static ACE_Profile_Timer tm;

// Explain usage and exit.

static void 
print_usage_and_die (void)
{
  cerr << "usage: " 
       << program_name 
       << " [-i input_file] [-o output_file] [-n iteration_count] [-r]\n";
  ACE_OS::exit (1);
}

// Clean up the output file on exit from a signal.

extern "C" void
cleanup (int = 0)
{
  if (remove_output)
    ACE_OS::unlink (output_filename);
  ACE_OS::exit (0);
}

// Parse the command-line arguments and set options.

static void
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "i:n:o:r");

  for (int c; ((c = get_opt ()) != -1); )
    switch (c)
    {
    case 'i':
      input_filename = get_opt.optarg;
      break;
    case 'n':
      iteration_count = ACE_OS::atoi (get_opt.optarg);
      break;
    case 'o':
      output_filename = get_opt.optarg;
      break;
    case 'r':
      remove_output = 0;
      break;
    default:
      print_usage_and_die ();
      break;
  }
}

// Vector of pointers to derived classes that inherit from IO_Test
// base class.

static IO_Test *test_vector[100];

static void
run_tests (int iterations, FILE *input_fp, FILE *output_fp)
{
  // If HP/UX didn't suck so badly we could initialize in the global
  // scope...
  int i = 0;

  ACE_NEW (test_vector[i++], Stdio_Test ("Stdio_Test", tm));
  ACE_NEW (test_vector[i++], Block_Fread_Fwrite_Test ("Block_Fread_Fwrite_Test", tm));
  ACE_NEW (test_vector[i++], Block_Read_Write_Test ("Block_Read_Write_Test", tm));
  ACE_NEW (test_vector[i++], Mmap1_Test ("Mmap1_Test", tm));
  ACE_NEW (test_vector[i++], Mmap2_Test ("Mmap2_Test", tm));
  ACE_NEW (test_vector[i++], Slow_Read_Write_Test ("Slow_Read_Write_Test", tm));

  test_vector[i] = (IO_Test *) 0;

  for (i = 0; test_vector[i] != 0; i++)
    {
      if (ACE_OS::ftruncate (fileno (output_fp), 0) == -1)
	ACE_ERROR ((LM_ERROR, "%s\n", "ftruncate"));

      cerr << "--------------------\n" 
	   << "starting " << test_vector[i]->name () << " for " << iterations
	   << " iteration(s):\n";

      test_vector[i]->run_test (iterations, input_fp, output_fp);

      ACE_Profile_Timer::ACE_Elapsed_Time et;
      tm.elapsed_time (et);

      cerr << "wallclock time = " << et.real_time
           << ", user time = " << et.user_time
	   << ", system time = " << et.system_time << endl;

      delete test_vector[i];
    }

  cerr << "--------------------\n";
}

int
main (int argc, char *argv[])
{
#if defined (ACE_WIN32)
  char delim = '\\';
#else
  char delim = '/';
#endif /* ACE_WIN32 */
  program_name = ACE::basename (argv[0], delim);
  parse_args (argc, argv);

  ACE_Sig_Action sa ((ACE_SignalHandler) cleanup, SIGINT);
  ACE_UNUSED_ARG (sa);

  FILE *input_fp = ACE_OS::fopen (input_filename, "r");
  FILE *output_fp = ACE_OS::fopen (output_filename, "w+");

  if (input_fp == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%s\n", "input_filename"), -1);

  if (output_fp == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%s\n", "output_filename"), -1);

  ACE_OS::unlink (output_filename);

  run_tests (iteration_count, input_fp, output_fp);

  if (ACE_OS::fclose (input_fp) == -1 
      || ACE_OS::fclose (output_fp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%s\n", "fclose"), -1);

  cleanup ();
  return 0;
}
