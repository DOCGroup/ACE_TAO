//$Id$
#include <ace/OS_NS_stdio.h>

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  for(int i=0; i<1000; i++)
    ACE_OS::printf ("%09d\n",i);
}
