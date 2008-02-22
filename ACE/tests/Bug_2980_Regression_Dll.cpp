#include "ace/Init_ACE.h"
#include "ace/OS.h"
#include "ace/Log_Msg.h"


extern "C"
{

ACE_Proper_Export_Flag void capi_init(void)
{
  ACE::init();
  ACE_OS::printf("ACE::init()\n");
}


ACE_Proper_Export_Flag void capi_fini(void)
{
  ACE_OS::printf("ACE::fini()\n");
  ACE::fini();
}


ACE_Proper_Export_Flag void capi_dosomething(void)
{
  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) capi_dosomething - entered\n")
  ));

  ACE_DEBUG((
    LM_DEBUG,
    ACE_TEXT ("(%P|%t) capi_dosomething - leaving\n")
  ));
}


}
