// -*- C++ -*-
//
// $Id$

#include "CodecFactory_ORBInitializer.h"
#include "CodecFactory.h"
#include "ORBInitInfo.h"
#include "SystemException.h"
#include "ORB_Constants.h"

ACE_RCSID (TAO_CodecFactory,
           CodecFactory_ORBInitializer,
           "$Id$")

void
TAO_CodecFactory_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ORBInitInfo_var tao_info =
    TAO_ORBInitInfo::_narrow (info
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_ORB_Core * orb_core = tao_info->orb_core ();

  IOP::CodecFactory_ptr codec_factory;
  ACE_NEW_THROW_EX (codec_factory,
                    TAO_CodecFactory (orb_core),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  IOP::CodecFactory_var cf = codec_factory;

  info->register_initial_reference ("CodecFactory",
                                    codec_factory
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CodecFactory_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
