// -*- C++ -*-
// $Id$

#include "ClientORBInitializer.h"
#include "ClientRequestInterceptor.h"

#include "tao/ORB_Constants.h"

#include "ace/Log_Msg.h"

PortableInterceptor::SlotId slot_id = 2093843211;

void
ClientORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr /* info */
    )
{
}

void
ClientORBInitializer::post_init (
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

  PortableInterceptor::ClientRequestInterceptor_ptr foo;
  ACE_NEW_THROW_EX (foo,
                    ClientRequestInterceptor (::slot_id,
                                              pi_current.in ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ClientRequestInterceptor_var interceptor =
    foo;

  info->add_client_request_interceptor (interceptor.in ());
}
