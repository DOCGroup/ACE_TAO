// file      : RolyPoly/ORB_Initializer.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "tao/corba.h"
#include "tao/ORBInitInfo.h"
#include "tao/ORB_Core.h"

#include "ORB_Initializer.h"
#include "ReplicaController.h"

void
ORB_Initializer::pre_init (PortableInterceptor::ORBInitInfo_ptr
                           ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ORB_Initializer::post_init (PortableInterceptor::ORBInitInfo_ptr info
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Allocate slot id.
  //
  state_slot_id (info->allocate_slot_id ());

  // Register replica controller as server request interceptor.
  //
  TAO_ORBInitInfo* tao_info (dynamic_cast<TAO_ORBInitInfo*> (info));

  CORBA::ORB_var orb (tao_info->orb_core ()->orb ());

  PortableInterceptor::ServerRequestInterceptor_var interceptor;

  ACE_NEW_THROW_EX (interceptor,
                    ReplicaController (orb.in ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  ACE_CHECK;

  info->add_server_request_interceptor (interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
