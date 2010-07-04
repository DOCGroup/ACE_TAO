// file      : RolyPoly/ORB_Initializer.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "ace/Auto_Ptr.h"
#include "tao/corba.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/ORB_Core.h"

#include "ORB_Initializer.h"
#include "ReplicaController.h"

void
ORB_Initializer::pre_init (PortableInterceptor::ORBInitInfo_ptr)
{
}

void
ORB_Initializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  // Allocate slot id.
  //
  state_slot_id (info->allocate_slot_id ());

  // Register replica controller as server request interceptor.
  //
  TAO_ORBInitInfo* tao_info = dynamic_cast<TAO_ORBInitInfo*> (info);

  CORBA::ORB_var orb (tao_info->orb_core ()->orb ());

  PortableInterceptor::ServerRequestInterceptor_var interceptor;

  {
    PortableInterceptor::ServerRequestInterceptor *tmp_interceptor = 0;

    ACE_NEW_THROW_EX (tmp_interceptor,
                      ReplicaController (orb.in ()),
                      CORBA::NO_MEMORY (
                        CORBA::SystemException::_tao_minor_code (
                          TAO::VMCID,
                          ENOMEM),
                      CORBA::COMPLETED_NO));


    interceptor = tmp_interceptor;
  }

  info->add_server_request_interceptor (interceptor.in ());
}
