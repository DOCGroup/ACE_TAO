// -*- C++ -*-
#include "Server_ORBInitializer.h"
#include "server_interceptor.h"
extern PortableInterceptor::SlotId slot_id;

void
Server_ORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr)
{
}

void
Server_ORBInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  // allocate a slot
  slot_id = info->allocate_slot_id ();
  ACE_DEBUG ((LM_DEBUG,
              "Server_ORBInitializer::post_init() : allocated slot id: %d\n",
              static_cast<int> (slot_id)));

  // Install the Echo server request interceptor
  PortableInterceptor::ServerRequestInterceptor_ptr temp_interceptor {};
  ACE_NEW_THROW_EX (temp_interceptor,
                    Echo_Server_Request_Interceptor,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ServerRequestInterceptor_var const interceptor (temp_interceptor);

  info->add_server_request_interceptor (interceptor.in ());
}

