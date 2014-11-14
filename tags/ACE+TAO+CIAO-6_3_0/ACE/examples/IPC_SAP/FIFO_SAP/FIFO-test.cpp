// $Id$

// Purpose: This program uses ACE_FIFO wrappers to perform
// interprocess communication between a parent process and a child
// process.  The parents reads from an input file and writes it into
// the fifo.  The child reads from the ACE_FIFO and executes the more
// command.

#include "ace/FIFO_Recv.h"
#include "ace/FIFO_Send.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_fcntl.h"



#define PERMS                   0666
#define EXEC_NAME               "more"
#define EXEC_COMMAND_ARG        "more"

static const ACE_TCHAR *FIFO_NAME = ACE_TEXT ("/tmp/fifo");

static int
do_child (ACE_FIFO_Recv &fifo_reader)
{
  // Set child's stdin to read from the fifo.
  if (ACE_OS::close (ACE_STDIN) == -1
      || ACE_OS::dup (fifo_reader.get_handle ()) == ACE_INVALID_HANDLE)
    return -1;

  char *argv[2];
  argv[0] = const_cast<char *> (EXEC_COMMAND_ARG);
  argv[1] = 0;

  if (ACE_OS::execvp (EXEC_NAME, argv) == -1)
    return -1;
  return 0;
}

static int
do_parent (const ACE_TCHAR fifo_name[],
           ACE_TCHAR input_filename[])
{
  ACE_FIFO_Send fifo_sender (fifo_name, O_WRONLY | O_CREAT);
  ssize_t len;
  char buf[BUFSIZ];

  if (fifo_sender.get_handle () == ACE_INVALID_HANDLE)
    return -1;

  ACE_HANDLE inputfd =
    ACE_OS::open (input_filename, O_RDONLY);

  if (inputfd == ACE_INVALID_HANDLE)
    return -1;

  // Read from input file and write into input end of the fifo.

  while ((len = ACE_OS::read (inputfd, buf, sizeof buf)) > 0)
    if (fifo_sender.send (buf, len) != len)
      return -1;

  if (len == -1)
    return -1;

  if (fifo_sender.remove () == -1)
    return -1;
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_LOG_MSG->open (argv[0]);

  if (argc != 2)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("usage: %n input-file\n"),
                  1));
      ACE_OS::exit (1);
    }

  ACE_FIFO_Recv fifo_reader (FIFO_NAME, O_RDONLY | O_CREAT, PERMS, 0);

  if (fifo_reader.get_handle () == ACE_INVALID_HANDLE)
    return -1;

  pid_t child_pid = ACE_OS::fork ();

  switch (child_pid)
    {
    case -1:
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%n: %p\n%a"),
                  ACE_TEXT ("fork"),
                  1));
    case 0:
      if (do_child (fifo_reader) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%n: %p\n%a"),
                    ACE_TEXT ("do_child"),
                    1));
    default:
      if (do_parent (FIFO_NAME, argv[1]) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%n: %p\n%a"),
                    ACE_TEXT ("do_parent"),
                    1));

      // wait for child to ACE_OS::exit.
      if (ACE_OS::waitpid (child_pid, (ACE_exitcode *) 0, 0) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%n: %p\n%a"),
                    ACE_TEXT ("waitpid"),
                    1));
    }

  return 0;
}
