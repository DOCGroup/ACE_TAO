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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_AV_Child_Process_B<Bench_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Child_Process<Bench_Server_StreamEndPoint, TAO_VDev, AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_AV_Child_Process_B<Bench_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Child_Process<Bench_Server_StreamEndPoint, TAO_VDev, AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
