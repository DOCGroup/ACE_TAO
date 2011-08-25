// -*- C++ -*-
// $Id$

#include "tao/ORBInitializer_Registry.h"

#include "tao/PortableServer/PortableServer.h"

#include "tao/PI/PI.h"

#include "LWFT_Server_Init.h"
#include "ServerORBInitializer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool LWFT_Server_Init::initialized_ = false;

LWFT_Server_Init::LWFT_Server_Init (void)
{
}

LWFT_Server_Init::~LWFT_Server_Init (void)
{
}

CORBA::ORB_ptr
LWFT_Server_Init::pre_init (int & /* argc */, char * /* argv */[])
{
  CORBA::ORB_var the_orb;

  return the_orb._retn ();
}

int
LWFT_Server_Init::Initializer (void)
{
  try
    {
      if (! LWFT_Server_Init::initialized_)
        {
          PortableInterceptor::ORBInitializer_ptr oi_ptr;

          ACE_NEW_RETURN (oi_ptr,
                          ServerORBInitializer,
                          -1); // No CORBA exceptions yet!

          PortableInterceptor::ORBInitializer_var orb_initializer =
            oi_ptr;
          PortableInterceptor::register_orb_initializer (
            orb_initializer.in ());
        
          LWFT_Server_Init::initialized_ = true;
        }
    }
  catch (CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_ERROR, "LWFT_Server_Init::init: "));
    }
    
  return 0;
}
