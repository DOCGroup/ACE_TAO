// -*- C++ -*-
//
// $Id$

#include "CodecFactory_ORBInitializer.h"
#include "CodecFactory.h"

ACE_RCSID (TAO_CodecFactory,
           CodecFactory_ORBInitializer,
           "$Id$")

void
TAO_CodecFactory_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  // The CodecFactory is stateless and reentrant, so share a single
  // instance between all ORBs.
  info->register_initial_reference ("CodecFactory",
                                    &(this->codec_factory_),
                                    ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_CodecFactory_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
