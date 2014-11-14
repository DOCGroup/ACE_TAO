//=============================================================================
/**
 *  @file    imore.cpp (imore stands for indirect more.)
 *
 *  $Id$
 *
 *  This program demonstrates how to redirect stdout of a parent
 *  process to the stdin of its child process using either unnamed pipe
 *  or named pipes to relay data to subprocess which runs "more" to
 *  display data on the screen.  Run imore to see how to use this
 *  program.
 *
 *  Unfortunately, on Win32, this program doesn't use any pipe at all because
 *  using pipes confuses MORE.COM on Win32 and it just acts like "cat" on Unix.
 *
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/FIFO_Recv.h"
#include "ace/FIFO_Send.h"
#include "ace/Pipe.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/Process.h"
#include "ace/Signal.h"



#if defined (ACE_WIN32)
static const ACE_TCHAR *executable = ACE_TEXT("MORE.COM");
#else
static const char * executable = "more"; // I like less better.
static const ACE_TCHAR *rendezvous_dir = ACE_TEXT("/tmp");
static const ACE_TCHAR *rendezvous_pfx = ACE_TEXT("imore");
#endif /* ACE_WIN32 */

static ACE_TCHAR *fname = 0;   // File you want to view.
static int use_named_pipe = 0; // Do we want to use named pipe?

static void
usage (void)
{
  ACE_ERROR ((LM_ERROR, "Usage: imore [-n|-u] <filename>\n"
              "\t-n Use named pipe.\n"
              "\t-u Use unnamed pipe.\n"));
}

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("un"));
  int c;

  while ((c = get_opt ()) != -1)
    {
    switch (c)
      {
      case 'n': // We want to use named pipe.
#if !defined (ACE_WIN32)
        use_named_pipe = 1;
#else
        ACE_ERROR_RETURN ((LM_ERROR, "Named pipes not supported on Win32\n"), -1);
#endif /* !ACE_WIN32 */
        break;
      case 'u':  // Use unnamed pipe.
        use_named_pipe = 0;
        break;
      default:  // What are you talking about?
        usage ();
        return -1;
      }
    }

  if (get_opt.opt_ind () >= argc) // Do you forget to give me a filename to "more?"
    {
      usage ();
      return -1;
    }
  else
    fname = argv[get_opt.opt_ind ()]; // Alright.

  return 0;
}

#if !defined (ACE_WIN32) && !defined (ACE_DISABLE_TEMPNAM)
static int
setup_named_pipes (ACE_Process_Options &opt)
{
  // Create a unique temporary name for named pipe.
  ACE_TCHAR *rendezvous = ACE_OS::tempnam (rendezvous_dir,
                                           rendezvous_pfx);

  // Out of memory?
  if (rendezvous == 0)
    return -1;

  // Alright, this is indeed strange.  Named pipes are meant to be
  // used for unrelated processes.  Because of the constraints in
  // ACE_Process, I have to pre-open the named pipes here.
  ACE_FIFO_Recv rfifo;          // read end fifo.
  ACE_FIFO_Send wfifo;          // write end fifo.

  // Check if the pipes are created successfully.
  if (rfifo.open (rendezvous) == -1 || wfifo.open (rendezvous) == -1)
    {
      ACE_OS::free (rendezvous);
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "fifo.open"), -1);
    }

  // Remove (rm, del) the file after no one uses it any more.
  ACE_OS::unlink (rendezvous);
  ACE_OS::free (rendezvous);

  // Setting up pipe between parent and child process.  Use the read
  // end of the named pipe as child process'es ACE_STDIN.
  // ACE_Process_Options will keep copies (by dup) of fd's that we
  // pass in.  Notice that we have to specify child process to use
  // ACE_STDOUT for output explicitly because we'll close it down in
  // the line after.  Child process will use whatever we use to dup2
  // ACE_STDOUT as its stdout.
  opt.set_handles (rfifo.get_handle (), ACE_STDOUT);

  // The previous keep a copy of original ACE_STDOUT fd, now we
  // can replace ACE_STDOUT of parent process to the write end
  // of the named pipe.
  ACE_OS::dup2 (wfifo.get_handle (), ACE_STDOUT);

  // Close unused fd's.  Notice ACE_FIFO doesn't close the fd
  // when it goes out of scope.
  rfifo.close ();
  wfifo.close ();
  return 0;
}
#endif

#if !defined (ACE_WIN32)
static int
setup_unnamed_pipe (ACE_Process_Options &opt)
{
  // Create an unnamed pipe instance.
  ACE_Pipe pipe;

  // Check if the pipe is created successfully.
  if (pipe.open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "pipe.open"), -1);

  // Setting up pipe between parent and child process.  Use the pipe
  // as child process'es ACE_STDIN.  ACE_Process_Options will keep
  // copies (by dup) of fd's that we pass in.  Notice that we have to
  // specify child process to use ACE_STDOUT for output explicitly
  // because we'll close it down in the line after.  Child process
  // will use whatever we use to dup2 ACE_STDOUT as its stdout.
  opt.set_handles (pipe.read_handle (), ACE_STDOUT);

  // The previous keep a copy of original ACE_STDOUT fd, now we
  // can replace ACE_STDOUT of parent process to the pipe.
  ACE_OS::dup2 (pipe.write_handle (), ACE_STDOUT);

  // Don't forget to close the unused fd.
  pipe.close ();
  return 0;
}
#endif

#if !defined (ACE_WIN32)
static int
print_file (ACE_HANDLE infd)
{
  char buffer[BUFSIZ];
  ssize_t len;

  while ((len = ACE_OS::read (infd, buffer, BUFSIZ)) > 0)
    {
      if ((ACE_OS::write (ACE_STDOUT, buffer, len) != len))
        {
          if (errno == EPIPE)
           {
              // I tried to "produce" EPIPE warning to test
              // the program but never seen one.  (odd.)
              // ACE_ERROR ((LM_ERROR, "\n\nEPIPE\n"));
              break;
            }
          else
            {
              ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "write"), -1);
            }
        }
    }

  return 0;
}
#endif

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Ignore SIGPIPE signal on Unix platforms in case
  // child process (more) terminates before we finish
  // writing to stdout.
#if !defined (ACE_WIN32)
  ACE_Sig_Action sig_act (SIG_IGN);
  if (sig_act.register_action (SIGPIPE) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Sig_Action::register_action"), -1);
#endif /* ACE_WIN32 */

  // Alright, what you want me to do now?
  if (::parse_args (argc, argv) == -1)
    return -1;

  // Can I find the file you want?
  ACE_HANDLE infile = ACE_OS::open (fname, O_RDONLY);
  if (infile == ACE_INVALID_HANDLE)
      ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", fname), -1);

  ACE_Process new_process;

  // The ACE_Process_Options does not need to be enclosed in a block
  // because it does not close the file handles, the ACE_Process closes
  // them upon destruction.
#if !defined (ACE_WIN32)
  ACE_Process_Options options;

  if (use_named_pipe)
    {
#  if defined (ACE_DISABLE_TEMPNAM)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_DISABLE_TEMPNAM set; can't use named pipes\n"),
                        -1);
#  else
      if (::setup_named_pipes (options) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "Error, bailing out!\n"), -1);
#  endif /* ACE_DISABLE_TEMPNAM */
    }
  else
    {
      if (::setup_unnamed_pipe (options) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "Error, bailing out!\n"), -1);
    }

  options.command_line (executable);
  if (new_process.spawn (options) == -1)
    {
      int const error_number = ACE_OS::last_error ();
      ACE_ERROR_RETURN ((LM_ERROR, "%p errno = %d.\n",
                         "test_more", error_number), -1);
    }

  // write file to ACE_STDOUT.
  if (::print_file (infile) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Error, bailing out!\n"), -1);

  // Close the STDOUT to inform child eof.
  ACE_OS::close (ACE_STDOUT);
#else
  // We can only pass a file handler directly to child process
  // otherwise "more" doesn't act quite the way we want.  Nonetheless,
  // if your child process don't need to interact with the terminal,
  // we can use the exact code for Unixes on NT.
  ACE_Process_Options options;
  options.command_line (executable);
  options.set_handles (infile);
  if (new_process.spawn (options) == -1)
    {
      int error = ACE_OS::last_error ();
      ACE_ERROR_RETURN ((LM_ERROR, "%p errno = %d.\n",
                         "test_more", error), -1);
    }
#endif /* ! ACE_WIN32 */

  // Wait till we are done.
  ACE_exitcode status;
  new_process.wait (&status);
  ACE_DEBUG ((LM_DEBUG, "Process exit with status %d\n", status));

  ACE_OS::close (infile);

  return 0;
}
