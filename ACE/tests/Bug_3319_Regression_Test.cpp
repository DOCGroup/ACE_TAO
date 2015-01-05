/**
 * @file Bug_3319_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 2975:
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=3319
 */

#include "ace/ACE.h"
#include "test_config.h"
#include "ace/Service_Config.h"
#include "ace/ARGV.h"
#include "ace/Log_Msg.h"

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3319_Regression_Test"));

  ACE_Log_Msg *log_msg = ACE_LOG_MSG;
  ACE_ARGV args;
  args.add (ACE_TEXT("MyProgram"), true);
  int ret = ACE_Service_Config::open(args.argc(), args.argv(),
                                     ACE_DEFAULT_LOGGER_KEY,
                                     1, 1, 1);

  if (ACE_OS::strcmp (log_msg->program_name (), ACE_TEXT("MyProgram")) != 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Program name is not <MyProgram> but <%s>\n"),
                       ACE_Log_Msg::program_name ()),
                       1);
  }

  ACE_END_TEST;

  return ret;
}
