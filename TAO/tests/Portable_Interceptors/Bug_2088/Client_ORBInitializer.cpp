// -*- C++ -*-
#include "Client_ORBInitializer.h"

void
Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
  pre_init_called = true;

  throw CORBA::NO_MEMORY ();
}

void
Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
  post_init_called = true;

  throw CORBA::NO_MEMORY ();
}

