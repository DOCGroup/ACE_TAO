// $Id$

//==========================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Logging_Strategy_Test.cpp
//
// = DESCRIPTION
//     This program tests the <ACE_Logging_Strategy> class in various ways
//     and also illustrates many of the features of the <ACE_Log_Msg>.
//     The test works as follows:
//     -Load the inserted arguments;
//     -Remove existent log_files with the file_name specified by the user;
//     -Generate 1000 messages to create the DEBUG statements to be stored in
//      the files;
//     -Counts the created log_files and if it was specified a maximum number
//      of log_files, compare and verify if they are the same.
//     -Verify the order of the files with the order argument.
//
// = AUTHOR
//    Orlando Ribeiro <oribeiro@inescporto.pt>
//
//==========================================================================

#include "ace/OS.h"
#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"
#include <ace/Get_Opt.h>
#include "test_config.h"

ACE_RCSID(tests, Logging_Strategy_Test, "Logging_Strategy_Test.cpp,v 1.00 2001/02/19 05:17:39 oribeiro Exp")

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Considering UNIX OS to be default. On Win32 platforms, the symbols
// are got form the .exe as one cant have .exe and .dll for the same
// .cpp. Also, on Win32 platforms one cant use the .obj to obtain
// symbols dynamically at runtime.

#if defined (ACE_WIN32)
#  define OBJ_SUFFIX ACE_TEXT (".exe")
#  define OBJ_PREFIX ACE_TEXT ("")
#else
#  define OBJ_SUFFIX ACE_DLL_SUFFIX
#  define OBJ_PREFIX "./" ACE_DLL_PREFIX
#endif /*ACE_WIN32*/

ACE_TCHAR const *
cdecl_decoration (ACE_TCHAR const *func_name)
{
#if defined (__BORLANDC__)
  static ACE_TCHAR decorated_func_name[10*1024];
  ACE_OS::sprintf (decorated_func_name,
                   ACE_TEXT ("_%s"),
                   func_name);
  return decorated_func_name;
#else
  return func_name;
#endif /* __BORLANDC__ */
}

// Global variables.
static ACE_TCHAR *file_name = 0;
static int max_size_files = 0;
static int max_num_files = 0;
static int interval_time = 0;
static int order_state = 0;
static int num_files = 0;

void
run_reactor (void *)
{
  ACE_Reactor::instance ()->owner (ACE_Thread_Manager::instance ()->thr_self ());
  ACE_Reactor::instance ()->run_event_loop ();
}

// Initiate the cycle of messages.

static
void print_till_death (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n-> start generating messages... \n"));

  for (int i = 0; i < 1000; i++)
    {
      if (i % 50 == 0)
        ACE_OS::sleep (1);

      if (i == 0)
        ACE_DEBUG ((LM_DEBUG,
                    " (%t) (%D) message\n"));
      else
        ACE_DEBUG ((LM_DEBUG,
                    " (%t) %d message\n",
                    i));
    }
  ACE_Reactor::instance ()->end_event_loop ();

  ACE_DEBUG ((LM_DEBUG,
              "-< generating messages finished \n\n"));
}

// count the generated files
void
count_files (void)
{
  int i = 0;
  int error = 0;
  FILE *stream;
  ACE_TCHAR backup_ct[MAXPATHLEN+1];
  ACE_DEBUG ((LM_DEBUG,
              "-> start counting...\n"));

  do
    {
      if (i == 0)
        ACE_OS::sprintf (backup_ct,
                         "%s",
                         file_name);
      else
        ACE_OS::sprintf (backup_ct,
                         "%s.%d",
                         file_name,
                         i);

      stream = ACE_OS::fopen (backup_ct, "r");
      if (stream == 0)
        error = 1;
      else
        {
          i++;
          ACE_OS::fclose (stream);
        }

    }
  while (error != 1);

  num_files = i;

  if (max_num_files !=0)
    {
      if (max_num_files != num_files)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Creating files...Failed! Imput value=%d, Checked value=%d"),
                    max_num_files,
                    num_files));
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("      Creating files...OK! Imput value=%d, Checked value=%d"),
                    max_num_files,
                    num_files));
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("      The number of files generated is: %d"),
                num_files));

  ACE_DEBUG ((LM_DEBUG,
              "\n-< counting finished... \n"));
}

// get the file statistics
static int
get_statistic (char *f_name)
{
  ACE_stat buf;
  int result;

  // Get data associated with "file_name":
  result = ACE_OS::stat (f_name, &buf);

  // Check if statistics are valid:
  if (result != 0)
    ACE_OS::perror ("\nProblem getting information");
  else
    {
      // Output some of the statistics:
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("      File name     : %s\n"),
                  f_name));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("      File size (B): %d\n"),
                  buf.st_size));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("      Time modified : %s\n"),
                  ACE_OS::ctime (&buf.st_mtime)));
    }
  return buf.st_mtime;
}

// analyse the file order
static void
order (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n-> start testing order... \n")));

  if (num_files <= 2)
    {
      if (num_files == 1)
        get_statistic (file_name);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("      Ordering...OK! - Only %d file (s) was (were) generated"),
                  num_files));
    }
  else
    {
      int tm_bk_1, tm_bk_2;
      ACE_TCHAR backup_1[MAXPATHLEN+1];
      ACE_TCHAR backup_2[MAXPATHLEN+1];
      ACE_OS::sprintf (backup_1,
                       ACE_TEXT ("%s.%d"),
                       file_name,
                       1);
      ACE_OS::sprintf (backup_2,
                       ACE_TEXT ("%s.%d"),
                       file_name,
                       num_files - 1);

      tm_bk_1 = get_statistic (backup_1);
      tm_bk_2 = get_statistic (backup_2);

      if ((tm_bk_1 > tm_bk_2) && !order_state)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("      %s (newest) ; %s (oldest)\n"),
                      backup_1,
                      backup_2));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("      Ordering...OK!")));
        }
      else
        {
          if ((tm_bk_1 < tm_bk_2) && order_state)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("      %s (newest) ; %s (oldest)\n"),
                          backup_2,
                          backup_1));
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("      Ordering...OK!")));
            }
          else
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("      Ordering...FAILED! - The files are disorderly")));
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n-< testing order finished...\n\n")));
}

// remove log_files
static void
remove_files (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n-> removing existent files...\n")));

  int error = 0;
  int test;
  int i = 0;
  ACE_TCHAR backup[MAXPATHLEN+1];

  do
    {
      i++;
      ACE_OS::sprintf (backup,
                       ACE_TEXT ("%s.%d"),
                       file_name,
                       i);
      test = ACE_OS::unlink (backup);
      if (test != 0)
        error = 1;

    }
  while (error != 1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("-< removing existent files finished... \n\n")));
}

static int
parse_args (int argc, char *argv[])
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Specifications:\n")));
  ACE_Get_Opt get_opt (argc, argv, "s:i:m:f:n:o");
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 's':
          file_name = get_opt.optarg;
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("File name: %s\n"),
                      file_name));
          break;
        case 'i':
          interval_time = atoi (get_opt.optarg);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Interval time (s): %d\n"),
                      interval_time));
          break;
        case 'm':
          max_size_files = atoi (get_opt.optarg);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Maximum size (KB): %d\n"),
                      max_size_files));
          break;
        case 'f':
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Modes: %s\n"),
                      get_opt.optarg));
          break;
        case 'n':
          max_num_files = atoi (get_opt.optarg);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Maximum files number: %d\n"),
                      max_num_files));
          break;
        case 'o':
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Ordering files activated\n")));
          order_state = 1;
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: [-s]<file_name>")
                             ACE_TEXT ("[-i]<sample_interval> [-m]<max_size> [-f]<msg_flags> [-n]<num_files> [-o]\n")
                             ACE_TEXT ("\t-s: Specify the name of the log files.\n")
                             ACE_TEXT ("\t-i: Define the sample interval in secs.\n")
                             ACE_TEXT ("\t-m: Define the max size for the log_files in KB.\n")
                             ACE_TEXT ("\t-f: Indicates the Log_Msg flags.\n")
                             ACE_TEXT ("\t-n: Define the maximum number of log_files.\n")
                             ACE_TEXT ("\t-o: If activated puts the log_files ordered.\n")),
                            -1);
          /* NOTREACHED */
          break;
        }
    }

  return 0;
}

int main (int argc, char *argv [])
{
  ACE_START_TEST (ACE_TEXT ("Logging_Strategy_Test"));

  // Protection against this test being run on platforms not supporting Dlls.
#if defined (ACE_WIN32) || defined (ACE_HAS_SVR4_DYNAMIC_LINKING) || \
  defined (__hpux)

  // Implement the dynamic entries via main arguments
  ACE_LOG_MSG->open (argv[0]);

  ACE_TCHAR *l_argv[4];

  if (argc > 1)
    {
      if (parse_args (argc, argv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Invalid command-line parameters.\n"),
                          1);
    }
  else
    {
      l_argv[0] = argv[0];
      l_argv[1] = (char *) ACE_TEXT ("-sfoo");
      l_argv[2] = (char *) ACE_TEXT ("-o");
      l_argv[3] = 0;

      if (parse_args (3, l_argv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Invalid command-line parameters.\n"),
                          1);
      argv = l_argv;
      argc = 3;
    }

  ACE_TCHAR arg_str[250];
  sprintf (arg_str,
           "dynamic Logger Service_Object *ACE:_make_ACE_Logging_Strategy() \"");

  for (int i = 1; i < argc; i++)
    {
      ACE_OS_String::strcat (arg_str, argv[i]);
      ACE_OS_String::strcat (arg_str, " ");
    }

  ACE_OS_String::strcat (arg_str, "\"");

  if (ACE_Service_Config::process_directive (arg_str) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error opening _make_ACE_Log_Strategy object.\n"),
                      1);

  // launch a new Thread
  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (run_reactor)) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Spawning Reactor.\n"),
                      1);

  // Remove the existent files
  remove_files ();

  // Function to print the message
  print_till_death ();

  // Counts the generated files
  count_files ();

  // Get the file order
  order ();

  if (ACE_Reactor::instance ()->end_event_loop ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error ending reactor.\n"),
                      1);
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("DLLs not supported on this platform\n")));

  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_UNUSED_ARG (print_till_death);
  ACE_UNUSED_ARG (order);
  ACE_UNUSED_ARG (remove_files);
  ACE_UNUSED_ARG (parse_args);
#endif /* ACE_WIN32 || ACE_HAS_SVR4_DYNAMIC_LINKING || __hpux */

  ACE_END_TEST;
  return 0;
}
