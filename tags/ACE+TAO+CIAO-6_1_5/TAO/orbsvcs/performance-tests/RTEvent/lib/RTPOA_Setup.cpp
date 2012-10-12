/**
 * @file RTPOA_Setup.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "RTPOA_Setup.h"
#include "RIR_Narrow.h"
#include "RTCORBA_Setup.h"
#include "tao/RTCORBA/RTCORBA.h"

#include "ace/Log_Msg.h"

#if !defined(__ACE_INLINE__)
#include "RTPOA_Setup.inl"
#endif /* __ACE_INLINE__ */

RTPOA_Setup::RTPOA_Setup (CORBA::ORB_ptr orb,
                          const RTCORBA_Setup &rtcorba_setup)
{
  RTPortableServer::POA_var root_poa =
    RIR_Narrow<RTPortableServer::POA>::resolve (orb,
                                                "RootPOA");

  RTCORBA::RTORB_var rtorb =
    RIR_Narrow<RTCORBA::RTORB>::resolve (orb,
                                         "RTORB");

  const CORBA::ULong stacksize = 1024 * 1024; // 1 Mb
  const RTCORBA::ThreadpoolLanes &lanes = rtcorba_setup.lanes ();
  const CORBA::Boolean allow_borrowing = 0;
  const CORBA::Boolean allow_request_buffering = 0;
  const CORBA::ULong max_buffered_requests   = 0; // dummy value
  const CORBA::ULong max_request_buffer_size = 0; // dummy value

  RTCORBA::ThreadpoolId pool_id =
    rtorb->create_threadpool_with_lanes (stacksize,
                                         lanes,
                                         allow_borrowing,
                                         allow_request_buffering,
                                         max_buffered_requests,
                                         max_request_buffer_size);

  // @@ We need an 'auto_ptr for thread pools' here!
  CORBA::PolicyList policies (4); policies.length (4);
  policies[0] =
    rtorb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                         rtcorba_setup.process_priority ());

  policies[1] =
    root_poa->create_id_assignment_policy (PortableServer::SYSTEM_ID);

  policies[2] =
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

#if 0
  policies.length (3);
#else
  policies[3] =
    rtorb->create_threadpool_policy (pool_id);
#endif /* 0 */

  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager ();

  this->poa_ =
    root_poa->create_POA ("RTEC_Perf",
                          poa_manager.in (),
                          policies);

  for (CORBA::ULong i = 0; i != policies.length (); ++i)
    {
      policies[i]->destroy ();
    }
}
