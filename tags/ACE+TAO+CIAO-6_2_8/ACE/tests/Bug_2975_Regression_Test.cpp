/**
 * @file Bug_2975_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 2975:
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=2975
 *
 * @author Pavel Zaichenko <zaichep@mail.ru>
 */

#include "ace/ACE.h"
#include "test_config.h"
#include "ace/Service_Config.h"
#include "ace/ARGV.h"

int
run_main (int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2975_Regression_Test"));

  int ret = 0;

  ACE_ARGV args;
  args.add (argv[0], true);
  args.add (ACE_TEXT("-S"));
#if (ACE_USES_CLASSIC_SVC_CONF == 1)
  args.add (
      ACE_TEXT ("\"dynamic ACE_Logger Service_Object * ")
      ACE_TEXT ("ACE:_make_ACE_Logging_Strategy() ")
      ACE_TEXT ("'-p ~TRACE|DEBUG|INFO|WARNING|NOTICE|ERROR|CRITICAL|ALERT'")
      ACE_TEXT ("\""));
#else // not (ACE_USES_CLASSIC_SVC_CONF == 1)
  args.add (
      ACE_TEXT ("'<ACE_Svc_Conf>")
      ACE_TEXT ("<dynamic id=\"ACE_Logger\"")
      ACE_TEXT (" type=\"Service_Object\">")
      ACE_TEXT ("<initializer path=\"ACE\"")
      ACE_TEXT (" init=\"_make_ACE_Logging_Strategy\"")
      ACE_TEXT (" params=\"-p ~TRACE|DEBUG|INFO|WARNING|NOTICE|ERROR|CRITICAL|ALERT\"/>")
      ACE_TEXT ("</dynamic>")
      ACE_TEXT ("</ACE_Svc_Conf>'"));
#endif // not (ACE_USES_CLASSIC_SVC_CONF == 1)

  ret = ACE_Service_Config::open(args.argc(), args.argv(),
                                 ACE_DEFAULT_LOGGER_KEY,
                                 1, 1);

  if (0 != ret)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("ACE_Service_Config::open() failed\n")));
  }

  ACE_END_TEST;

  return 0;
}
