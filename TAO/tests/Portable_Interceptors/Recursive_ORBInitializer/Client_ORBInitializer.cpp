// -*- C++ -*-
//
// $Id$
//

#include "Client_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"

ACE_RCSID (Recursive_ORBInitializer,
           Client_ORBInitializer,
           "$Id$")

Client_ORBInitializer::Client_ORBInitializer (Client_ORBInitializer *second) :
  pre_init_called (0),
  post_init_called (0),
  second_ (second)
{
}

void
Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ++pre_init_called;

  if (second_ != 0)
    {
      PortableInterceptor::register_orb_initializer (second_
                                                     ACE_ENV_ARG_PARAMETER);

      // Registered the initializer, clear the pointer so that we only do it
      // once
      second_ = 0;
    }
}

void
Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ++post_init_called;
}

