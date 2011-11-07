
//=============================================================================
/**
 *  @file    Logging_Strategy_Test.cpp
 *
 *  $Id$
 *
 *   This program tests the <ACE_Logging_Strategy> class in various
 *   ways and also illustrates many of the features of the
 *   <ACE_Log_Msg>.  The test works as follows:
 *   -Load the inserted arguments;
 *   -Remove existent log_files with the file_name specified by the
 *    user;
 *   -Generate 1000 messages to create the DEBUG statements to be
 *    stored in the files;
 *   -Counts the created log_files and if it was specified a maximum
 *    number of log_files, compare and verify if they are the same.
 *   -Verify the order of the files with the order argument.
 *
 *   When Dlls are used, we utilize the dynamic service
 *   configuration mechanism to activate the logging strategy. This
 *   is not a must though, and you may activate the logging strategy
 *   as described in the non-DLL section below under DLL
 *   environments as well.
 *
 *
 *  @author Orlando Ribeiro <oribeiro@inescporto.pt>
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/Auto_Ptr.h"
#include "ace/Service_Config.h"
#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"

#if defined (ACE_AS_STATIC_LIBS) || \
  (!defined (ACE_WIN32) && !defined (ACE_HAS_SVR4_DYNAMIC_LINKING) && \
   !defined (__hpux))
#include "ace/Logging_Strategy.h"
#endif

#include "ace/Auto_Ptr.cpp"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_time.h"

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
static bool order_state = false;
static int num_files = 0;
static bool wipeout_logfile = false;

// This adapter function runs the Reactor's event loop in a separate
// thread of control.

static void *
run_reactor (void *)
{
  ACE_Reactor::instance ()->owner
    (ACE_Thread_Manager::instance ()->thr_self ());
  ACE_Reactor::instance ()->run_reactor_event_loop ();
  return 0;
}

// Initiate the cycle of messages.

static
void print_till_death (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n-> start generating messages...\n"));

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

  if (ACE_Reactor::instance ()->end_reactor_event_loop () == -1)
    ACE_ERROR ((LM_ERROR,
                "Error ending reactor.\n"));

  ACE_DEBUG ((LM_DEBUG,
              "-< generating messages finished \n\n"));
}

// Count the generated files.

static void
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
                         ACE_TEXT ("%s"),
                         file_name);
      else
        ACE_OS::sprintf (backup_ct,
                         ACE_TEXT ("%s.%d"),
                         file_name,
                         i);

      stream = ACE_OS::fopen (backup_ct, ACE_TEXT ("r"));
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
                    ACE_TEXT ("Creating files...Failed!")
                    ACE_TEXT (" Input value=%d, Checked value=%d"),
                    max_num_files,
                    num_files));
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("      Creating files...OK!")
                    ACE_TEXT (" Input value=%d, Checked value=%d"),
                    max_num_files,
                    num_files));
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("      The number of files generated is: %d"),
                num_files));

  ACE_DEBUG ((LM_DEBUG,
              "\n-< counting finished...\n"));
}

// get the file statistics

static time_t
get_statistics (ACE_TCHAR *f_name)
{
  ACE_stat buf;

  // Get data associated with "file_name":
  int result = ACE_OS::stat (f_name, &buf);

  // Check if statistics are valid:
  if (result != 0)
    ACE_OS::perror (ACE_TEXT ("\nProblem getting information"));
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
              ACE_TEXT ("\n-> start testing order...\n")));

  if (num_files <= 2)
    {
      if (num_files == 1)
        get_statistics (file_name);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("      Ordering...OK! - ")
                  ACE_TEXT (" Only %d file (s) was (were) generated"),
                  num_files));
    }
  else
    {
      time_t tm_bk_1, tm_bk_2;
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

      tm_bk_1 = get_statistics (backup_1);
      tm_bk_2 = get_statistics (backup_2);

      if (tm_bk_1 > tm_bk_2 && !order_state)
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
          if (tm_bk_1 < tm_bk_2 && order_state)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("      %s (newest);")
                          ACE_TEXT ("%s (oldest)\n"),
                          backup_2,
                          backup_1));
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("      Ordering...OK!")));
            }
          else
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("      Ordering...FAILED!")
                        ACE_TEXT ("- The files are disorderly")));
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
              ACE_TEXT ("-> removing existent files...\n")));

  int error = 0;
  int i = 0;

  do
    {
      ++i;
      ACE_TCHAR backup[MAXPATHLEN+1];
      ACE_OS::sprintf (backup,
                       ACE_TEXT ("%s.%d"),
                       file_name,
                       i);
      if (ACE_OS::unlink (backup) != 0)
        error = 1;
    }
  while (error != 1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("-< removing existing files...\n\n")));
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Specifications:\n")));
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("s:i:m:f:N:ow"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 's':
          file_name = get_opt.opt_arg ();
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("File name: %s\n"),
                      file_name));
          break;
        case 'i':
          interval_time = ACE_OS::atoi (get_opt.opt_arg ());

          //FUZZ: disable check_for_lack_ACE_OS
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Interval time (s): %d\n"),
                      interval_time));
          //FUZZ: enable check_for_lack_ACE_OS
          break;
        case 'm':
          max_size_files = ACE_OS::atoi (get_opt.opt_arg ());
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Maximum size (KB): %d\n"),
                      max_size_files));
          break;
        case 'f':
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Modes: %s\n"),
                      get_opt.opt_arg ()));
          break;
        case 'N':
          max_num_files = ACE_OS::atoi (get_opt.opt_arg ());
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Maximum files number: %d\n"),
                      max_num_files));
          break;
        case 'o':
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Ordering files activated\n")));
          order_state = true;
          break;
        case 'w':
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Wipeout logfile activated\n")));
          wipeout_logfile = true;
          break;
        default:
          ACE_ERROR_RETURN
            ((LM_ERROR,
              ACE_TEXT ("usage: [-s]<file_name>")
              ACE_TEXT ("[-i]<sample_interval> ")
              ACE_TEXT ("[-m]<max_size> [-f]<msg_flags> ")
              ACE_TEXT ("[-n]<num_files> [-o]\n")
              ACE_TEXT ("\t-s: Specify the name of the log files.\n")
              ACE_TEXT ("\t-i: Define the sample interval in secs.\n")
              ACE_TEXT ("\t-m: Define the max size for the log_files in KB.\n")
              ACE_TEXT ("\t-f: Indicates the Log_Msg flags.\n")
              ACE_TEXT ("\t-N: Define the maximum number of log_files.\n")
              ACE_TEXT ("\t-o: If activated puts the log_files ordered.\n"),
              ACE_TEXT ("\t-w: If activated cause the logfile to be wiped out,")
              ACE_TEXT (" both on startup and on reconfigure.\n")),
             -1);
          /* NOTREACHED */
          break;
        }
    }

  ACE_UNUSED_ARG (wipeout_logfile);
  return 0;
}

int run_main (int argc, ACE_TCHAR *argv [])
{
  ACE_START_TEST (ACE_TEXT ("Logging_Strategy_Test"));

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
      l_argv[0] = (ACE_TCHAR *)ACE_TEXT ("Logging_Strategy_Test");
      l_argv[1] =
        (ACE_TCHAR *) ACE_TEXT ("-s")
                      ACE_DEFAULT_TEST_DIR
                      ACE_TEXT ("log/Logging_Strategy_Test")
                      ACE_LOG_FILE_EXT_NAME;
      l_argv[2] = (ACE_TCHAR *) ACE_TEXT ("-o");
      l_argv[3] = 0;

      if (parse_args (3, l_argv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Invalid command-line parameters.\n"),
                          1);
      argv = l_argv;
      argc = 3;
    }

  // Remove existing files.
  remove_files ();

  // This is necessary only if the provided logfile name is the same
  // as the default name.  If so, nothing will be written as the
  // previous ofstream is closed only at the end (ACE_END_TEST)
  ACE_CLOSE_TEST_LOG;

  // When Dlls are used, we utilize the dynamic service configuration
  // mechanism to activate the logging strategy. This is not a must
  // though, and you may activate the logging strategy as described in
  // the non-DLL section below under DLL environments as well.

#if !defined (ACE_AS_STATIC_LIBS) && \
  (defined (ACE_WIN32) || defined (ACE_HAS_SVR4_DYNAMIC_LINKING) || \
   defined (__hpux))

  // Platform support DLLs, and not configured to link statically
  ACE_TCHAR arg_str[250];
  ACE_OS::sprintf (arg_str,
                   ACE_TEXT ("dynamic Logger Service_Object ")
                   ACE_TEXT ("*ACE:_make_ACE_Logging_Strategy()")
                   ACE_TEXT ("\""));

  for (int i = 1; i < argc; i++)
    {
      ACE_OS::strcat (arg_str, argv[i]);
      ACE_OS::strcat (arg_str, ACE_TEXT (" "));
    }

  ACE_OS::strcat (arg_str, ACE_TEXT ("\""));

  if (ACE_Service_Config::process_directive (arg_str) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error opening _make_ACE_Log_Strategy.\n"),
                      1);
#else // Platform doesn't support DLLs, or configured to link
      // statically
  ACE_Logging_Strategy logging_strategy;
  unsigned char ls_argc = argc - 1;
  ACE_Auto_Basic_Ptr<ACE_TCHAR *> ls_argv (new ACE_TCHAR *[ls_argc]);

  for (unsigned char c = 0; c < ls_argc; c++)
    (ls_argv.get ())[c] = argv[c+1];

  if (logging_strategy.init (ls_argc, ls_argv.get ()) == -1)
     ACE_ERROR_RETURN
       ((LM_ERROR,
         "Error initializing the ACE_Logging_Strategy.\n"),
                       1);
#endif /* !ACE_AS_STATIC_LIBS && (ACE_WIN32 ||
          ACE_HAS_SVR4_DYNAMIC_LINKING || __hpux) */

  // launch a new Thread
  if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (run_reactor)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Spawning Reactor.\n"),
                      1);

  // Function to print the message
  print_till_death ();

  // Counts the generated files
  count_files ();

  // Get the file order
  order ();

  // Wait for the thread to exit before we exit.
  ACE_Thread_Manager::instance ()->wait ();
  ACE_END_TEST;
  return 0;
}
