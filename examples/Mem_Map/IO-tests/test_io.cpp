// $Id$

// Test program for different methods of copying files.

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/ACE.h"
#include "ace/Profile_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Signal.h"
#include "ace/Log_Msg.h"
#include "IO_Test.h"



#if !defined(_WIN32)

// Name of program.
static const ACE_TCHAR *program_name;

// Name of default input file.
static const ACE_TCHAR *input_filename = ACE_TEXT ("/usr/dict/words");

// Name of default output file.
static const ACE_TCHAR *output_filename = ACE_TEXT ("/tmp/foo");

// Check if removing output file upon completion...
static int remove_output = 1;

// Count of the number of iterations to run the tests.
static int iteration_count = 100;

// Profiler used to keep track of file I/O time.
static ACE_Profile_Timer profile_timer;

// Explain usage and exit.

static void
print_usage_and_die (void)
{
  ACE_OS::fprintf (stderr, "usage: %s"
                   " [-i input_file] [-o output_file] [-n iteration_count] [-r]\n",
                   ACE_TEXT_ALWAYS_CHAR (program_name));
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
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("i:n:o:r"));

  for (int c; ((c = get_opt ()) != -1); )
    switch (c)
    {
    case 'i':
      input_filename = get_opt.opt_arg ();
      break;
    case 'n':
      iteration_count = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 'o':
      output_filename = get_opt.opt_arg ();
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

static int
run_tests (int iterations, FILE *input_fp, FILE *output_fp)
{
  int i = 0;

  ACE_NEW_RETURN (test_vector[i],
                  Stdio_Test ("Stdio_Test",
                              profile_timer),
                  -1);
  i++;
  ACE_NEW_RETURN (test_vector[i],
                  Block_Fread_Fwrite_Test ("Block_Fread_Fwrite_Test",
                                           profile_timer),
                  -1);
  i++;
  ACE_NEW_RETURN (test_vector[i],
                  Block_Read_Write_Test ("Block_Read_Write_Test",
                                         profile_timer),
                  -1);
  i++;
  ACE_NEW_RETURN (test_vector[i],
                  Mmap1_Test ("Mmap1_Test",
                              profile_timer),
                  -1);
  i++;
  ACE_NEW_RETURN (test_vector[i],
                  Mmap2_Test ("Mmap2_Test",
                              profile_timer),
                  -1);
  i++;
  ACE_NEW_RETURN (test_vector[i],
                  Slow_Read_Write_Test ("Slow_Read_Write_Test",
                                        profile_timer),
                  -1);
  i++;

  test_vector[i] = (IO_Test *) 0;

  for (i = 0; test_vector[i] != 0; i++)
    {
      ACE_HANDLE hfile = fileno (output_fp);
      if (ACE_OS::ftruncate (hfile, 0) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%s\n"),
                           ACE_TEXT ("ftruncate")),
                          -1);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("--------------------\n")
                  ACE_TEXT ("starting %C for %d iterations(s):\n"),
                  test_vector[i]->name (),
                  iterations));

      test_vector[i]->run_test (iterations,
                                input_fp,
                                output_fp);

      ACE_Profile_Timer::ACE_Elapsed_Time et;
      profile_timer.elapsed_time (et);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("wallclock time = %f, user time = %f, system time = %f\n"),
                  et.real_time,
                  et.user_time,
                  et.system_time));

      delete test_vector[i];
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("--------------------\n")));
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  program_name = ACE::basename (argv[0],
                                ACE_DIRECTORY_SEPARATOR_CHAR);
  parse_args (argc, argv);

  ACE_Sig_Action sa ((ACE_SignalHandler) cleanup, SIGINT);
  ACE_UNUSED_ARG (sa);

  FILE *input_fp =
    ACE_OS::fopen (input_filename, ACE_TEXT ("r"));
  FILE *output_fp =
    ACE_OS::fopen (output_filename, ACE_TEXT ("w+"));

  if (input_fp == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       input_filename),
                      -1);

  if (output_fp == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       output_filename),
                      -1);

  ACE_OS::unlink (output_filename);

  if (run_tests (iteration_count,
                 input_fp,
                 output_fp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("run_tests")),
                      -1);

  if (ACE_OS::fclose (input_fp) == -1
      || ACE_OS::fclose (output_fp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("fclose")),
                      -1);
  cleanup ();
  return 0;
}
#else
int ACE_TMAIN (int, ACE_TCHAR*[]) {
  // not supported on win32
  return 0;
}
#endif
