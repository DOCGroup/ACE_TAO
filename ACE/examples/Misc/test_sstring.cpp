// $Id$

#include "ace/OS_main.h"
#include "ace/SString.h"
#include "ace/Log_Msg.h"



int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_CString s1 ("hello");
  ACE_CString s2 ("world");
  ACE_CString s3 ("el");
  ACE_NS_WString s4 ("hello");
  ACE_NS_WString s5 ("world");
  ACE_NS_WString s6 ("el");

  ACE_ASSERT (s1 != s2);
  ACE_ASSERT (s1.strstr (s2) == ACE_CString::npos);
  ACE_ASSERT (s1.strstr (s2) == ACE_CString::npos);
  ACE_ASSERT (s1.strstr (s3));
  ACE_ASSERT (s4.strstr (s5) == ACE_NS_WString::npos);
  ACE_ASSERT (s5.strstr (s6));
  return 0;
}
