// $Id$
#include "child.h"

int
main (int argc, char **argv)
{
  TAO_AV_Child_Process_B<Bench_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> bench_child;
  CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        argv);

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in (),
                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var mgr
        = poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      mgr->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (bench_child.init (argc,
                            argv,
                            orb.in (), poa.in ()) == -1)
        return 1;
      if (bench_child.run () == -1)
        return 2;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_AV_Child_Process_B<Bench_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Child_Process<Bench_Server_StreamEndPoint, TAO_VDev, AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_AV_Child_Process_B<Bench_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Child_Process<Bench_Server_StreamEndPoint, TAO_VDev, AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
