#include "server_interceptor.h"
#include "tao/OctetSeqC.h"

#include "ace/Log_Msg.h"

#include <cstring>

Ping_Death_Request_Interceptor::Ping_Death_Request_Interceptor (int *counter)
  : counter_ (counter),
    poa_ (PortableServer::POA::_nil())
{
}

void
Ping_Death_Request_Interceptor::set_poa (PortableServer::POA_ptr poa)
{
  poa_ = PortableServer::POA::_duplicate (poa);
}

char *
Ping_Death_Request_Interceptor::name ()
{
  return CORBA::string_dup ("Ping_Death_Interceptor");
}

void
Ping_Death_Request_Interceptor::destroy ()
{
}

void
Ping_Death_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var operation = ri->operation ();

  if (std::strcmp (operation.in (), "ping") != 0)
    {
      return;
    }

  if (this->counter_ != nullptr)
    {
      ++(*counter_);
    }
  if (CORBA::is_nil(this->poa_.in()))
    {
      return;
    }
  try
    {
      this->poa_->destroy (true,true);
    }
  catch (const CORBA::Exception &ex)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P) deactivate raised %C\n",
                  ex._name()));
    }
  throw ::CORBA::TRANSIENT
    ( CORBA::SystemException::_tao_minor_code (TAO_POA_HOLDING, 1),
      CORBA::COMPLETED_NO);
}

void
Ping_Death_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Ping_Death_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}

void
Ping_Death_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}

void
Ping_Death_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}
