// -*- C++ -*-

#include "Server_Request_Interceptor.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/ORB_Constants.h"
#include "tao/CDR.h"
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
Server_Request_Interceptor::reset (ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->request_count_ = 0;
  this->forward_request_thrown_ = false;
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
                   TAO::VMCID,
                   EINVAL),
                 CORBA::COMPLETED_NO));

  char *argv[] = {NULL};
  int   argc = 0;
  
  // Fetch the ORB having been initialized in main()
  CORBA::ORB_var orb =
    CORBA::ORB_init (argc, argv, "Server ORB" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var str1 = orb->object_to_string (obj1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var str2 = orb->object_to_string (obj2 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->obj_[0] = orb->string_to_object (str1.in () ACE_ENV_ARG_PARAMETER);
  this->obj_[1] = orb->string_to_object (str2.in () ACE_ENV_ARG_PARAMETER);
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

/*  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) receive_request_service_context called [%d] times \n",
              this->request_count_));
              */

  if (this->forward_request_thrown_ == true)
    {
      IOP::ServiceContext_var svc =
        ri->get_request_service_context (IOP::FT_GROUP_VERSION
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

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

      //cdr >> group_component.component_version;
      //cdr >> group_component.group_domain_id.inout ();
      //cdr >> group_component.object_group_id;
      cdr >> group_component.object_group_ref_version;

      if (group_component.object_group_ref_version != 5)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Error occurred \n"));
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
      ACE_THROW (PortableInterceptor::ForwardRequest (this->obj_[1]));
    }

  if (this->forward_request_thrown_ == true)
    {
      IOP::ServiceContext_var svc =
        ri->get_request_service_context (IOP::FT_GROUP_VERSION
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

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

      //cdr >> group_component.component_version;
      //cdr >> group_component.group_domain_id.inout ();
      //cdr >> group_component.object_group_id;
      cdr >> group_component.object_group_ref_version;

      if (group_component.object_group_ref_version != 5)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Error occurred \n"));
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
