// -*- C++ -*-
// $Id$

#include "ServerORBInitializer.h"
#include "ServerRequestInterceptor.h"
#include "ClientRequestInterceptor2.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/ORB_Core.h"

PortableInterceptor::SlotId slot_id = 2093843221;

void
ServerORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr /* info */
    )
{
}

void
ServerORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  CORBA::Object_var obj =
    info->resolve_initial_references ("PICurrent");

  PortableInterceptor::Current_var pi_current =
    PortableInterceptor::Current::_narrow (obj.in ());

  if (CORBA::is_nil (pi_current.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR: Could not resolve PICurrent object.\n"));

      throw CORBA::INTERNAL ();
    }

  ::slot_id = info->allocate_slot_id ();

  PortableInterceptor::ServerRequestInterceptor_ptr server_tmp;
  ACE_NEW_THROW_EX (server_tmp,
                    ServerRequestInterceptor (::slot_id,
                                              pi_current.in ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ServerRequestInterceptor_var server_interceptor =
    server_tmp;

  info->add_server_request_interceptor (server_interceptor.in ());

  PortableInterceptor::ClientRequestInterceptor_ptr client_tmp;
  ACE_NEW_THROW_EX (client_tmp,
                    ClientRequestInterceptor2 (::slot_id),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ClientRequestInterceptor_var client_interceptor =
    client_tmp;

  info->add_client_request_interceptor (client_interceptor.in ());

  // Disable collocation -- TAO-specific!!!
  //
  // Collocation optimizations must be disabled since TAO doesn't
  // implement interceptor support for THRU_POA collocated
  // invocations yet, and we need to force a client request
  // interceptor to be invoked in this server.
  TAO_ORBInitInfo_var tao_info =
    TAO_ORBInitInfo::_narrow (info);

  tao_info->orb_core ()->optimize_collocation_objects (0);
}
