/* Purpose:  This program uses ACE_FIFO wrappers to perform interprocess
// $Id$

   communication between a parent process and a child process.        
   The parents reads from an input file and writes it into the fifo.  
   The child reads from the ACE_FIFO and executes the more command.       */

#include "ace/Log_Msg.h"
#include "ace/FIFO_Recv.h"
#include "ace/FIFO_Send.h"

#define PERMS                   0666
#define EXEC_NAME               "more"
#define EXEC_COMMAND_ARG        "more"

const char *FIFO_NAME = "/tmp/fifo";

int
do_child (ACE_FIFO_Recv &fifo_reader)
{
  /* Set child's stdin to read from the fifo */
  if (ACE_OS::close (0) == -1 || ACE_OS::dup (fifo_reader.get_handle ()) == -1)
    return -1;

  char *argv[2];
  argv[0] = EXEC_COMMAND_ARG;
  argv[1] = 0;

  if (ACE_OS::execvp (EXEC_NAME, argv) == -1)
    return -1;
}

int 
do_parent (const char fifo_name[], char input_filename[])
{
  int       inputfd;
  ACE_FIFO_Send fifo_sender (fifo_name, O_WRONLY | O_CREAT);
  int       len;
  char      buf[BUFSIZ];

  if (fifo_sender.get_handle () == ACE_INVALID_HANDLE)
    return -1;

  if ((inputfd = ACE_OS::open (input_filename, O_RDONLY)) == -1)
    return -1;

  /* Read from input file and write into input end of the fifo */

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
main (int argc, char *argv[])
{
  ACE_LOG_MSG->open (argv[0]);

  if (argc != 2)
    ACE_ERROR ((LM_ERROR, "usage: %n input-file\n%a", 1));

  ACE_FIFO_Recv fifo_reader (FIFO_NAME, O_RDONLY | O_CREAT, PERMS, 0);

  if (fifo_reader.get_handle () == ACE_INVALID_HANDLE)
    return -1;

  pid_t child_pid;

  switch (child_pid = ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR ((LM_ERROR, "%n: %p\n%a", "fork", 1));
    case 0:
      if (do_child (fifo_reader) == -1)
        ACE_ERROR ((LM_ERROR, "%n: %p\n%a", "do_child", 1));
    default:
      if (do_parent (FIFO_NAME, argv[1]) == -1)
        ACE_ERROR ((LM_ERROR, "%n: %p\n%a", "do_parent", 1));

      if (ACE_OS::waitpid (child_pid, (int *) 0, 0) == -1)    /* wait for child to ACE_OS::exit */
        ACE_ERROR ((LM_ERROR, "%n: %p\n%a", "waitpid", 1));
    }

  return 0;
}
