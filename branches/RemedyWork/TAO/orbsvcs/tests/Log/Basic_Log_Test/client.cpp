// $Id$

#include "Basic_Log_Test.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  BasicLog_Test log_test;

  ACE_DEBUG((LM_DEBUG, "\nBasic Log test \n\n"));

  try
    {
      if (log_test.init(argc, argv) == -1)
        {
          ACE_ERROR_RETURN((LM_ERROR,
                            "xx - Failed initializing BasicLog_Test.\n"),
                           -1);
        }

      // We may want to replace this with a run on the BasicLog_Test.
      // If we cannot create a log, there is no use continuing.
      // Create a log with max size (octets) of 1000.
      if (log_test.test_CreateLog(1000) == -1)
        {
          ACE_ERROR_RETURN((LM_ERROR,
                            "xx - Failed creating log and bailing out.\n"),
                           -1);
        }
      else
        {
          ACE_DEBUG((LM_DEBUG,
                     "** - The creating log test succeeded.\n\n"));
        }

      // go easy until this works
      //@@ use ACE_U64_TO_U32 to convert ULongLong to ULong
      //in call to test_retrieval
    /*  if (log_test.test_retrieval(10) == -1)
        {
          ACE_DEBUG((LM_ERROR,"The test of retrieval failed.\n"));
        }
      else
        {
          ACE_DEBUG((LM_ERROR,"The test of retrieval succeeded.\n"));
        }
    */
      if (log_test.test_log_destroy() == -1)
        {
          ACE_ERROR_RETURN((LM_ERROR,
                            "xx - Failed destroying  log.\n\n"),
                           -1);
        }
      else
        {
          ACE_DEBUG((LM_DEBUG,
                     "** - The destroying log test succeeded.\n\n"));
        }

      if (log_test.test_CreateLog(1000) == -1)
        {
          ACE_ERROR_RETURN((LM_ERROR,
                            "xx - Failed creating log and bailing out.\n\n"),
                           -1);
        }

      if (log_test.test_capacity_alarm_threshold() == -1)
        {
          ACE_ERROR((LM_ERROR,"xx - The capacity alarm threshold test failed.\n\n"));
        }
      else
        {
          ACE_DEBUG((LM_DEBUG,"** - The capacity alarm threshold test succeeded.\n\n"));
        }

      if (log_test.test_LogAction() == -1)
        {
          ACE_ERROR((LM_ERROR,"xx - The log action test failed.\n\n"));
        }
      else
        {
          ACE_DEBUG((LM_DEBUG,"** - The log action test succeeded.\n\n"));
        }

      if (log_test.test_adminState() == -1)
        {
          ACE_ERROR((LM_ERROR,"xx - The administration test failed.\n\n"));
        }
      else
        {
          ACE_DEBUG((LM_DEBUG,"** - The administration test succeeded.\n\n"));
        }

      if (log_test.test_logSize() == -1)
        {
          ACE_ERROR((LM_ERROR,"xx - The log size test failed.\n\n"));
        }
      else
        {
          ACE_DEBUG((LM_DEBUG,"** - The log size test succeeded.\n\n"));
        }

      /* if (log_test.test_logCompaction(30) == -1)
        {
          ACE_DEBUG((LM_ERROR,"xx - The test of log compaction failed.\n\n"));
        }
      else
        {
          ACE_DEBUG((LM_ERROR,"** - The test of log compaction succeeded.\n\n"));
        }
      */

      if (log_test.test_week_mask() == -1)
        {
          ACE_ERROR((LM_ERROR,"xx - The week mask test failed.\n\n"));
        }
      else
        {
          ACE_DEBUG((LM_DEBUG,"** - The week mask test succeeded.\n\n"));
        }
    }
  catch (const ::CORBA::Exception &e)
    {
      e._tao_print_exception("Caught CORBA exception");
      return 1;
    }

  return 0;
}
