// ============================================================================
// $Id$

//
// = LIBRARY
//    examples
// 
// = FILENAME
//    process.cpp
//
// = DESCRIPTION
//    This example tests the ACE_Process.  For more info, check the
//    README file in this directory.
//
// = AUTHOR
//    Tim Harrison.
// 
// ============================================================================

#include "ace/OS.h"

#include "ace/Process.h"

#if defined (ACE_WIN32)
#define EXEC_NAME "c:\\WINNT35\\system32\\MORE.COM"
#else
#define EXEC_NAME "/usr/bin/cat";
#endif /* ACE_WIN32 */

int
main (int argc, char *argv[])
{
  if (ACE_LOG_MSG->open (argv[0]) == -1)
    ACE_ERROR ((LM_ERROR, "cannot open logger!!!\n"));

  char *executable = argc > 1 ? argv[1] : EXEC_NAME;
  char *input_file = argc > 2 ? argv[2] : "process.cpp";

  ACE_HANDLE infile = ACE_OS::open (input_file, O_RDONLY);

  if (infile == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", input_file), 1);

  static char *l_argv[2]; // Starts out with 0's ;-)
  l_argv[0] = executable;

  // Try to create a new process running the <executable>.
  ACE_Process new_process (l_argv, infile);

  if (ACE_LOG_MSG->op_status () == -1)
    {
      int error = ACE_OS::last_error ();
      ACE_ERROR ((LM_ERROR, "%p errno = %d.\n",
		  executable, error));
    }

  new_process.wait ();
  ACE_OS::close (infile);

  ACE_DEBUG ((LM_DEBUG, "Goodbye.\n"));
  return 42;
}


