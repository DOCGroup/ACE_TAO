// -*- C++ -*-
#include "Server_Request_Interceptor.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/ORB_Constants.h"
#include "tao/CDR.h"
#include "testS.h"

static constexpr CORBA::ULong expected_version {5};

static void
check_group_version (PortableInterceptor::ServerRequestInfo_ptr request_info)
{
  IOP::ServiceContext_var const service_context =
    request_info->get_request_service_context (IOP::FT_GROUP_VERSION);
  TAO_InputCDR cdr (
    reinterpret_cast<char const *> (service_context->context_data.get_buffer ()),
    service_context->context_data.length ());

  CORBA::Boolean byte_order {};
  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return;

  cdr.reset_byte_order (static_cast<int> (byte_order));

  CORBA::ULong version {};
  if ((cdr >> version) == 0)
    return;

  if (version != expected_version)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: (%P|%t) Version does not match the expected value (%u != %u)\n",
                  version,
                  expected_version));
    }
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

  this->obj_[0] = CORBA::Object::_duplicate (obj1);
  this->obj_[1] = CORBA::Object::_duplicate (obj2);
}

char *
Server_Request_Interceptor::name ()
{
  return CORBA::string_dup ("Server_Request_Interceptor");
}

void
Server_Request_Interceptor::destroy ()
{
  this->obj_[0] = CORBA::Object::_nil ();
  this->obj_[1] = CORBA::Object::_nil ();
}

void
Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  ++this->request_count_;

  if (this->forward_request_thrown_)
    check_group_version (ri);
}

void
Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  if (this->request_count_ == 8)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "SERVER (%P|%t) Request " ACE_UINT32_FORMAT_SPECIFIER " will be forwarded "
                  "to object 1\n"        // "object 1" as in "obj_[0]"
                  "SERVER (%P|%t) via "
                  "receive_request_service_contexts().\n",
                  this->request_count_));

      this->forward_request_thrown_ = true;

      // Throw forward exception
      throw PortableInterceptor::ForwardRequest (this->obj_[1].in ());
    }

  if (this->forward_request_thrown_)
    {
      check_group_version (ri);
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
