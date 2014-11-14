// -*- C++ -*-
// $Id$

#include "Server_Request_Interceptor.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/ORB_Constants.h"
#include "tao/CDR.h"
#include "testS.h"

static const CORBA::ULong expected_version = 5;

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
Server_Request_Interceptor::reset ()
{
  this->request_count_ = 0;
  this->forward_request_thrown_ = false;
}

void
Server_Request_Interceptor::forward_references (
  CORBA::Object_ptr obj1,
  CORBA::Object_ptr obj2)
{
  if (CORBA::is_nil (obj1) || CORBA::is_nil (obj2))
    throw CORBA::INV_OBJREF (
      CORBA::SystemException::_tao_minor_code (
        TAO::VMCID,
        EINVAL),
      CORBA::COMPLETED_NO);

  ACE_TCHAR *argv[] = {NULL};
  int   argc = 0;

  // Fetch the ORB having been initialized in main()
  CORBA::ORB_var orb =
    CORBA::ORB_init (argc, argv, "Server ORB");

  CORBA::String_var str1 = orb->object_to_string (obj1);

  CORBA::String_var str2 = orb->object_to_string (obj2);

  this->obj_[0] = orb->string_to_object (str1.in ());
  this->obj_[1] = orb->string_to_object (str2.in ());
}

char *
Server_Request_Interceptor::name (void)
{
  return CORBA::string_dup ("Server_Request_Interceptor");
}

void
Server_Request_Interceptor::destroy (void)
{
  CORBA::release (this->obj_[0]);
  CORBA::release (this->obj_[1]);
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  this->request_count_++;

/*  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) receive_request_service_context called [%d] times\n",
              this->request_count_));
              */

  if (this->forward_request_thrown_ == true)
    {
      IOP::ServiceContext_var svc =
        ri->get_request_service_context (IOP::FT_GROUP_VERSION);

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

      if (group_component.object_group_ref_version != expected_version)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: (%P|%t) Version does not match the"
                      "expected value (%u != %u)\n",
                      group_component.object_group_ref_version,
                      expected_version));
        }

    }
}

void
Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
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
      throw PortableInterceptor::ForwardRequest (this->obj_[1]);
    }

  if (this->forward_request_thrown_ == true)
    {
      IOP::ServiceContext_var svc =
        ri->get_request_service_context (IOP::FT_GROUP_VERSION);

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

      if (group_component.object_group_ref_version != expected_version)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: (%P|%t) Version does not match the "
                      "expected value (%u != %u)\n",
                      group_component.object_group_ref_version,
                      expected_version));
        }

    }
  return;
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
