//$Id$
#include <ace/OS_NS_stdio.h>

int
main(int argc, char* argv[])
{

  for(int i=0; i<1000; i++)
    ACE_OS::printf ("%09d\n",i);
}
