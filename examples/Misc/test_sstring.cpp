// $Id$

#include "ace/SString.h"

int 
main (int, char *[])
{
  ACE_CString s1 ("hello");
  ACE_CString s2 ("world");
  ACE_CString s3 ("el");
  ACE_WString s4 ("hello");
  ACE_WString s5 ("world");
  ACE_WString s6 ("el");
  
  ACE_ASSERT (s1 != s2);
  ACE_ASSERT (s1.strstr (s2) == -1);
  ACE_ASSERT (s1.strstr (s2) == -1);
  ACE_ASSERT (s1.strstr (s3));
  ACE_ASSERT (s4.strstr (s5) == -1);
  ACE_ASSERT (s5.strstr (s6));
  return 0;
}
