// $Id$
#include "child.h"

int
main (int argc, char **argv)
{
  TAO_AV_Child_Process_B<Bench_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> bench_child;

   if (bench_child.init (argc,argv) == -1)
     return 1;
   if (bench_child.run () == -1)
     return 2;

   return 0;
}
