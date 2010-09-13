// $Id$

#include "ace/Init_ACE.h"
#include "ace/Log_Msg.h"
#include "test_config.h"

extern "C"
{

ACE_Proper_Export_Flag int capi_init(void)
{
  int ret = ACE::init();
  ACE_START_TEST (ACE_TEXT ("Bug_2980_Regression_Test"));
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("ACE::init ()\n")));
  ACE_END_TEST;
  return ret;
}


ACE_Proper_Export_Flag int capi_fini(void)
{
  ACE_START_TEST (ACE_TEXT ("Bug_2980_Regression_Test"));
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("ACE::fini ()\n")));
  ACE_END_TEST;

  return ACE::fini();
}


ACE_Proper_Export_Flag int capi_dosomething(void)
{
  ACE_START_TEST (ACE_TEXT ("Bug_2980_Regression_Test"));

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) capi_dosomething - entered\n")
  ));

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) capi_dosomething - leaving\n")
  ));

  ACE_END_TEST;
  return 0;
}


}
