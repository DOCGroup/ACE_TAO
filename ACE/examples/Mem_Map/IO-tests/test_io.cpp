// Test program for different methods of copying files.

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/ACE.h"
#include "ace/Profile_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Signal.h"
#include "ace/Log_Msg.h"
#include "IO_Test.h"
#include <array>
#include <memory>

#if !defined(ACE_WIN32)

static ACE_TCHAR const default_input_filename[] = ACE_TEXT ("/usr/dict/words");
static ACE_TCHAR const default_output_filename[] = ACE_TEXT ("/tmp/foo");

struct Options
{
  ACE_TCHAR const *program_name {};
  ACE_TCHAR const *input_filename { default_input_filename };
  ACE_TCHAR const *output_filename { default_output_filename };
  bool remove_output { true };
  int iteration_count { 100 };
};

static Options options {};

// Profiler used to keep track of file I/O time.
static ACE_Profile_Timer profile_timer;

struct File_Deleter
{
  void operator() (FILE *file) const
  {
    if (file != nullptr)
      ACE_OS::fclose (file);
  }
};

using File_Ptr = std::unique_ptr<FILE, File_Deleter>;

// Explain usage and exit.

static void
print_usage_and_die ()
{
  ACE_OS::fprintf (stderr, "usage: %s"
                   " [-i input_file] [-o output_file] [-n iteration_count] [-r]\n",
                   ACE_TEXT_ALWAYS_CHAR (options.program_name));
  ACE_OS::exit (1);
}

// Clean up the output file on exit from a signal.

extern "C" void
cleanup (int)
{
  if (options.remove_output)
    ACE_OS::unlink (options.output_filename);
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
      options.input_filename = get_opt.opt_arg ();
      break;
    case 'n':
      options.iteration_count = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 'o':
      options.output_filename = get_opt.opt_arg ();
      break;
    case 'r':
      options.remove_output = false;
      break;
    default:
      print_usage_and_die ();
      break;
  }
}

template <typename TEST>
static int
create_test (std::unique_ptr<IO_Test> &test,
             char const *name)
{
  TEST *raw_test {};
  ACE_NEW_RETURN (raw_test, TEST (name, profile_timer), -1);
  test.reset (raw_test);
  return 0;
}

static int
run_tests (int iterations, FILE *input_fp, FILE *output_fp)
{
  std::array<std::unique_ptr<IO_Test>, 6> test_vector {};
  if (create_test<Stdio_Test> (test_vector[0], "Stdio_Test") == -1
      || create_test<Block_Fread_Fwrite_Test> (test_vector[1],
                                               "Block_Fread_Fwrite_Test") == -1
      || create_test<Block_Read_Write_Test> (test_vector[2],
                                             "Block_Read_Write_Test") == -1
      || create_test<Mmap1_Test> (test_vector[3], "Mmap1_Test") == -1
      || create_test<Mmap2_Test> (test_vector[4], "Mmap2_Test") == -1
      || create_test<Slow_Read_Write_Test> (test_vector[5],
                                            "Slow_Read_Write_Test") == -1)
    return -1;

  for (std::unique_ptr<IO_Test> const &test : test_vector)
    {
      ACE_HANDLE const hfile = fileno (output_fp);
      if (ACE_OS::ftruncate (hfile, 0) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%s\n"),
                           ACE_TEXT ("ftruncate")),
                          -1);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("--------------------\n")
                  ACE_TEXT ("starting %C for %d iterations(s):\n"),
                  test->name (),
                  iterations));

      test->run_test (iterations,
                      input_fp,
                      output_fp);

      ACE_Profile_Timer::ACE_Elapsed_Time et;
      profile_timer.elapsed_time (et);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("wallclock time = %f, user time = %f, system time = %f\n"),
                  et.real_time,
                  et.user_time,
                  et.system_time));

    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("--------------------\n")));
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  options.program_name = ACE::basename (argv[0],
                                        ACE_DIRECTORY_SEPARATOR_CHAR);
  parse_args (argc, argv);

  ACE_Sig_Action sa (&cleanup, SIGINT);
  ACE_UNUSED_ARG (sa);

  File_Ptr input_fp (ACE_OS::fopen (options.input_filename, ACE_TEXT ("r")));
  File_Ptr output_fp (ACE_OS::fopen (options.output_filename, ACE_TEXT ("w+")));

  if (input_fp == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       options.input_filename),
                      -1);

  if (output_fp == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       options.output_filename),
                      -1);

  ACE_OS::unlink (options.output_filename);

  if (run_tests (options.iteration_count,
                 input_fp.get (),
                 output_fp.get ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("run_tests")),
                      -1);

  int const input_close_result = ACE_OS::fclose (input_fp.release ());
  int const output_close_result = ACE_OS::fclose (output_fp.release ());
  if (input_close_result == -1 || output_close_result == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("fclose")),
                      -1);
  cleanup (0);
  return 0;
}
#else
int ACE_TMAIN (int, ACE_TCHAR*[]) {
  // not supported on win32
  return 0;
}
#endif
