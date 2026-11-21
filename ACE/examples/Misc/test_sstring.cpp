#include "ace/OS_main.h"
#include "ace/SString.h"
#include "ace/Log_Msg.h"

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_CString const s1 ("hello");
  ACE_CString const s2 ("world");
  ACE_CString const s3 ("el");
  ACE_NS_WString const s4 ("hello");
  ACE_NS_WString const s5 ("world");
  ACE_NS_WString const s6 ("el");

  ACE_ASSERT (s1 != s2);
  ACE_ASSERT (s1.strstr (s2) == ACE_CString::npos);
  ACE_ASSERT (s1.strstr (s2) == ACE_CString::npos);
  ACE_ASSERT (s1.strstr (s3));
  ACE_ASSERT (s4.strstr (s5) == ACE_NS_WString::npos);
  ACE_ASSERT (s5.strstr (s6));
  return 0;
}
