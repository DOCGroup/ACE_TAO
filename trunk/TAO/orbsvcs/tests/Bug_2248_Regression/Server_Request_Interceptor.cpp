// -*- C++ -*-
// $Id$

#include "Server_Request_Interceptor.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/ORB_Constants.h"
#include "tao/CDR.h"
#include "testS.h"

Server_Request_Interceptor::Server_Request_Interceptor (void)
    : request_forwarded_ (0)
{
}

Server_Request_Interceptor::~Server_Request_Interceptor (void)
{
}

void
Server_Request_Interceptor::forward_reference (
  const char* iorstr
  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->iorstr_ = CORBA::string_dup (iorstr);
}

char *
Server_Request_Interceptor::name (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("Server_Request_Interceptor");
}

void
Server_Request_Interceptor::destroy (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr //
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

}

void
Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  IOP::ServiceContext_var svc =
    ri->get_request_service_context (IOP::FT_GROUP_VERSION
                                     ACE_ENV_ARG_PARAMETER);

  // extract the group component
  TAO_InputCDR cdr (reinterpret_cast<const char*> (svc->context_data.get_buffer ()),
                    svc->context_data.length ());

  CORBA::Boolean byte_order;

  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    {
      return;
    }

  cdr.reset_byte_order (static_cast<int> (byte_order));

  FT::TagFTGroupTaggedComponent group_component;

  cdr >> group_component.object_group_ref_version;

  if (group_component.object_group_ref_version < 11)
    {
      if (!request_forwarded_)
        {
          int argc = 0;
          char** argv = 0;
          CORBA::ORB_var orb = CORBA::ORB_init(argc,argv,ri->orb_id() ACE_ENV_ARG_PARAMETER);

          CORBA::Object_var obj = orb->string_to_object (iorstr_.in() ACE_ENV_ARG_PARAMETER);

          ACE_DEBUG ((LM_DEBUG,
                    "SERVER (%P|%t) Request will be forwarded "
                    "to object 1\n" // "object 1" as in "obj_[0]"
                    "SERVER (%P|%t) via "
                    "receive_request_service_contexts().\n"));
          request_forwarded_ = 1;
          // Throw forward exception
          ACE_THROW (PortableInterceptor::ForwardRequest (obj.in()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
          "object_group_ref_version is %d\n", group_component.object_group_ref_version));
        }
    }

  return;
}

void
Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}
