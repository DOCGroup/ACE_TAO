// $Id$

// ============================================================================
//
// = LIBRARY
//    examples/Log_Msg
// 
// = FILENAME
//    test_log_msg.cpp
//
// = DESCRIPTION
//     This program tests the Log_Msg abstraction and demontrates 
//     several use cases.  
//
// = AUTHOR
//    Douglas Schmidt
// 
// ============================================================================


#include "ace/OS.h"
#include "ace/streams.h"

ACE_RCSID(Log_Msg, test_log_msg, "$Id$")

static void
cleanup (void)
{
  ACE_DEBUG ((LM_INFO, "leaving (%P)!\n"));
}

static void
cause_error (void)
{
  errno = EWOULDBLOCK;
  ACE_ERROR ((LM_DEBUG, "would block\n"));
}

int
main (int argc, char *argv[])
{
  // Note that the default behavior is to log to STDERR...

  int counter = 1 ;

  if (argc > 1)
    {
      if (ACE_LOG_MSG->open (argv[0], ACE_Log_Msg::OSTREAM) == -1)
	ACE_ERROR ((LM_ERROR, "cannot open logger!!!\n"));

      cause_error ();
      // Check to see what happened.
      if (ACE_LOG_MSG->op_status () == -1 
	  && ACE_LOG_MSG->errnum () == EWOULDBLOCK)
	ACE_DEBUG ((LM_DEBUG, "op_status and errnum work!\n"));
      else
	ACE_ERROR ((LM_ERROR, "op_status and errnum failed!\n"));
    }
  else
    {
      if (ACE_LOG_MSG->open (argv[0]) == -1)
	ACE_ERROR ((LM_ERROR, "cannot open logger!!!\n"));

      cause_error ();

      // Check to see what happened.
      if (ACE_LOG_MSG->op_status () == -1 
	  && ACE_LOG_MSG->errnum () == EWOULDBLOCK)
	ACE_DEBUG ((LM_DEBUG, "op_status and errnum work!\n"));
      else
	ACE_ERROR ((LM_ERROR, "op_status and errnum failed!\n"));

      // Exercise many different combinations of STDERR and OSTREAM.

      ACE_DEBUG ((LM_INFO, "%10f, %*s%s = %d\n", 
		  3.1416 * counter++, 8, "", "hello", 10000));

      ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
      ACE_LOG_MSG->msg_ostream (&cout);

      ACE_DEBUG ((LM_INFO, "%10f, %*s%s = %d\n", 
		  3.1416 * counter, 8, "", "world", 10000 * counter++));

      ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);

      ACE_DEBUG ((LM_INFO, "%10f, %*s%s = %d\n", 
		  3.1416 * counter, 8, "", "world", 10000 * counter++));

      ACE_LOG_MSG->msg_ostream (0);

      ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);

      ACE_DEBUG ((LM_INFO, "%10f, %*s%s = %d\n", 
		  3.1416 * counter, 8, "", "world", 10000 * counter++));

      ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);
      ACE_LOG_MSG->msg_ostream (&cerr);

      ACE_DEBUG ((LM_INFO, "%10f, %*s%s = %d\n", 
		  3.1416 * counter, 8, "", "world", 10000 * counter++));

      static int array[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

      // Print out the binary bytes of the array in hex form.
      ACE_LOG_MSG->log_hexdump (LM_DEBUG, (char *) array, sizeof array);

      // Disable the LM_DEBUG and LM_INFO messages.
      int priority_mask = ACE_LOG_MSG->priority_mask ();
      ACE_CLR_BITS (priority_mask, LM_DEBUG | LM_INFO);
      ACE_LOG_MSG->priority_mask (priority_mask);

      ACE_DEBUG ((LM_INFO, "This LM_INFO message should not print!\n"));
      ACE_DEBUG ((LM_DEBUG, "This LM_DEBUG message should not print!\n"));

      char *badname = "badname";

      char *l_argv[2];
      l_argv[0] = badname;
      l_argv[1] = 0;

      if (ACE_OS::execv (badname, l_argv) == -1)
	ACE_ERROR ((LM_ERROR, "%n: (%x), %p%r%a\n", 
		    10000, badname, cleanup, 1));
    }
  return 0;
}
