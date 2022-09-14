#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_sys_utsname.h"

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("ACE: %u.%u.%u\n"),
              ACE::major_version(),
              ACE::minor_version(),
              ACE::micro_version()));

  ACE_utsname uname;
  ACE_OS::uname(&uname);
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("OS: %C %C\n"),
              uname.sysname,
              uname.release));

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Compiler: %s %u.%u\n"),
              ACE::compiler_name(),
              ACE::compiler_major_version(),
              ACE::compiler_minor_version(),
              ACE::compiler_beta_version()));

  return 0;
}
