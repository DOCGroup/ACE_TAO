// -*- C++ -*-
#include "Server_Request_Interceptor.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/ORB_Constants.h"
#include "tao/CDR.h"
#include "testS.h"

void
Server_Request_Interceptor::forward_reference (
  char const * iorstr)
{
  this->iorstr_ = CORBA::string_dup (iorstr);
}

char *
Server_Request_Interceptor::name ()
{
  return CORBA::string_dup ("Server_Request_Interceptor");
}

void
Server_Request_Interceptor::destroy ()
{
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  IOP::ServiceContext_var svc =
    ri->get_request_service_context (IOP::FT_GROUP_VERSION);

  // extract the group component
  TAO_InputCDR cdr (reinterpret_cast<char const *> (svc->context_data.get_buffer ()),
                    svc->context_data.length ());

  CORBA::Boolean byte_order {};

  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    {
      return;
    }

  cdr.reset_byte_order (static_cast<int> (byte_order));

  FT::TagFTGroupTaggedComponent group_component;

  cdr >> group_component.object_group_ref_version;

  if (group_component.object_group_ref_version < 11)
    {
      if (!this->request_forwarded_.load ())
        {
          int argc {};
          ACE_TCHAR **argv {};
          CORBA::String_var const orb_id = ri->orb_id ();
          CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv, orb_id.in ());

          CORBA::Object_var const obj = orb->string_to_object (this->iorstr_.in ());

          bool expected_forwarded {};
          if (this->request_forwarded_.compare_exchange_strong (
                expected_forwarded, true))
            {
              ACE_DEBUG ((LM_DEBUG,
                          "SERVER (%P|%t) Request will be forwarded "
                          "to object 1\n"
                          "SERVER (%P|%t) via "
                          "receive_request_service_contexts().\n"));
              throw PortableInterceptor::ForwardRequest (obj.in ());
            }
        }

      ACE_DEBUG ((LM_DEBUG, "object_group_ref_version is %d\n",
                  group_component.object_group_ref_version));
    }
}

void
Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}
