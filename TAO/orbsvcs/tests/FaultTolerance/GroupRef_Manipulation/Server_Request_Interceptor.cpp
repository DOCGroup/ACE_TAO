// -*- C++ -*-

#include "Server_Request_Interceptor.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "testS.h"

ACE_RCSID (ForwardRequest,
           Server_Request_Interceptor,
           "$Id$")

Server_Request_Interceptor::Server_Request_Interceptor (void)
  : request_count_ (0)
  , forward_request_thrown_ (false)
{
  this->obj_[0] = CORBA::Object::_nil ();
  this->obj_[1] = CORBA::Object::_nil ();
}

Server_Request_Interceptor::~Server_Request_Interceptor (void)
{
}

void
Server_Request_Interceptor::forward_references (
  CORBA::Object_ptr obj1,
  CORBA::Object_ptr obj2
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (obj1) || CORBA::is_nil (obj2))
    ACE_THROW (CORBA::INV_OBJREF (
                 CORBA::SystemException::_tao_minor_code (
                   TAO_DEFAULT_MINOR_CODE,
                   EINVAL),
                 CORBA::COMPLETED_NO));

  this->obj_[0] = CORBA::Object::_duplicate (obj1);
  this->obj_[1] = CORBA::Object::_duplicate (obj2);
}

char *
Server_Request_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("Server_Request_Interceptor");
}

void
Server_Request_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::release (this->obj_[0]);
  CORBA::release (this->obj_[1]);
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  this->request_count_++;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) receive_request_service_context called [%d] times \n",
              this->request_count_));

  if (this->forward_request_thrown_ == true)
    {
      IOP::ServiceContext_var svc =
        ri->get_request_service_context (IOP::FT_GROUP_VERSION
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // extract the group component
      TAO_InputCDR cdr (ACE_reinterpret_cast (const char*,
                                              svc->context_data.get_buffer ()),
                        svc->context_data.length ());

      CORBA::Boolean byte_order;

      if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
        {
          return;
        }

      cdr.reset_byte_order (ACE_static_cast (int,byte_order));

      FT::TagFTGroupTaggedComponent group_component;

      cdr >> group_component;

      if (group_component.object_group_ref_version != 5)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Error occureed \n"));
        }

    }
}

void
Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) receive_request called [%d] times \n",
              this->request_count_));

  if (this->request_count_ == 8)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "SERVER (%P|%t) Request %d will be forwarded "
                  "to object 1\n"        // "object 1" as in "obj_[0]"
                  "SERVER (%P|%t) via "
                  "receive_request_service_contexts().\n",
                  this->request_count_));

      this->forward_request_thrown_ = true;

      // Throw forward exception
      ACE_THROW (PortableInterceptor::ForwardRequest (this->obj_[1], 0));
    }

  if (this->forward_request_thrown_ == true)
    {
      IOP::ServiceContext_var svc =
        ri->get_request_service_context (IOP::FT_GROUP_VERSION
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // extract the group component
      TAO_InputCDR cdr (ACE_reinterpret_cast (const char*,
                                              svc->context_data.get_buffer ()),
                        svc->context_data.length ());

      CORBA::Boolean byte_order;

      if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
        {
          return;
        }

      cdr.reset_byte_order (ACE_static_cast (int,byte_order));

      FT::TagFTGroupTaggedComponent group_component;

      cdr >> group_component;

      if (group_component.object_group_ref_version != 5)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Error occureed \n"));
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
