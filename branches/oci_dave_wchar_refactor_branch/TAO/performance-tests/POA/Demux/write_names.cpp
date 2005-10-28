//$Id$
#include <stdio.h>

int
ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{

  for(int i=0; i<1000; i++)
    printf ("%09d\n",i);
}
