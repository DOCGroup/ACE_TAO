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
//     This program tests the ACE_Log_Msg abstraction and demontrates
//     several common use cases.
//
// = AUTHOR
//    Douglas Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "ace/OS_main.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdlib.h"



static void
cleanup (void)
{
  ACE_DEBUG ((LM_INFO,
              "leaving (%P)!\n"));
}

static void
cause_error (void)
{
  errno = EWOULDBLOCK;
  ACE_ERROR ((LM_DEBUG,
              "would block\n"));
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int counter = 1;

  if (argc > 1) // Just give a dummy command-line argument to trigger this path.
    {
      if (ACE_LOG_MSG->open (argv[0],
                             ACE_Log_Msg::OSTREAM) == -1)
        ACE_ERROR ((LM_ERROR,
                    "cannot open logger!!!\n"));

      cause_error ();
      // Check to see what happened.
      if (ACE_LOG_MSG->op_status () == -1
          && ACE_LOG_MSG->errnum () == EWOULDBLOCK)
        ACE_DEBUG ((LM_DEBUG,
                    "op_status and errnum work!\n"));
      else
        ACE_ERROR ((LM_ERROR,
                    "op_status and errnum failed!\n"));
    }
  else   // The default behavior is to log to STDERR...
    {
      if (ACE_LOG_MSG->open (argv[0]) == -1)
        ACE_ERROR ((LM_ERROR,
                    "cannot open logger!!!\n"));

      cause_error ();

      // Check to see what happened.
      if (ACE_LOG_MSG->op_status () == -1
          && ACE_LOG_MSG->errnum () == EWOULDBLOCK)
        ACE_DEBUG ((LM_DEBUG,
                    "op_status and errnum work!\n"));
      else
        ACE_ERROR ((LM_ERROR,
                    "op_status and errnum failed!\n"));

      // Exercise many different combinations of STDERR and OSTREAM.

      double f = 3.1416 * counter++;
      int i = 10000;

      ACE_DEBUG ((LM_INFO,
                  "%10f, %*s%s = %d\n",
                  f,
                  8,
                  "",
                  "hello",
                  i));

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

      ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
      ACE_LOG_MSG->msg_ostream (&cout);

      f = 3.1416 * counter;
      i = 10000 * counter++;

      // This message will print twice - once for OSTREAM and once for
      // STDERR.

      ACE_DEBUG ((LM_INFO,
                  "%10f, %*s%s = %d\n",
                  f,
                  8,
                  "",
                  "world",
                  i));

      ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);

      f = 3.1416 * counter;
      i = 10000 * counter++;

      ACE_DEBUG ((LM_INFO,
                  "%10f, %*s%s = %d\n",
                  f,
                  8,
                  "",
                  "world",
                  i));

      ACE_LOG_MSG->msg_ostream (0);

      ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);

      f = 3.1416 * counter;
      i = 10000 * counter++;

      ACE_DEBUG ((LM_INFO,
                  "%10f, %*s%s = %d\n",
                  f,
                  8,
                  "",
                  "world",
                  i));

      ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);
      ACE_LOG_MSG->msg_ostream (&cerr);

      f = 3.1416 * counter;
      i = 10000 * counter++;

      ACE_DEBUG ((LM_INFO,
                  "%10f, %*s%s = %d\n",
                  f,
                  8,
                  "",
                  "world",
                  i));

#endif /* !defined (ACE_LACKS_IOSTREAM_TOTALLY) */

      static int array[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

      // Print out the binary bytes of the array in hex form.
      ACE_LOG_MSG->log_hexdump (LM_DEBUG,
                                (char *) array,
                                sizeof array);

      // Disable the LM_DEBUG and LM_INFO messages at the process level.
      u_long priority_mask =
        ACE_LOG_MSG->priority_mask (ACE_Log_Msg::PROCESS);
      ACE_CLR_BITS (priority_mask,
                    LM_DEBUG | LM_INFO);
      ACE_LOG_MSG->priority_mask (priority_mask,
                                  ACE_Log_Msg::PROCESS);

      ACE_DEBUG ((LM_INFO,
                  "This LM_INFO message should not print!\n"));
      ACE_DEBUG ((LM_DEBUG,
                  "This LM_DEBUG message should not print!\n"));

      ACE_SET_BITS (priority_mask,
                    LM_INFO);
      ACE_LOG_MSG->priority_mask (priority_mask,
                                  ACE_Log_Msg::PROCESS);

      ACE_DEBUG ((LM_INFO,
                  "This LM_INFO message should print!\n"));
      ACE_DEBUG ((LM_DEBUG,
                  "This LM_DEBUG message should not print!\n"));

      ACE_CLR_BITS (priority_mask, LM_INFO);
      ACE_LOG_MSG->priority_mask (priority_mask,
                                  ACE_Log_Msg::PROCESS);

      ACE_DEBUG ((LM_INFO,
                  "This LM_INFO message should not print!\n"));
      ACE_DEBUG ((LM_DEBUG,
                  "This LM_DEBUG message should not print!\n"));

      char badname[] = "badname";

      char *l_argv[2];
      l_argv[0] = badname;
      l_argv[1] = 0;

      if (ACE_OS::execv (badname,
                         l_argv) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "%n: (%x), %p%r\n",
                      10000,
                      badname,
                      cleanup));
          ACE_OS::_exit ();
        }
    }
  return 0;
}
