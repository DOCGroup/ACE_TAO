// -*- C++ -*-

#include "ServerORBInitializer.h"
#include "ServerRequestInterceptor.h"

ACE_RCSID (PICurrent,
           ServerORBInitializer,
           "$Id$")

PortableInterceptor::SlotId slot_id = 2093843221;

void
ServerORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr /* info */
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ServerORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::slot_id = info->allocate_slot_id (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableInterceptor::ServerRequestInterceptor_ptr tmp;
  ACE_NEW_THROW_EX (tmp,
                    ServerRequestInterceptor (slot_id),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::ServerRequestInterceptor_var interceptor = tmp;

  info->add_server_request_interceptor (interceptor.in ()
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
