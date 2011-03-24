
//=============================================================================
/**
 *  @file    process.cpp
 *
 *  $Id$
 *
 *  This example tests the <ACE_Process>.  For more info, check the
 *  README file in this directory.
 *
 *
 *  @author Tim Harrison <harrison@cs.wustl.edu>.
 */
//=============================================================================


#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"
#include "ace/Process.h"
#include "ace/Log_Msg.h"
#include "ace/Time_Value.h"
#include "ace/SString.h"
#include "ace/Truncate.h"
#include "ace/Tokenizer_T.h"



#if defined (ACE_WIN32)
#define EXEC_NAME ACE_TEXT ("MORE.COM")
const ACE_TCHAR *DATE_PATH = ACE_TEXT ("date.exe");
const ACE_TCHAR *LS_PATH = ACE_TEXT ("ls.exe");
const ACE_TCHAR *SLEEP_PATH = ACE_TEXT ("sleep.exe");
#else
#define EXEC_NAME ACE_TEXT ("less")
const ACE_TCHAR *DATE_PATH = ACE_TEXT ("date");
const ACE_TCHAR *LS_PATH = ACE_TEXT ("ls");
const ACE_TCHAR *SLEEP_PATH = ACE_TEXT ("sleep");
#endif /* ACE_WIN32 */

static const ACE_TCHAR *executable = EXEC_NAME;
static ACE_TCHAR *print_file = 0;
static ACE_TCHAR *environment_string = 0;
static int get_env = 0;
static int run_date = 0;
static int run_ls = 0;
static int run_all = 0;
static int run_setenv = 0;
static int run_tokenizer = 0;
static int run_wait = 0;

// Parse the command-line arguments and set options.
static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("dlx:p:e:gastuw"));
  int c;

  while ((c = get_opt ()) != -1)
    {
    switch (c)
      {
      case 't':
        run_tokenizer = 1;
        break;
      case 's':
        run_setenv = 1;
        break;
      case 'a':
        run_all = 1;
        break;
      case 'd':
        run_date = 1;
        break;
      case 'l':
        run_ls = 1;
        break;
      case 'x':
        executable = get_opt.opt_arg ();
        break;
      case 'p':
        print_file = get_opt.opt_arg ();
        break;
      case 'e':
        environment_string = get_opt.opt_arg ();
        break;
      case 'g':
        get_env = 1;
        break;
      case 'w':
        run_wait = 1;
        break;
      case 'u':
      default:
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Usage:\n")
                           ACE_TEXT ("-d print date\n")
                           ACE_TEXT ("-l run ls\n")
                           ACE_TEXT ("-x <executable=more.com>\n")
                           ACE_TEXT ("-p print <file_name>\n")
                           ACE_TEXT ("-e <env variable message>\n")
                           ACE_TEXT ("-s setenv ACE_PROCESS_ENV and spawn -g\n")
                           ACE_TEXT ("-g get_env ACE_PROCESS_ENV\n")
                           ACE_TEXT ("-t test tokenizer\n")
                           ACE_TEXT ("-w test wait functions\n")
                           ACE_TEXT ("-a run all (d,l,e \"running\")\n")),
                          -1);
      }
    }

  return 0;
}

// This shows how to set handles.
static void
test_more (void)
{
  ACE_HANDLE infile = ACE_OS::open (print_file, O_RDONLY);

  if (infile == ACE_INVALID_HANDLE)
    {
      ACE_ERROR ((LM_DEBUG, ACE_TEXT ("%p\n"), print_file));
      return;
    }

  ACE_Process new_process;
  ACE_Process_Options options;
  options.command_line (executable);
  options.set_handles (infile);

  if (new_process.spawn (options) == -1)
    {
      int error = ACE_OS::last_error ();
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p errno = %d.\n"),
                  ACE_TEXT ("test_more"),
                  error));
    }

  ACE_exitcode status;
  new_process.wait (&status);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Process exit with status %d\n"),
              status));
  ACE_OS::close (infile);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("More succeeded.\n")));
}

// This is a simple usage of ACE_Process.

static void
test_date (void)
{
  ACE_Process_Options options;
  options.command_line (DATE_PATH);

  // Try to create a new process running date.
  ACE_Process new_process;
  if (new_process.spawn (options) == -1)
    {
      int error = ACE_OS::last_error ();
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p errno = %d.\n"),
                  ACE_TEXT ("test_date"),
                  error));
      return;
    }

  ACE_exitcode status;
  new_process.wait (&status);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Process exit with status %d\n"),
              status));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("date succeeded.\n")));
}

static void
test_ls (void)
{
  ACE_Process_Options options;
#if defined (ACE_WIN32) || !defined (ACE_USES_WCHAR)
  options.command_line (ACE_TEXT ("%s -al"), LS_PATH);
#else
  options.command_line (ACE_TEXT ("%ls -al"), LS_PATH);
#endif
  ACE_Process new_process;
  if (new_process.spawn (options) == -1)
    {
      int error = ACE_OS::last_error ();
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p errno = %d.\n"),
                  ACE_TEXT ("test_ls"),
                  error));
    }

  ACE_exitcode status;
  new_process.wait (&status);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Process exit with status %d\n"),
              status));
}

static void
test_wait (void)
{
  ACE_Process_Options options;
#if defined (ACE_WIN32) || !defined (ACE_USES_WCHAR)
  options.command_line (ACE_TEXT ("%s 10"), SLEEP_PATH);
#else
  options.command_line (ACE_TEXT ("%ls 10"), SLEEP_PATH);
#endif
  ACE_Process process1;
  if (process1.spawn (options) == -1)
    {
      int error = ACE_OS::last_error ();
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p errno = %d.\n"),
                  ACE_TEXT ("test_ls"),
                  error));
    }

  int result;
  ACE_exitcode status;

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%T] New process sleeping 10; try wait(2)\n")));
  //FUZZ: enable check_for_lack_ACE_OS

  result = process1.wait (ACE_Time_Value (2), &status);

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%T] wait(2) returns %d(%d)...now try regular wait\n"),
              result,
              status));
  //FUZZ: enable check_for_lack_ACE_OS

  result = process1.wait (&status);

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%T] wait() returns %d(%d)\n"),
              result,
              status));
  //FUZZ: enable check_for_lack_ACE_OS

  ACE_Process process2;
  if (process2.spawn (options) == -1)
    {
      int error = ACE_OS::last_error ();
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p errno = %d.\n"),
                  ACE_TEXT ("test_ls"),
                  error));
    }

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%T] New process sleeping 10; try wait(12)\n"),
              status));
  //FUZZ: enable check_for_lack_ACE_OS

  result = process2.wait (ACE_Time_Value (12), &status);

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%T] wait(12) returns %d(%d)...now try regular wait\n"),
              result,
              status));
  //FUZZ: enable check_for_lack_ACE_OS

  result = process2.wait (&status);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("[%T] wait returns %d(%d)\n"),
              result,
              status));
}

#if defined (ACE_WIN32)
// This is just to test the direct usage of CreateProcess.  I use this
// occasionally as a sanity check when ACE_Process breaks.
static void
win32_test_ls (void)
{
  PROCESS_INFORMATION process_info;
  ACE_TEXT_STARTUPINFO startup_info;
  ACE_OS::memset ((void *) &startup_info,
                  0,
                  sizeof startup_info);
  ACE_OS::memset ((void *) &process_info,
                  0,
                  sizeof process_info);
  startup_info.cb = sizeof startup_info;
  startup_info.dwFlags = STARTF_USESTDHANDLES;

  ACE_HANDLE std_out = ACE_STDOUT;

  if (!::DuplicateHandle (::GetCurrentProcess (),
                          std_out,
                          ::GetCurrentProcess (),
                          &startup_info.hStdOutput,
                          0,
                          TRUE,
                          DUPLICATE_SAME_ACCESS))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p duplicate failed.\n"),
                  ACE_TEXT ("test_ls")));
      return;
    }

  ACE_TCHAR cmd_line[8];
  ACE_OS::strncpy (cmd_line, ACE_TEXT ("-a"), sizeof (cmd_line));
  BOOL fork_result =
    ACE_TEXT_CreateProcess (ACE_TEXT ("c:\\Utils\\bin\\ls.exe"),
                            cmd_line,
                            0, // No process attributes.
                            0, // No thread attributes.
                            TRUE, // Allow handle inheritance.
                            0, // CREATE_NEW_CONSOLE, // Create a new console window.
                            0,
                            0, // Current directory to start in.
                            &startup_info,
                            &process_info);

  ::CloseHandle (startup_info.hStdOutput);

  if (fork_result == 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p CreateProcess failed.\n"),
                ACE_TEXT ("test_ls")));
  else
    {
      ::WaitForSingleObject (process_info.hProcess,
                             INFINITE);
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("ls succeeded.\n")));
    }
}

// This code spawns a new process.  The new process inherits our
// existing environment, plus one more.  This has to be done by hand
// since CreateProcess does not allow us to inherit AND add
// environment variables.

static void
win32_spawn_environment_process (void)
{
  PROCESS_INFORMATION process_info;
  ACE_TEXT_STARTUPINFO startup_info;
  ACE_OS::memset ((void *) &startup_info,
                  0,
                  sizeof startup_info);
  ACE_OS::memset ((void *) &process_info,
                  0,
                  sizeof process_info);
  startup_info.cb = sizeof (startup_info);
  startup_info.dwFlags = STARTF_USESTDHANDLES;

  ACE_HANDLE std_in = ACE_STDIN;
  ACE_HANDLE std_out = ACE_STDOUT;
  ACE_HANDLE std_err = ACE_STDERR;

  if (!::DuplicateHandle (::GetCurrentProcess(),
                          std_out,
                          ::GetCurrentProcess(),
                          &startup_info.hStdOutput,
                          0,
                          TRUE,
                          DUPLICATE_SAME_ACCESS))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p duplicate failed.\n"),
                  ACE_TEXT ("spawn_environment_process")));
      return;
    }

  if (!::DuplicateHandle (::GetCurrentProcess(),
                          std_err,
                          ::GetCurrentProcess(),
                          &startup_info.hStdError,
                          0,
                          TRUE,
                          DUPLICATE_SAME_ACCESS))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p duplicate failed.\n"),
                  ACE_TEXT ("spawn_environment_process")));
      return;
    }

  if (!::DuplicateHandle (::GetCurrentProcess(),
                          std_in,
                          ::GetCurrentProcess(),
                          &startup_info.hStdInput,
                          0,
                          TRUE,
                          DUPLICATE_SAME_ACCESS))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p duplicate failed.\n"),
                  ACE_TEXT ("spawn_environment_process")));
      return;
    }

  // Normally, this would be just GetEnvironmentStrings, but it
  // doesn't follow the same rules as the rest of the Win32 API
  ACE_TCHAR *existing_environment = ACE_OS::getenvstrings ();
  ACE_TCHAR environment[10240];
  ACE_OS::sprintf (environment,
                   ACE_TEXT("ACE_PROCESS_TEST=%s"),
                   environment_string);

  int size = 0;
  while (existing_environment[size] != '\0')
    size +=
      ACE_Utils::truncate_cast<int> (
        ACE_OS::strlen (existing_environment + size) + 1);

  ACE_OS::memcpy (environment + (ACE_OS::strlen (environment) + 1),
                  existing_environment,
                  size);

  ACE_TEXT_FreeEnvironmentStrings (existing_environment);

  ACE_TCHAR cmd_line[16];
  ACE_OS::strncpy (cmd_line, ACE_TEXT ("process -g"), sizeof (cmd_line));
  BOOL fork_result =
    ACE_TEXT_CreateProcess (ACE_TEXT ("d:\\harrison\\ACE_wrappers\\examples\\OS\\Process\\process.exe"),
                            cmd_line,
                            0, // No process attributes.
                            0, // No thread attributes.
                            TRUE, // Allow handle inheritance.
                            0, // CREATE_NEW_CONSOLE, // Create a new console window.
                            environment, // Environment.
                            //"d:\\harrison\\ACE_wrappers\\examples\\OS\\Process\\",
                            0,
                            &startup_info,
                            &process_info);

  ::CloseHandle (startup_info.hStdOutput);
  ::CloseHandle (startup_info.hStdError);

  if (fork_result == 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p.\n"),
                ACE_TEXT ("spawn_environment_process")));
  else
    {
      ::WaitForSingleObject (process_info.hProcess,
                             INFINITE);
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("spawn_environment_process succeeded.\n")));
    }
}
#endif

static void
test_setenv (const ACE_TCHAR *argv0)
{
  ACE_Process_Options options;
  //  options.setenv ("ACE_PROCESS_TEST", "here's a really large number: %u", 0 - 1);
  options.setenv (ACE_TEXT ("ACE_PROCESS_TEST= here's a large number %u"),
                  0 - 1);
  options.setenv (ACE_TEXT ("ACE_PROCESS_TEST2"), ACE_TEXT ("ophilli"));
#if defined (ACE_WIN32) || !defined (ACE_USES_WCHAR)
  options.command_line ("%s -g", argv0);
#else
  options.command_line ("%ls -g", argv0);
#endif
  ACE_Process process;
  if (process.spawn (options) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p.\n"),
                  ACE_TEXT ("test_setenv")));
      return;
    }

  ACE_exitcode status;
  process.wait (&status);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Process exit with status %d\n"),
              status));
}

// Tests the ACE_Tokenizer.
static void
tokenize (ACE_TCHAR *buffer)
{
  // This tokenizer will replace all spaces with end-of-string
  // characters and will preserve text between "" and '' pairs.
  ACE_Tokenizer parser (buffer);
  parser.delimiter_replace (' ', '\0');
  parser.preserve_designators ('\"', '\"'); // "  This quote is for emacs
  parser.preserve_designators ('\'', '\'');

  for (const ACE_TCHAR *temp; ;)
    {
      temp = parser.next ();
      if (temp == 0)
        break;
      ACE_DEBUG ((LM_DEBUG, temp));
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (ACE_LOG_MSG->open (argv[0]) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("cannot open logger!!!\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("starting...\n")));

  if (::parse_args (argc, argv) == -1)
    return -1;

  if (run_all)
    {
#if defined (ACE_WIN32) || !defined (ACE_USES_WCHAR)
      const ACE_TCHAR *cmdline = ACE_TEXT ("%s -d -l -s -w");
#else
      const ACE_TCHAR *cmdline = ACE_TEXT ("%ls -d -l -s -w");
#endif
      ACE_Process_Options options;
      options.command_line (cmdline, argv[0]);
      ACE_Process process;
      if (process.spawn (options) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p.\n"),
                           ACE_TEXT ("main")),
                          -1);
      ACE_exitcode status;
      process.wait (&status);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Process exit with status %d\n"),
                  status));
    }

  if (run_date)
    ::test_date ();

  if (run_setenv)
    ::test_setenv (argv[0]);

  if (get_env)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("checking ACE_PROCESS_TEST\n")));
      char *value = ACE_OS::getenv ("ACE_PROCESS_TEST");
      char *value2 = ACE_OS::getenv ("ACE_PROCESS_TEST2");
      ACE_DEBUG ((LM_DEBUG,
                  "ACE_PROCESS_TEST = %C.\n"
                  "ACE_PROCESS_TEST2 = %C.\n",
                  value == 0 ? "no value" : value,
                  value2 == 0 ? "no value" : value2));
    }

  if (run_ls)
    ::test_ls ();

  if (run_wait)
    ::test_wait ();

#if defined (ACE_WIN32)
  ACE_UNUSED_ARG (&win32_test_ls);

  if (environment_string != 0)
    win32_spawn_environment_process ();
#endif /* ACE_WIN32 */

  if (print_file != 0)
    test_more ();

  ACE_TCHAR buf1[30];
  ACE_TCHAR buf2[30];
  ACE_OS::strcpy(buf1, ACE_TEXT (" -f hi honey -g \"I\'m home\""));
  ACE_OS::strcpy(buf2, ACE_TEXT ("\"token 1\"\'token 2\'\"token 3\" "));

  if (run_tokenizer)
    {
      tokenize (buf1);
      tokenize (buf2);
    }

  return 0;
}
