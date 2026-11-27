#include <ace/OS_NS_stdio.h>

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  for(int i=0; i<1000; ++i)
    ACE_OS::printf ("%09d\n",i);
}
